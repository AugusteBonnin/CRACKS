#include "mainwindow.h"
#include "skeldocform.h"
#include "skelpage.h"
#include "skelparamform.h"
#include "ui_skelform.h"
#include "shapefil.h"
#include "openglscalewidget.h"
#include <QDir>
#include <QFileInfo>
#include <QTimer>

void SkelPage::saveSVG()
{
    QFileInfo file(settings.value("File").toString());
    QString path = tr("%1/Squelettisation-%2.svg").arg(file.absoluteDir().absolutePath()).arg(file.fileName());
    QFile data(path);
    if (data.open(QFile::WriteOnly)) {
        QTextStream out(&data);
        out << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">\n" ;

        for (int i = 0 ; i < mainWindow->getDoubleSidedEdges().count() ; i++)
        {
            out << "<polyline points=\"" ;
            for (int j = 0 ; j < mainWindow->getDoubleSidedEdges()[i].str.count() - 1 ; j++)
            {
                QPointF point = mainWindow->getSkelVertices()[mainWindow->getDoubleSidedEdges()[i].str[j]] ;
                out << point.x() << " " << point.y() << "," ;
            }
            QPointF point = mainWindow->getSkelVertices()[mainWindow->getDoubleSidedEdges()[i].str[mainWindow->getDoubleSidedEdges()[i].str.count() - 1]] ;
            out << point.x() << " " << point.y() ;

            out << "\" stroke=\"black\" fill=\"none\" stroke-width=\"1\"/>\n" ;
        }

        out << "</svg>\n" ;
    }
    data.close() ;
    mainWindow->log(tr("%1").arg(path));

}

void SkelPage::saveSHP()
{
    QFileInfo file(settings.value("File").toString());
    QString path = tr("%1/Squelettisation-%2.shp").arg(file.absoluteDir().absolutePath()).arg(file.fileName());
    SHPHandle shapeFile = SHPCreate( path.toStdString().c_str(), SHPT_ARC );
    DBFHandle dbfFile = DBFCreate(path.toStdString().c_str()) ;

    int fieldNumber = DBFAddField( dbfFile, "HALF_WIDTH",
                     FTDouble, 7, 3 );


    for (int i = 0 ; i < mainWindow->getDoubleSidedEdges().count() ; i++)
    {
        QVector<double> padfX,padfY;

        for (int j = 0 ; j < mainWindow->getDoubleSidedEdges()[i].str.count() ; j++)
        {
            QPointF point = mainWindow->getSkelVertices()[mainWindow->getDoubleSidedEdges()[i].str[j]];
            padfX.append(point.x());
            padfY.append(point.y());
        }


        SHPObject * object =

                    SHPCreateSimpleObject( SHPT_ARC, mainWindow->getDoubleSidedEdges()[i].str.count(),
                                           padfX.data(), padfY.data() ,NULL );

        int entity = SHPWriteObject( shapeFile, -1 /*for newly created shape*/, object );

        DBFWriteDoubleAttribute( dbfFile,entity,fieldNumber,
                                     mainWindow->getDoubleSidedEdges()[i].mean_distance );


        SHPDestroyObject(object);

    }
    SHPClose( shapeFile );
    DBFClose(dbfFile);

    mainWindow->log(path);
    mainWindow->log(path.replace(".shp",".shx"));
    mainWindow->log(path.replace(".shx",".dbf"));

}

SkelPage::SkelPage(MainWindow *parent) :
    Page(parent)
{
    docForm = new SkelDocForm(parent);
    paramForm = new SkelParamForm(parent,this);

QVBoxLayout * layout = new QVBoxLayout;
    widget = new SkelWidget(this) ;

    layout->addWidget(widget);
    OpenGLScaleWidget * sw = new OpenGLScaleWidget(this,mainWindow->scale,mainWindow->openedQImage.size());
    sw->setFixedHeight(40);
    layout->addWidget(sw);
    setLayout(layout);
    sw->setVisible(settings.value("Crop/Unit",false).toBool());

    connect(widget,SIGNAL(ScaleChanged(double)),sw,SLOT(ScaleChanged(double)));

    QTimer::singleShot(200,widget,SLOT(buildSkel()));

   initDone = false ;

    repaint() ;
}

SkelPage::~SkelPage()
{
}

void SkelPage::reinit()
{
    initDone = false ;
    widget->buildSkel();

}

void SkelPage::nextPhase()
{
    if (settings.value("Skel/SaveJPG",false).toBool())
    {
        QImage image;
        if (settings.value("Skel/Screenshot").toBool())
        {
            screenshot(image) ;
        }
        else //Full image
        {
            image = widget->getImage();
        }

        mainWindow->trySaveImage(tr("Squelettisation-"),image);

    }
    if (settings.value("Skel/SaveSVG",false).toBool())
        saveSVG();
    if (settings.value("Skel/SaveSHP",false).toBool())
        saveSHP();
}

void SkelPage::prevPhase()
{
    mainWindow->getSkelChildren().clear();
    mainWindow->getSkelColors().clear();
mainWindow->getSkelDistanceToBoundary().clear();
mainWindow->getSkelIndices().clear();
mainWindow->getSkelVertices().clear();
}
