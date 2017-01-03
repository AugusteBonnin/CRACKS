#include "contourdocform.h"
#include "contourpage.h"
#include "contourparamform.h"
#include "mainwindow.h"
#include "ui_contourform.h"
#include "shapefil.h"
#include "openglscalewidget.h"

#include <QDir>
#include <QSettings>
#include <QTimer>

void ContourPage::saveSVG()
{
    QFileInfo file(settings.value("File").toString());
QString path = tr("%1/Contour-%2.svg").arg(file.absoluteDir().absolutePath()).arg(file.fileName()) ;
    QFile data(path);
    if (data.open(QFile::WriteOnly)) {
        QPointF point;
        QTextStream out(&data);
        out << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">\n" ;

        for (int i = 0 ; i < mainWindow->getConnectedComponentsStarts().count() ; i++)
        {
            out << "<polyline points=\"" ;
            int j = mainWindow->getConnectedComponentsStarts()[i] ;
            do
            {
                point = mainWindow->getContourVertices()[j] ;
                out << point.x() << " " << point.y() << "," ;
                j = mainWindow->getNextPointIndex()[j] ;
            } while (j!=mainWindow->getConnectedComponentsStarts()[i]) ;

            point = mainWindow->getContourVertices()[j] ;
            out << point.x() << " " << point.y();

            out << "\" stroke=\"black\" fill=\"none\" stroke-width=\"1\"/>\n" ;
        }

        out << "</svg>\n" ;
    }
    data.close() ;
mainWindow->log(tr("%1").arg(path));
}

void ContourPage::saveSHP()
{
    QFileInfo file(settings.value("File").toString());

    QString path(tr("%1/Contour-%2.shp").arg(file.absoluteDir().absolutePath()).arg(file.fileName()));

    SHPHandle shapeFile = SHPCreate( path.toStdString().c_str(), SHPT_ARC );

    for (int i = 0 ; i < mainWindow->getConnectedComponentsStarts().count() ; i++)
    {
        QVector<double> padfX,padfY;
QPointF point ;
        int j = mainWindow->getConnectedComponentsStarts()[i] ;
        do
        {
            point = mainWindow->getContourVertices()[j] ;
            padfX.append(point.x());
            padfY.append(point.y());
            j = mainWindow->getNextPointIndex()[j] ;
        }while (j!=mainWindow->getConnectedComponentsStarts()[i]) ;

        point = mainWindow->getContourVertices()[j] ;
        padfX.append(point.x());
        padfY.append(point.y());



        SHPObject * object =

                    SHPCreateSimpleObject( SHPT_ARC, padfX.count(),
                                           padfX.data(), padfY.data() ,NULL );

        SHPWriteObject( shapeFile, -1 /*for newly created shape*/, object );

        SHPDestroyObject(object);

    }
    SHPClose( shapeFile );

    mainWindow->log(path);
    mainWindow->log(path.replace(".shp",".shx"));

}

ContourPage::ContourPage(MainWindow *parent) :
    Page(parent)
{

    originalImage = new DoubleImage(*(mainWindow->openedImage)) ;
    originalImage->computeMinMax();
    originalQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32);
    originalImage->toQImage(originalQImage);

    mainWindow->openedQImage = originalQImage ;

    widget = new ContourWidget(this) ;
widget->setFixedHeight(512);

    QVBoxLayout * layout = new QVBoxLayout ;
    layout->addWidget(widget);
    OpenGLScaleWidget * scale = new OpenGLScaleWidget(this,mainWindow->scale,mainWindow->openedQImage.size());
    scale->setFixedHeight(40);
    layout->addWidget(scale);
    scale->setVisible(settings.value("Crop/Unit",false).toBool());
    setLayout(layout);

    connect(widget,SIGNAL(ScaleChanged(double)),scale,SLOT(ScaleChanged(double)));

    docForm = new ContourDocForm(parent);
    paramForm = new ContourParamForm(parent,this);

    QTimer::singleShot(0,this,SLOT(setImages())) ;
}

void ContourPage::updateThreshold(int value)
{
    mainWindow->action_next->setEnabled(false);
    paramForm->setEnabled(false);

    double threshold = value*.0001;
    widget->updateThreshold(threshold*(originalImage->getMax()-originalImage->getMin())+originalImage->getMin());

    ((ContourParamForm*)paramForm)->ui->label_3->setText(tr("Nombre d'ilots distincts : %1").arg(getMainWindow()->getConnectedComponentsStarts().count()));

    paramForm->setEnabled(true);

    mainWindow->action_next->setEnabled(true);

    initDone = true ;

}

void ContourPage::setImages()
{
    //widget->resetImage(originalQImage);

    if(settings.value("Contour/Invert",false).toBool())
        emit invertSignal() ;

    if(settings.value("Contour/KMeans",false).toBool())
        emit kMeansSignal() ;

    updateThreshold(settings.value("ThresholdForm-Threshold",QVariant(5000)).toInt()) ;
}

ContourPage::~ContourPage()
{
}

void ContourPage::nextPhase()
{

    if (settings.value("Contour/SaveJPG",false).toBool())
    {
        QImage image;
        if (settings.value("Contour/Screenshot",true).toBool())
        {
            screenshot(image) ;
        }
        else //Full image
        {
            image = widget->getImage();
        }

        mainWindow->trySaveImage(tr("Contour-"),image);
    }
    if (settings.value("Contour/SaveSVG",false).toBool())
        saveSVG();
    if (settings.value("Contour/SaveSHP",false).toBool())
        saveSHP();
}

void ContourPage::prevPhase()
{
    mainWindow->getContourIndices().clear();
    mainWindow->getContourVertices().clear();

}

void ContourPage::reinit()
{
    mainWindow->getContourIndices().clear();
    mainWindow->getContourVertices().clear();

    originalImage = mainWindow->openedImage ;
    originalQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32);
    originalImage->toQImage(originalQImage);

    mainWindow->openedQImage = originalQImage ;

    setImages() ;

}



