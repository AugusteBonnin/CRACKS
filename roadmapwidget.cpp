#include "roadmapwidget.h"
#include "mainwindow.h"

RoadMapWidget::RoadMapWidget(MapPage *parent, QVector<QColor> & colors) : MapWidget(parent) , colors(colors)
{

    QVector<float>  & pre_roads_vbo = mainWindow->pre_roads_vbo ;

    int i = 0 ;
    while (i < mainWindow->valid_roads.count())
    {

        for (int k = mainWindow->pre_valid_roads_index_vbo_start[i];
             k <  mainWindow->pre_valid_roads_index_vbo_end[i]
             ; k++)
        {
            pre_roads_vbo[mainWindow->pre_valid_roads_index_vbo[k]*6+2] = colors[i].redF() ;
            pre_roads_vbo[mainWindow->pre_valid_roads_index_vbo[k]*6+3] = colors[i].greenF() ;
            pre_roads_vbo[mainWindow->pre_valid_roads_index_vbo[k]*6+4] = colors[i].blueF() ;
        }
        i++ ;
    }

    valid_roads_vbo.create();
    valid_roads_vbo.bind();
    valid_roads_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    valid_roads_vbo.allocate(pre_roads_vbo.constData(),pre_roads_vbo.count()*sizeof(float));

    valid_roads_index_vbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    valid_roads_index_vbo->create();
    valid_roads_index_vbo->bind();
    valid_roads_index_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    valid_roads_index_vbo->allocate(mainWindow->pre_valid_roads_index_vbo.constData(),
                                    mainWindow->pre_valid_roads_index_vbo.count()*sizeof(unsigned int));
}

void RoadMapWidget::saveSVG(QString path)
{
    MainWindow * mainWindow = ((MapPage*)parent())->getMainWindow() ;
    QFile data(path);
        if (data.open(QFile::WriteOnly)) {
            QTextStream out(&data);
            out << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">\n" ;

            //places
            for (int i = 0 ; i < mainWindow->places_contours_line_strings.count() ; i++)
            {
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

            //roads
            for (int i = 0 ; i < mainWindow->valid_roads.count() ; i++)
            {
                out << "<polyline points=\"" ;
                for (int j = 0 ; j < mainWindow->roads_line_strings[mainWindow->valid_roads[i]].count() - 1 ; j++)
                {
                    QPointF point = mainWindow->roads_line_strings[mainWindow->valid_roads[i]][j] ;
                    out << point.x() << " " << point.y() << "," ;
                }
                QPointF point = mainWindow->roads_line_strings[mainWindow->valid_roads[i]]
                        [mainWindow->roads_line_strings[mainWindow->valid_roads[i]].count() - 1] ;
                 out << point.x() << " " << point.y() ;

                 QColor c = colors[i] ;

                out << "\" stroke=\""+c.name()+"\" fill=\"none\" stroke-width=\"1\"/>\n" ;
            }

            out << "</svg>\n" ;
        }
        data.close() ;
}


void RoadMapWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glLineWidth(3);
}

void RoadMapWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h) ;
    }

void RoadMapWidget::paintGL()
{
    glClearColor(1,1,1,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3);

    QMatrix4x4 m;
    m.scale(2*scale/width(),-2*scale/height());
    m.translate(-center.x(),-center.y());

    //roads

    mainWindow->line_program->bind();

    valid_roads_vbo.bind();
    mainWindow->line_program->setUniformValue("matrix", m);
    mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
    mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
    mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

    valid_roads_index_vbo->bind();
    glLineWidth(3);
    glDrawElements(GL_LINES, valid_roads_index_vbo->size()/sizeof(unsigned int), GL_UNSIGNED_INT,0);
    valid_roads_vbo.release();
    valid_roads_index_vbo->release();


}
