#include "openglscalewidget.h"
#include "roadspage.h"
#include "roadsparamform.h"
#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QTimer>
#include "mainwindow.h"
#include <QAction>
#include "shapefil.h"

RoadsPage::RoadsPage(MainWindow *parent) :
    Page(parent)
{ 
    docForm = new DocForm(parent,tr("qrc:/docs/aide.html#Voies"));
    paramForm = new RoadsParamForm(parent,this);

    QVBoxLayout * layout = new QVBoxLayout ;

    widget = new RoadsWidget(this);
    layout->addWidget(widget);

    OpenGLScaleWidget * sw = new OpenGLScaleWidget(this,mainWindow->scale,mainWindow->openedQImage.size());
    sw->setFixedHeight(40);
    layout->addWidget(sw);
    sw->setVisible(settings.value("Crop/Unit",false).toBool());

    setLayout(layout);

    connect(widget,SIGNAL(ScaleChanged(double)),sw,SLOT(ScaleChanged(double)));

    QTimer::singleShot(100,this,SLOT(on_pushButton_2_clicked()));
}

RoadsPage::~RoadsPage()
{
}

void RoadsPage::nextPhase()
{
    if (settings.value("Roads/SaveJPG",true).toBool())
    {
        QImage image;
        if (settings.value("Roads/Screenshot").toBool())
        {
            screenshot(image) ;
        }
        else //Full image
        {
            image = widget->getImage();
        }
        mainWindow->trySaveImage(tr("Voies et places-"),image);

    }
    if (settings.value("Roads/SaveSVG",false).toBool())
        saveSVG();
    if (settings.value("Roads/SaveSHP",false).toBool())
        saveSHP();
}

void RoadsPage::reinit()
{
    on_pushButton_2_clicked() ;
}

void RoadsPage::on_pushButton_2_clicked()
{
    //mainWindow->action_next->setEnabled(false);
    paramForm->setEnabled(false);
    widget->buildRoads(settings.value("RoadsParamForm-ScaleFactor",QVariant(1.0)).toDouble(),
                       settings.value("RoadsParamForm-MaxAngle",QVariant(45.0)).toDouble());
    paramForm->setEnabled(true);
    //mainWindow->action_next->setEnabled(true);

}


void RoadsPage::saveSVG()
{
    qDebug() << "debut save svg" ;
    QFileInfo file(settings.value("File").toString()) ;
    QString path = tr("%1/Voies-%2.svg").arg(file.dir().absolutePath()).arg(file.baseName());
        QFile data(path);
        if (data.open(QFile::WriteOnly)) {
            QTextStream out(&data);
            out << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">\n" ;
            mainWindow->progress->setMaximum(mainWindow->places_contours_line_strings.count()-1);

            //places
            for (int i = 0 ; i < mainWindow->places_contours_line_strings.count() ; i++)
            {
                mainWindow->progress->setValue(i);
                out << "<polyline points=\"" ;
                for (int j = 0 ; j < mainWindow->places_contours_line_strings[i].count() - 1 ; j++)
                {
                    QPointF point = mainWindow->places_contours_line_strings[i][j] ;
                    out << point.x() << " " << point.y() << "," ;
                }
                //repeat first point
                QPointF point = mainWindow->places_contours_line_strings[i][0] ;
                 out << point.x() << " " << point.y() ;

                out << "\" stroke=\"black\" fill=\"white\" stroke-width=\"1\"/>\n" ;
            }
            mainWindow->progress->setMaximum(mainWindow->valid_roads.count()-1);

            //roads
            for (int i = 0 ; i < mainWindow->valid_roads.count() ; i++)
            {
                mainWindow->progress->setValue(i);
                out << "<polyline points=\"" ;
                for (int j = 0 ; j < mainWindow->roads_line_strings[mainWindow->valid_roads[i]].count() - 1 ; j++)
                {
                    QPointF point = mainWindow->roads_line_strings[mainWindow->valid_roads[i]][j] ;
                    out << point.x() << " " << point.y() << "," ;
                }
                QPointF point = mainWindow->roads_line_strings[mainWindow->valid_roads[i]]
                        [mainWindow->roads_line_strings[mainWindow->valid_roads[i]].count() - 1] ;
                 out << point.x() << " " << point.y() ;

                out << "\" stroke=\""+mainWindow->roads_line_strings_colors[mainWindow->valid_roads[i]].name()+
                        "\" fill=\"none\" stroke-width=\"1\"/>\n" ;
            }

            out << "</svg>\n" ;
        }
        data.close() ;
        mainWindow->log(tr("%1").arg(path));
        qDebug() << "fin save svg" ;

}


void RoadsPage::saveSHP()
{
    double scale = 1.0 ;

    QFileInfo file(settings.value("File").toString()) ;

    //roads
    QString path = tr("%1/Voies-%2.shp").arg(file.absoluteDir().absolutePath()).arg(file.baseName());



        const char * names[]={tr("ACCESSIBILITE").toStdString().c_str(),
                            tr("ORTHOGONALITE").toStdString().c_str(),
                             tr("ESPACEMENT").toStdString().c_str(),
                             tr("LONGUEUR").toStdString().c_str(),
                            tr("DEGRE").toStdString().c_str(),
                            tr("LONGUEUR_TOPO").toStdString().c_str()};
        int fieldNumbers[6] ;

        SHPHandle shapeFile = SHPCreate( path.toStdString().c_str(), SHPT_ARC );
        DBFHandle dbfFile = DBFCreate(path.toStdString().c_str()) ;

        for(int i = 0 ; i < 4;i++)
            fieldNumbers[i] = DBFAddField( dbfFile, names[i],
                         FTDouble, 13, 6 );
        fieldNumbers[4] = DBFAddField( dbfFile, names[4],
                          FTInteger, 3, 0 );
        fieldNumbers[5] = DBFAddField( dbfFile, names[5],
                          FTInteger, 4, 0 );

        for (int i = 0 ; i < mainWindow->valid_roads.count() ; i++)
        {

            QVector<double> padfX,padfY;

            for (int j = 0 ; j < mainWindow->roads_line_strings[mainWindow->valid_roads[i]].count() ; j++)
            {
                QPointF &point = mainWindow->roads_line_strings[mainWindow->valid_roads[i]][j];
                padfX.append(point.x()*scale);
                padfY.append(point.y()*scale);
            }


            SHPObject * object =

                        SHPCreateSimpleObject( SHPT_ARC, mainWindow->roads_line_strings[mainWindow->valid_roads[i]].count(),
                                               padfX.data(), padfY.data() ,NULL );

            int entity = SHPWriteObject( shapeFile, -1 /*for newly created shape*/, object );
            for (int j = 0 ; j < mainWindow->histoDoubleData.count()-1 ; j++)
                DBFWriteDoubleAttribute( dbfFile,entity,fieldNumbers[j],
                                         mainWindow->histoDoubleData[j+1][i]);

            DBFWriteIntegerAttribute (dbfFile,entity,fieldNumbers[4],
                                     mainWindow->histoIntData[1][i]);
            DBFWriteIntegerAttribute (dbfFile,entity,fieldNumbers[5],
                                     mainWindow->histoIntData[2][i]);

            SHPDestroyObject(object);

        }

        int fieldNumber = DBFAddField( dbfFile,"DEGRE",FTInteger, 3, 0 );

        for (int i = 0 ; i < mainWindow->places_contours_line_strings.count() ; i++)
        {

            QVector<double> padfX,padfY;

            for (int j = 0 ; j < mainWindow->places_contours_line_strings[i].count() ; j++)
            {
                QPointF &point = mainWindow->places_contours_line_strings[i][j];
                padfX.append(point.x());
                padfY.append(point.y());
            }


            SHPObject * object =

                        SHPCreateSimpleObject( SHPT_ARC, mainWindow->places_contours_line_strings[i].count(),
                                               padfX.data(), padfY.data() ,NULL );

            int entity = SHPWriteObject( shapeFile, -1 /*for newly created shape*/, object );

            DBFWriteIntegerAttribute (dbfFile,entity,fieldNumber,
                                     mainWindow->histoIntData[0][i]);


            SHPDestroyObject(object);

        }

        SHPClose( shapeFile );
        DBFClose(dbfFile);

        mainWindow->log(path);
mainWindow->log(path.replace(".shp",".shx"));
mainWindow->log(path.replace(".shx",".dbf"));

     }
