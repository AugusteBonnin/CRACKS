#include "roadmapwidget.h"
#include "mainwindow.h"

RoadMapWidget::RoadMapWidget(MapPage *parent, QVector<QColor> & colors) : MapWidget(parent) , colors(colors)
{

    QVector<float> pre_valid_roads_vbo ;
    QVector<uint32_t> pre_valid_roads_index_vbo ;

    for (int i = 0 ; i < colors.count() ; i++)
    {
        int j = 0 ;
        QVector<QPointF> & road = mainWindow->roads_line_strings[i] ;
        for  (;j < road.count()-1 ; j++)
        {
            pre_valid_roads_index_vbo << pre_valid_roads_vbo.count()/6 << pre_valid_roads_vbo.count()/6 +1;
            pre_valid_roads_vbo << road[j].x()
                                   << road[j].y()
                                   << colors[i].redF()
                                   << colors[i].greenF()
                                   << colors[i].blueF()
                                   << 1 ;

        }
        pre_valid_roads_vbo << road[j].x()
                               << road[j].y()
                               << colors[i].redF()
                               << colors[i].greenF()
                               << colors[i].blueF()
                               << 1 ;

    }

//    int i = 0 ;
//    while (i < mainWindow->valid_roads.count())
//    {

//        for (int k = mainWindow->pre_valid_roads_index_vbo_start[i];
//             k <  mainWindow->pre_valid_roads_index_vbo_end[i]
//             ; k++)
//        {
//            pre_roads_vbo[mainWindow->pre_valid_roads_index_vbo[k]*6+2] = colors[i].redF() ;
//            pre_roads_vbo[mainWindow->pre_valid_roads_index_vbo[k]*6+3] = colors[i].greenF() ;
//            pre_roads_vbo[mainWindow->pre_valid_roads_index_vbo[k]*6+4] = colors[i].blueF() ;
//        }
//        i++ ;
//    }

    valid_roads_vbo.create();
    valid_roads_vbo.bind();
    valid_roads_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    valid_roads_vbo.allocate(pre_valid_roads_vbo.constData(),pre_valid_roads_vbo.count()*sizeof(float));
valid_roads_vbo.release();

    valid_roads_index_vbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    valid_roads_index_vbo->create();
    valid_roads_index_vbo->bind();
    valid_roads_index_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    valid_roads_index_vbo->allocate(pre_valid_roads_index_vbo.constData(),
                                    pre_valid_roads_index_vbo.count()*sizeof(uint32_t));
valid_roads_index_vbo->release();
//places
    QVector<QVector<float> > & pre_junctions_vbos = mainWindow->pre_junctions_vbos ;

    for (int i = 0 ; i < pre_junctions_vbos.count();i++)
    {
        for (int k = 0 ; k < pre_junctions_vbos[i].count() ; k+=6)
        {
            pre_junctions_vbos[i][k+2] = 1.0f ;
            pre_junctions_vbos[i][k+3] = 1.0f ;
            pre_junctions_vbos[i][k+4] = 1.0f ;
        }
        QOpenGLBuffer *vbo = new QOpenGLBuffer;
        vbo->create();
        vbo->bind();
        vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo->allocate(pre_junctions_vbos[i].constData(),pre_junctions_vbos[i].count()*sizeof(float));
        vbo->release();
        junctions_vbos << vbo ;

    }
}

void RoadMapWidget::saveSVG(QString path)
{
    QFile data(path);
        if (data.open(QFile::WriteOnly)) {
            QTextStream out(&data);
            out << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">\n" ;
QVector<QVector<QPointF > > & places = mainWindow->places_contours_line_strings;
            //places
            for (int i = 0 ; i < places.count() ; i++)
            {
                out << "<polyline points=\"" ;
                for (int j = 0 ; j < places[i].count()-1 ; j++)
                {
                    QPointF point = places[i][j] ;
                    out << point.x() << " " << point.y() << "," ;
                }
                //repeat first point
                QPointF point = places[i][places[i].count()-1 ] ;
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

    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

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

    mainWindow->line_program->bind();


    //junctions contours
    for (int i = 0 ; i < mainWindow->junctions_contours_vbos.count() ; i++)
    {
        mainWindow->junctions_contours_vbos[i]->bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));
        glDrawArrays(GL_LINE_STRIP,0,mainWindow->junctions_contours_vbos[i]->size()/(6*sizeof(float)));
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
        glDrawArrays(GL_POLYGON,0,junctions_vbos[i]->size()/(6*sizeof(float)));
        junctions_vbos[i]->release();
    }
    //roads

    valid_roads_vbo.bind();
    mainWindow->line_program->setUniformValue("matrix", m);
    mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
    mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
    mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

    valid_roads_index_vbo->bind();
    glLineWidth(3);
    glDrawElements(GL_LINES, valid_roads_index_vbo->size()/sizeof(uint32_t), GL_UNSIGNED_INT,0);
    valid_roads_vbo.release();
    valid_roads_index_vbo->release();


}
