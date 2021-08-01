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

    QTimer::singleShot(0,this,SLOT(on_pushButton_2_clicked()));
}

RoadsPage::~RoadsPage()
{
    delete docForm;
    delete paramForm;

}

void RoadsPage::nextPhase()
{

        QImage image = widget->getImage();

        mainWindow->trySaveImage(tr("Voies et places-"),image);

        saveSVG();
        saveSHP();
        saveCSV();
        widget->saveDistanceMatrixCSV();
}

void RoadsPage::reinit()
{
    on_pushButton_2_clicked() ;
}

void RoadsPage::on_pushButton_2_clicked()
{
    mainWindow->setActionsEnabled(false);
    paramForm->setEnabled(false);
    widget->buildRoads(settings.value("RoadsParamForm-ScaleFactor",QVariant(1.0)).toDouble(),
                       settings.value("RoadsParamForm-MaxAngle",QVariant(45.0)).toDouble());
    paramForm->setEnabled(true);
    mainWindow->setActionsEnabled(true);

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
            QVector<QVector<QPointF> >  & places = mainWindow->places_contours_line_strings ;
            //places
            for (int i = 0 ; i < places.count() ; i++)
            {
                mainWindow->progress->setValue(i);
                out << "<polyline points=\"" ;
                for (int j = 0 ; j < places[i].count() - 1 ; j++)
                {
                    QPointF point = places[i][j] ;
                    out << point.x() << " " << point.y() << "," ;
                }
                //repeat first point
                QPointF point = places[i][places[i].count() - 1] ;
                 out << point.x() << " " << point.y() ;

                out << "\" stroke=\"black\" fill=\"white\" stroke-width=\"1\"/>\n" ;
            }
            mainWindow->progress->setMaximum(mainWindow->valid_roads.count()-1);

            //roads
            for (int i = 0 ; i < mainWindow->valid_roads.count() ; i++)
            {
                mainWindow->progress->setValue(i);
                out << "<polyline points=\"" ;
                for (int j = 0 ; j < mainWindow->roads_line_strings[i].count() - 1 ; j++)
                {
                    QPointF point = mainWindow->roads_line_strings[i][j] ;
                    out << point.x() << " " << point.y() << "," ;
                }
                QPointF point = mainWindow->roads_line_strings[i]
                        [mainWindow->roads_line_strings[i].count() - 1] ;
                 out << point.x() << " " << point.y() ;

                out << "\" stroke=\""+mainWindow->roads_line_strings_colors[i].name()+
                        "\" fill=\"none\" stroke-width=\"1\"/>\n" ;
            }

            out << "</svg>\n" ;
        }
        data.close() ;
        mainWindow->appendToSavedFiles(tr("%1").arg(path));
        qDebug() << "fin save svg" ;

}

void RoadsPage::saveCSV()
{
    QFileInfo file(settings.value("File").toString()) ;

    //roads
    QString path = tr("%1/Voies-%2.csv").arg(file.absoluteDir().absolutePath()).arg(file.baseName());

    QFile data(path);
      if (data.open(QFile::WriteOnly)) {
          QTextStream out(&data);
          out << tr("ACCESSIBILITE") << ";" ;
          out << tr("ORTHOGONALITE") << ";" ;
          out << tr("ESPACEMENT") << ";" ;
          out << tr("LONGUEUR") << ";" ;
          out << tr("DEGRE") << ";" ;
          out << tr("LONGUEUR_TOPO") << "\n" ;

          for (int i = 0 ; i < mainWindow->valid_roads.count() ; i++)
          {
              for (int j = 0 ; j < 4 ; j++)
                  out << mainWindow->histoDoubleData[j+1][i] << ";" ;

              out << mainWindow->histoIntData[3][i] << ";" ;
              out << mainWindow->histoIntData[4][i] << "\n" ;

          }

          data.close();
      }

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

            for (int j = 0 ; j < mainWindow->roads_line_strings[i].count() ; j++)
            {
                QPointF &point = mainWindow->roads_line_strings[i][j];
                padfX.append(point.x()*scale);
                padfY.append(point.y()*scale);
            }


            SHPObject * object =

                        SHPCreateSimpleObject( SHPT_ARC, mainWindow->roads_line_strings[i].count(),
                                               padfX.data(), padfY.data() ,NULL );

            int entity = SHPWriteObject( shapeFile, -1 /*for newly created shape*/, object );
            for (int j = 0 ; j < 4 ; j++)
                DBFWriteDoubleAttribute( dbfFile,entity,fieldNumbers[j],
                                         mainWindow->histoDoubleData[j+1][i]);

            DBFWriteIntegerAttribute (dbfFile,entity,fieldNumbers[4],
                                     mainWindow->histoIntData[3][i]);
            DBFWriteIntegerAttribute (dbfFile,entity,fieldNumbers[5],
                                     mainWindow->histoIntData[4][i]);

            SHPDestroyObject(object);

        }
        SHPClose( shapeFile );
        DBFClose(dbfFile);

        path = tr("%1/Places-%2.SHP").arg(file.absoluteDir().absolutePath()).arg(file.baseName());
        shapeFile = SHPCreate( path.toStdString().c_str(), SHPT_POLYGON );
        dbfFile = DBFCreate(path.toStdString().c_str()) ;
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

                        SHPCreateSimpleObject( SHPT_POLYGON, mainWindow->places_contours_line_strings[i].count(),
                                               padfX.data(), padfY.data() ,NULL );

            int entity = SHPWriteObject( shapeFile, -1 /*for newly created shape*/, object );

            DBFWriteIntegerAttribute (dbfFile,entity,fieldNumber,
                                     mainWindow->histoIntData[0][i]);


            SHPDestroyObject(object);

        }

        SHPClose( shapeFile );
        DBFClose(dbfFile);

        mainWindow->appendToSavedFiles(path);
mainWindow->appendToSavedFiles(path.replace(".shp",".shx"));
mainWindow->appendToSavedFiles(path.replace(".shx",".dbf"));

     }
