#include "introdocform.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel = new QLabel(tr("Application chargée"),this);
    progress = new QProgressBar(this);

    statusBar()->addWidget(statusLabel);
    statusBar()->addPermanentWidget(progress);


    stackedWidget = new QStackedWidget(this) ;
    setCentralWidget(stackedWidget);

    QToolBar * toolbar = new QToolBar(this) ;
    toolbar->setObjectName(tr("Navigation"));
    addToolBar(toolbar);
    toolbar->setIconSize(QSize(96,96));

    QIcon icon_next(QString(":/next640.png")) ;
    QIcon icon_back(QString(":/prev640.png")) ;

    action_back = toolbar->addAction(icon_back,QString("BACK") );
    connect(action_back,SIGNAL(triggered(bool)),this,SLOT(prevPhase()));


    title = new QLabel("PHASE 0 : INTRODUCTION",this) ;
    QFont font = title->font() ;
    font.setPixelSize(72);
    title->setFont(font) ;
    toolbar->addWidget(title );

    action_next = toolbar->addAction(icon_next,QString("NEXT") );
    connect(action_next,SIGNAL(triggered(bool)),this,SLOT(nextPhase()));

    docWidget = new QDockWidget(tr("Aide"),this);
    docWidget->setObjectName(tr("Aide"));

    addDockWidget(Qt::TopDockWidgetArea,docWidget);

    paramWidget = new QDockWidget(tr("Paramètres"),this);
    paramWidget->setObjectName(tr("Paramètres"));

    addDockWidget(Qt::BottomDockWidgetArea,paramWidget);


    regularizedImage = NULL ;openedImage = NULL ;
    texture =  NULL ;

    kdtree = NULL ;

    //    QSettings settings;
    //    QVariant variant = settings.value("geometry") ;
    //    if (variant!=QVariant())
    //        restoreGeometry(variant.toByteArray());

    phase = -1 ;

    scale =  1 ;

    nextPhase();
}

MainWindow::~MainWindow()
{
    QSettings settings ;
    settings.setValue(QString("geometry"), saveGeometry());

    delete ui;
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

void MainWindow::repeatPhases(){
    QStringList files = settings.value("FileList").toStringList() ;

    QProgressDialog progressDialog(tr("Traitement en cours..."),tr("Annuler le traitement"), 8, 8*(files.count()-1), this);
    progressDialog.setWindowModality(Qt::WindowModal);
    QDir dir(settings.value("IntroParamForm-File-1").toString());
    QString new_dir_path = tr("Dossier %1").arg(QFileInfo(files[0]).baseName());
    QDir new_dir(dir.absoluteFilePath(new_dir_path));
    new_dir.removeRecursively();
    dir.mkdir(new_dir_path);
    dir.cd(new_dir_path);
    for (int f = 0 ; f < logStrings.count() ; f++)
       {
        QString new_path = dir.absoluteFilePath(QFileInfo(logStrings[f]).fileName()) ;
        QFile::remove(new_path);
        QFile::rename(logStrings[f],new_path);
    }

    for (int i = 1 ; i < files.count() ; i ++ )
    {
        logStrings.clear();
        QDir dir(settings.value("IntroParamForm-File-1").toString());
        settings.setValue("File",dir.absoluteFilePath(files[i]));
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

        for (int f = 0 ; f < logStrings.count() ; f++)
        {
            QString new_path = dir.absoluteFilePath(QFileInfo(logStrings[f]).fileName()) ;
            QFile::remove(new_path);
            QFile::rename(logStrings[f],new_path);

        }

    }

}

void MainWindow::nextPhase()
{
    savePhase() ;

    if (phase>=0)
        ((Page*)(stackedWidget->currentWidget()))->nextPhase();

    action_next->setEnabled(false);

    phase++ ;

    QWidget * widget = Phase::newFromPhase(phase,this);
    stackedWidget->addWidget(widget);
    stackedWidget->setCurrentIndex(phase);
    //showMaximized();

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
    Page * page = (Page*)stackedWidget->currentWidget() ;

    setPageTitle(page->getTitle());
    action_back->setToolTip(page->getPrevTooltip());
    action_next->setToolTip(page->getNextTooltip());

    restoreState(settings.value(QString("windowState%1").arg(phase)).toByteArray());

    docWidget->setWidget(page->getDocForm());
    docWidget->setMinimumSize(QSize(320,240));
     restoreDockWidget(docWidget);
    paramWidget->setWidget(page->getParamForm());
    restoreDockWidget(paramWidget);

    action_back->setEnabled(phase);

    showFullScreen();


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
    browser->setUrl(link);
    browser->show() ;
}

void MainWindow::on_action1_Introduction_triggered()
{
    QList<QAction*> list = ui->menuPhase->actions();
    for (int i = 1 ; i < list.count() ; i++)
    {
        list[i]->setChecked(false);
    }
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionStatistiques_triggered()
{
    stackedWidget->setCurrentIndex(1);
}



void MainWindow::on_actionAide_2_triggered()
{
    if (!docWidget)
    {
        docWidget = new QDockWidget(tr("Aide"),this);
        docWidget->setWidget(((Page*)stackedWidget->currentWidget())->getDocForm());

        addDockWidget(Qt::LeftDockWidgetArea,docWidget);
    }

    docWidget->show();
}

void MainWindow::on_actionParam_tres_triggered()
{
    if (!paramWidget)
    {
        paramWidget = new QDockWidget(tr("Paramètres"),this);
        paramWidget->setWidget(((Page*)stackedWidget->currentWidget())->getParamForm());

        addDockWidget(Qt::BottomDockWidgetArea,paramWidget);
    }

    paramWidget->show();
}

void MainWindow::setActionsEnabled(bool enabled)
{
    action_back->setEnabled(enabled?(phase>0):false) ;
    action_next->setEnabled(enabled);
    Page * page = (Page*)stackedWidget->currentWidget() ;
    page->getParamForm()->setEnabled(enabled);

    repaint();
}

void MainWindow::log(QString str)
{
    logStrings << str ;
    statusBar()->findChild<QLabel*>()->setText(str);
}

void MainWindow::trySaveImage(const QString &pre, const QImage &image)
{
    QFileInfo file(settings.value("File").toString());

    QString fileName = tr("%1/%2%3").arg(file.absoluteDir().absolutePath()).arg(pre).arg(file.fileName());
    if (image.save(fileName))
        log(tr("%1").arg(fileName));
    else
        log(tr("<FONT COLOR=\"RED\">%1 n'a pas été enregistrée.</FONT>").arg(fileName));

}

void MainWindow::trySaveDoubleImage( QString pre,  DoubleImage *image)
{
    QImage i(image->width(),image->height(),QImage::Format_ARGB32);
    image->toQImage(i);
    trySaveImage(pre, i);

}
