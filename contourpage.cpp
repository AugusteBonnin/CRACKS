#include "contourdocform.h"
#include "contourpage.h"
#include "contourparamform.h"
#include "mainwindow.h"
#include "ui_contourform.h"
#include "shapefil.h"

#include <QDir>
#include <QSettings>
#include <QTimer>

void ContourPage::saveSVG()
{
    QFileInfo file(settings.value("File").toString());

    QFile data(tr("%1/Contour-%2.SVG").arg(file.absoluteDir().absolutePath()).arg(file.fileName()));
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

}

void ContourPage::saveSHP()
{
    QFileInfo file(settings.value("File").toString());

    QString path(tr("%1/Contour-%2.SHP").arg(file.absoluteDir().absolutePath()).arg(file.fileName()));

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

}

ContourPage::ContourPage(MainWindow *parent) :
    Page(parent)
{
    docForm = new ContourDocForm(parent);
    paramForm = new ContourParamForm(parent,this);

    originalImage = mainWindow->openedImage ;
    originalQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32);
    originalImage->toQImage(originalQImage);

    mainWindow->openedQImage = originalQImage ;

    widget = new ContourWidget(this) ;

    QVBoxLayout * layout = new QVBoxLayout ;
    layout->addWidget(widget);
    setLayout(layout);

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

}

void ContourPage::setImages()
{
    //widget->resetImage(originalQImage);

    if(settings.value("Contour/Invert",false).toBool())
        emit invertSignal() ;

    emit kMeansSignal() ;

    updateThreshold(settings.value("ThresholdForm-Threshold",QVariant(5000)).toInt()) ;
}

ContourPage::~ContourPage()
{
}

void ContourPage::nextPhase()
{
    QFileInfo file(settings.value("File").toString());

    if (settings.value("Contour/SaveJPG",false).toBool())
    {
        QImage image;
        if (settings.value("Contour/Screenshot",true).toBool())
        {
            image = widget->getScreenshot() ;
        }
        else //Full image
        {
            image = widget->getImage();
        }

        image.save(tr("%1/Contour-%2").arg(file.absoluteDir().absolutePath()).arg(file.fileName()));
    }
    if (settings.value("Contour/SaveSVG",false).toBool())
        saveSVG();
    if (settings.value("Contour/SaveSHP",false).toBool())
        saveSHP();
}

void ContourPage::reinit()
{
    originalImage = mainWindow->openedImage ;
    originalQImage = QImage(originalImage->width(),originalImage->height(),QImage::Format_ARGB32);
    originalImage->toQImage(originalQImage);

    mainWindow->openedQImage = originalQImage ;

    setImages() ;

}



