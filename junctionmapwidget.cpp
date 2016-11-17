#include "junctionmapwidget.h"
#include "mainwindow.h"

JunctionMapWidget::JunctionMapWidget(MapPage *parent,QVector<QColor> & colors) : MapWidget(parent) , colors(colors)
{

    QVector<QVector<float> > & pre_junctions_vbos = mainWindow->pre_junctions_vbos ;

    for (int i = 0 ; i < pre_junctions_vbos.count();i++)
    {
        for (int k = 0 ; k < pre_junctions_vbos[i].count() ; k+=6)
        {
            pre_junctions_vbos[i][k+2] = colors[i].redF() ;
            pre_junctions_vbos[i][k+3] = colors[i].greenF() ;
            pre_junctions_vbos[i][k+4] = colors[i].blueF() ;
        }
        QOpenGLBuffer *vbo = new QOpenGLBuffer;
        vbo->create();
        vbo->bind();
        vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo->allocate(pre_junctions_vbos[i].constData(),pre_junctions_vbos[i].count()*sizeof(float));
        junctions_vbos << vbo ;

    }

}

void JunctionMapWidget::saveSVG(QString path)
{
    QFile data(path);
        if (data.open(QFile::WriteOnly)) {
            QTextStream out(&data);
            out << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">\n" ;

            //places
            for (int i = 0 ; i < mainWindow->places_contours_line_strings.count() ; i++)
            {
                QColor c = colors[i] ;
                out << "<polyline points=\"" ;
                for (int j = 0 ; j < mainWindow->places_contours_line_strings[i].count() - 1 ; j++)
                {
                    QPointF point = mainWindow->places_contours_line_strings[i][j] ;
                    out << point.x() << " " << point.y() << "," ;
                }
                //repeat first point
                QPointF point = mainWindow->places_contours_line_strings[i][0] ;
                 out << point.x() << " " << point.y() ;

                out << "\" stroke=\"black\" fill=\""+
                       c.name()+
                        "\" stroke-width=\"1\"/>\n" ;
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

                out << "\" stroke=\"black\" fill=\"none\" stroke-width=\"1\"/>\n" ;
            }

            out << "</svg>\n" ;
        }
        data.close() ;
}


void JunctionMapWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glLineWidth(3);
}

void JunctionMapWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h) ;
}

void JunctionMapWidget::paintGL()
{
    glClearColor(1,1,1,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3);

    QMatrix4x4 m;
    m.scale(2*scale/width(),-2*scale/height());
    m.translate(-center.x(),-center.y());

    //roads

    mainWindow->line_program->bind();

    mainWindow->black_roads_vbo.bind();
    mainWindow->line_program->setUniformValue("matrix", m);
    mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
    mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
    mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

    mainWindow->roads_index_vbo->bind();
    glLineWidth(3);
    glDrawElements(GL_LINES, mainWindow->roads_index_vbo->size()/sizeof(unsigned int), GL_UNSIGNED_INT,0);
    mainWindow->black_roads_vbo.release();
    mainWindow->roads_index_vbo->release();

    //junctions contours
    for (int i = 0 ; i < mainWindow->junctions_contours_vbos.count() ; i++)
    {
        mainWindow->junctions_contours_vbos[i]->bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));
        glDrawArrays(GL_LINE_STRIP,0,mainWindow->places_contours_line_strings[i].count());
        mainWindow->junctions_contours_vbos[i]->release();
    }
    //junctions colors
    for (int i = 0 ; i < junctions_vbos.count() ; i++)
    {
        junctions_vbos[i]->bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));
        glDrawArrays(GL_POLYGON,0,mainWindow->places_contours_line_strings[i].count());
        junctions_vbos[i]->release();
    }
}
