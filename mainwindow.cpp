#include "intropage.h"
#include "introparamform.h"
#include "mainwindow.h"
#include "phase.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QDockWidget>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QProgressDialog>
#include <QSettings>
#include <QTextBrowser>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    statusBar()->setVisible(true);


    statusLabel = new QLabel(tr("Application chargée"),this);
    progress = new QProgressBar(this);

    stackedWidget = new QStackedWidget(this) ;
    setCentralWidget(stackedWidget);

    QToolBar * toolbar = new QToolBar(this) ;
    toolbar->setObjectName(tr("Navigation"));
    addToolBar(toolbar);
    toolbar->setIconSize(QSize(32,32));

    QIcon icon_next(QString(":/images/next64.png")) ;
    QIcon icon_back(QString(":/images/prev64.png")) ;

    back = new QPushButton(this);
    back->setIcon(icon_back);
    back->setIconSize(QSize(32,32));
    back->setEnabled(false);
    back->setToolTip(tr("Pas de page précédente."));
    connect(back,SIGNAL(clicked(bool)),this,SLOT(prevPhase()));
    toolbar->addWidget(back);


    title = new QLabel(tr("PHASE 0 : INTRODUCTION"),this) ;
    QFont font = title->font() ;
    font.setPixelSize(24);
    title->setFont(font) ;
    toolbar->addWidget(title );

    next = new QPushButton(this);
    next->setIcon(icon_next);
    next->setIconSize(QSize(32,32));
    next->setEnabled(true);
    next->setToolTip(tr("PHASE 1 : RECADRAGE"));
    connect(next,SIGNAL(clicked(bool)),this,SLOT(nextPhase()));
    toolbar->addWidget(next);

    toolbar->addWidget(progress);

    regularizedImage = NULL ;openedImage = NULL ;
    texture =  NULL ;

    kdtree = NULL ;

    phase = -1 ;

    scale =  1 ;

    nextPhase();
}

MainWindow::~MainWindow()
{
    QSettings settings ;
    settings.setValue(QString("geometry"), saveGeometry());

    //delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::setPhase(int p)
{
    phase = p ;
    stackedWidget->setCurrentIndex(phase);
    restorePhase();
    ( (Page*)(stackedWidget->currentWidget()))->reinit();
    repaint() ;

}

void MainWindow::batchProcess(){
    QStringList files = settings.value("FileList").toStringList() ;

    QProgressDialog progressDialog(tr("Traitement en cours..."),tr("Annuler le traitement"), 8, 8*files.count(), this);
    progressDialog.setWindowModality(Qt::WindowModal);
    QDir dir(settings.value("IntroParamForm-File-1").toString());
    QString new_dir_path = tr("Dossier %1").arg(QFileInfo(files[0]).baseName());
    QDir new_dir(dir.absoluteFilePath(new_dir_path));
    new_dir.removeRecursively();
    dir.mkdir(new_dir_path);
    dir.cd(new_dir_path);
    for (int f = 0 ; f < savedFilesPaths.count() ; f++)
    {
        QString new_path = dir.absoluteFilePath(QFileInfo(savedFilesPaths[f]).fileName()) ;
        QFile::remove(new_path);
        QFile::rename(savedFilesPaths[f],new_path);
    }

    for (int i = 1 ; i < files.count() ; i ++ )
    {
        savedFilesPaths.clear();
        QDir dir(settings.value("IntroParamForm-File-1").toString());
        settings.setValue("File",dir.absoluteFilePath(files[i]));
        initialImage.load(dir.absoluteFilePath(files[i]));
        QString new_dir_path = tr("Dossier %1").arg(QFileInfo(files[i]).baseName());
        QDir new_dir(dir.absoluteFilePath(new_dir_path));
        new_dir.removeRecursively();
        dir.mkdir(new_dir_path);
        dir.cd(new_dir_path);
        for (int phase = 1 ; phase < 9 ; phase++)
        {
            setPhase(phase);
            while (! ( (Page*)(stackedWidget->currentWidget()))->initDone)
                qApp->processEvents();

            ( (Page*)(stackedWidget->currentWidget()))->nextPhase();
            qApp->processEvents();

            progressDialog.setValue(progressDialog.value()+1);
            qApp->processEvents();
            if(progressDialog.wasCanceled())
                break ;
        }

        for (int f = 0 ; f < savedFilesPaths.count() ; f++)
        {
            QString new_path = dir.absoluteFilePath(QFileInfo(savedFilesPaths[f]).fileName()) ;
            QFile::remove(new_path);
            QFile::rename(savedFilesPaths[f],new_path);

        }

    }

}

void::MainWindow::organizeSequenceFiles(QStringList dirs_paths)
{
    for (int i = 0 ; i < savedFilesPaths.count() ; i++)
    {
        for (int j = 0 ; j < dirs_paths.count() ; j++)
            if (savedFilesPaths[i].startsWith(dirs_paths[j]))
            {
                QDir dir(dirs_paths[j]);
                QString new_path = dir.absoluteFilePath(QFileInfo(savedFilesPaths[i]).fileName()) ;
                QFile::remove(new_path);
                QFile::rename(savedFilesPaths[i],new_path);
            }

    }
}

bool MainWindow::isOnPicture(int e,double treshold)
{
    DoubleSidedEdge & dse = double_sided_edges[e] ;
    QVector<uint32_t> str = dse.truncated_str ;
    double total = 0 ;
    for (uint32_t i = 0 ; i < str.count() ; i++)
    {
        QPointF & point = skelVertices[str[i]];
//        if (settings.value("Contour/Invert").toBool())
//        {
//            if (openedImage->subPixel(point.x(),point.y())>treshold)
//                total ++;
//        }
//        else
//            if (openedImage->subPixel(point.x(),point.y())<treshold)
//                total++;

        QRgb pixel = openedQImage.pixel(point.x(),point.y()) ;
        int gray = qBlue(pixel);
        if (settings.value("Contour/Invert").toBool())
        {
            if (gray<127)
                total ++;
        }
        else
            if (gray>127)
                total++;
    }
    total/=str.count();

    const double ratio = .9 ;
    return (total>ratio);
}

void MainWindow::dynamicalAnalysis(){
    QStringList files = settings.value("FileList").toStringList() ;

    double_sided_edges_copy = double_sided_edges ;

    QProgressDialog progressDialog(tr("Traitement en cours..."),tr("Annuler le traitement"), 8, 8*files.count(), this);
    progressDialog.setWindowModality(Qt::WindowModal);

    QSet<QString> dirs_paths_set ;
    for (int i = 0 ; i < savedFilesPaths.count(); i++)
    {
        QString key = savedFilesPaths[i].left(savedFilesPaths[i].indexOf(files[files.count()-1])-1);
        dirs_paths_set.insert(key);
    }

    QDir dir(settings.value("IntroParamForm-File-2").toString());

    QStringList dirs_paths = dirs_paths_set.values() ;
    for (int i = 0 ; i < dirs_paths.count(); i++)
    {
        QDir new_dir(dirs_paths[i]);
        new_dir.removeRecursively();
        dir.mkdir(QFileInfo(dirs_paths[i]).fileName());
    }

    organizeSequenceFiles(dirs_paths);


    QSet<int> remaining_edges;
    double_sided_edges_birth.clear();
    for (int i = 0 ; i < double_sided_edges.count() ; i++)
    {
        double_sided_edges_birth << files.count() -1;

        remaining_edges.insert(i);
    }

    for (int i = 0 ; i < files.count() -1; i ++ )
    {
        savedFilesPaths.clear();
        settings.setValue("File",dir.absoluteFilePath(files[i]));
        initialImage.load(dir.absoluteFilePath(files[i]));
        for (int phase = 1 ; phase < 4 ; phase++)
        {
            setPhase(phase);
            while (! ( (Page*)(stackedWidget->currentWidget()))->initDone)
                qApp->processEvents();

            ( (Page*)(stackedWidget->currentWidget()))->nextPhase();
            qApp->processEvents();

            progressDialog.setValue(progressDialog.value()+1);
            qApp->processEvents();
            if(progressDialog.wasCanceled())
                break ;
        }

        double threshold = settings.value("ThresholdForm-Threshold").toInt() *.0001;
        threshold = threshold*(openedImage->getMax()-openedImage->getMin())+openedImage->getMin();


        QList<int> remainings = remaining_edges.values() ;
        for (int j = 0 ; j < remainings.count() ; j++)
            if (isOnPicture(remainings[i],threshold))
            {
                remaining_edges.remove(remainings[j]);
                double_sided_edges_birth[remainings[j]]=i;
            }

        //        for (int phase = 4 ; phase < 9 ; phase++)
        //        {
        //            setPhase(phase);
        //            while (! ( (Page*)(stackedWidget->currentWidget()))->initDone)
        //                qApp->processEvents();

        //            ( (Page*)(stackedWidget->currentWidget()))->nextPhase();
        //            qApp->processEvents();

        //            progressDialog.setValue(progressDialog.value()+1);
        //            qApp->processEvents();
        //            if(progressDialog.wasCanceled())
        //                break ;
        //        }

        organizeSequenceFiles(dirs_paths);

        if(progressDialog.wasCanceled())
            break ;

    }

    histoIntData << double_sided_edges_birth ;

    QVector<uint32_t> roads_birth,roads_death,roads_life_span;
    for (uint32_t i = 0 ; i < valid_roads.count() ; i++)
    {
        uint32_t min = INT_MAX ;
        uint32_t max = 0 ;
        for (int j = 0 ; j < roads_edges[valid_roads[i]].count() ; j++)
        {
            min = qMin(min,double_sided_edges_birth[roads_edges[valid_roads[i]][j]]);
            max = qMax(max,double_sided_edges_birth[roads_edges[valid_roads[i]][j]]);
        }
        roads_birth << min ;
        roads_death << max ;
        roads_life_span << max - min ;
    }

    histoIntData <<  roads_birth << roads_death << roads_life_span ;

    phase = 9 ;
}

void MainWindow::nextPhase()
{
    savePhase() ;

    if (phase>=0)
        ((Frame*)(stackedWidget->currentWidget()))->page->nextPhase();

    phase++ ;

    QWidget * widget = Phase::newFromPhase(phase,this);

    stackedWidget->addWidget(widget);
    stackedWidget->setCurrentIndex(phase);
    showMaximized();

    restorePhase();
}

void MainWindow::savePhase()
{
    QSettings settings;

    settings.setValue(QString("windowState%1").arg(phase), saveState());
    settings.setValue(QString("windowGeometry%1").arg(phase), saveGeometry());

}

void MainWindow::restorePhase()
{
    QSettings settings ;
    Frame * frame = (Frame*)stackedWidget->currentWidget() ;

    setPageTitle(frame->page->getTitle());
    back->setToolTip(frame->page->getPrevTooltip());
    next->setToolTip(frame->page->getNextTooltip());

    restoreState(settings.value(QString("windowState%1").arg(phase)).toByteArray());

    back->setEnabled(phase);

}

void MainWindow::prevPhase()
{
    savePhase();

    ((Page*)stackedWidget->currentWidget())->prevPhase();

    stackedWidget->removeWidget(stackedWidget->currentWidget());
    phase--;
    restorePhase() ;

    setActionsEnabled(true);
}

void MainWindow::setPageTitle(QString str)
{
    title->setText(str);
}

void MainWindow::receiveProgressIncrement(int inc)
{
    progress->setValue(progress->value()+inc);
}

void MainWindow::openBrowser(QUrl link)
{
    QTextBrowser * browser = new QTextBrowser;
    //browser->setUrl(link);
    browser->show() ;
}




void MainWindow::on_actionAide_2_triggered()
{
//    if (!docWidget)
//    {
//        docWidget = new QDockWidget(tr("Aide"),this);
//        docWidget->setWidget(((Page*)stackedWidget->currentWidget())->getDocForm());

//        addDockWidget(Qt::LeftDockWidgetArea,docWidget);
//    }

//    docWidget->show();
}

void MainWindow::on_actionParam_tres_triggered()
{
//    if (!paramWidget)
//    {
//        paramWidget = new QDockWidget(tr("Paramètres"),this);
//        paramWidget->setWidget(((Page*)stackedWidget->currentWidget())->getParamForm());

//        addDockWidget(Qt::BottomDockWidgetArea,paramWidget);
//    }

//    paramWidget->show();
}



void MainWindow::setActionsEnabled(bool enabled)
{
    back->setEnabled(enabled?(phase>0):false) ;
    next->setEnabled(enabled);
//    Page * page = ((Frame*)stackedWidget->currentWidget())->page;
//    page->getParamForm()->setEnabled(enabled);

    repaint();
}

void MainWindow::appendToSavedFiles(QString str)
{
    savedFilesPaths << str ;
    statusBar()->showMessage(str+tr(" enregistré."),2000);
}

void MainWindow::trySaveImage(const QString &pre, const QImage &image)
{
    QFileInfo file(settings.value("File").toString());

    QString fileName = tr("%1/%2%3.jpg").arg(file.absoluteDir().absolutePath()).arg(pre).arg(file.baseName());
    if (image.save(fileName))
    {
        appendToSavedFiles(tr("%1").arg(fileName));
        htmlLog.append(tr("Vous avez enregistré : %1<BR>").arg(fileName));
    }
    else
        htmlLog.append(tr("<FONT COLOR=\"RED\">%1 n'a pas été enregistrée.</FONT><BR>").arg(fileName));

}

void MainWindow::trySaveDoubleImage( QString pre,  DoubleImage *image)
{
    QImage i(image->width(),image->height(),QImage::Format_ARGB32);
    image->toQImage(i);
    trySaveImage(pre, i);

}
