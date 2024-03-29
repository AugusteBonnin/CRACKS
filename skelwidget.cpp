#include "skelwidget.h"
#include "customtypes.h"
#include "mainwindow.h"
#include "kdtree2d.h"
#include "skelpage.h"
#include "skelworker.h"
#include "skelworker2.h"
#include "skelworker3.h"
#include <QApplication>

#include <QAction>
#include <QThreadPool>

int SkelWidget::getProgressMax()
{
    const int step3Max = skel_distance.count();
    const int step4Max = skel_points.count();
    const int step5Max = skel_points.count() ;
    const int step6Max = skel_children.count() ;
return    step3Max+step4Max+step5Max+step6Max;
}

SkelWidget::SkelWidget(SkelPage * parent) : ScrollableOpenGLWidget(parent) ,
    connected_components(mainWindow->getConnectedComponentsStarts()) ,
    skel_children(mainWindow->getSkelChildren()) ,
    contour_vertices(mainWindow->getContourVertices()) ,
    skel_points(mainWindow->getSkelVertices()) ,
    next_point_index(mainWindow->getNextPointIndex()) ,
    skel_distance(mainWindow->getSkelDistanceToBoundary()) ,
    skel_colors(mainWindow->getSkelColors()) ,
    skel_indices(mainWindow->getSkelIndices())
{

}

SkelWidget::~SkelWidget()
{

}

void SkelWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);

    mainWindow->skel_index_vbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer) ;
    mainWindow->faces_index_vbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer) ;
}

void SkelWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void SkelWidget::paintGL()
{
    glClearColor(.5f,.5f,.5f,1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    QMatrix4x4 m;
    m.scale(2*scale/(width()-1),-2*scale/(height()-1));
    m.translate(-center.x(),-center.y());

    //background quad
    mainWindow->quad_program->bind();
    mainWindow->quad_vbo.bind();
    mainWindow->quad_program->setUniformValue("matrix", m);
    mainWindow->quad_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->quad_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    mainWindow->quad_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
    mainWindow->quad_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

    mainWindow->texture->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mainWindow->quad_vbo.release();


    //contour lines
    if (mainWindow->getContourIndices().count())
    {
        mainWindow->line_program->bind();
        mainWindow->contour_vbo.bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

        mainWindow->contour_index_vbo->bind();
        glDrawElements(GL_LINES, mainWindow->getContourIndices().count(), GL_UNSIGNED_INT,0);
        mainWindow->contour_vbo.release();
        mainWindow->contour_index_vbo->release();
    }

    //selection triangle
    if (mainWindow->selection_vbo.isCreated())
    {
    mainWindow->line_program->bind();
    mainWindow->selection_vbo.bind();
    mainWindow->line_program->setUniformValue("matrix", m);
    mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
    mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
    mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

    glDrawArrays(GL_LINE_STRIP, 0,4);
    mainWindow->selection_vbo.release();
}

    if (mainWindow->circle_vbo.isCreated())
    {
    mainWindow->line_program->bind();
    mainWindow->circle_vbo.bind();
    mainWindow->line_program->setUniformValue("matrix", m);
    mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
    mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
    mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

    glDrawArrays(GL_LINE_STRIP, 0,61);
    mainWindow->circle_vbo.release();
}
    //skeletton
    if (mainWindow->skel_index_vbo->isCreated())
    {
        mainWindow->line_program->bind();
        mainWindow->skel_vbo.bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

        mainWindow->skel_index_vbo->bind();
        glDrawElements(GL_LINES, mainWindow->skel_index_vbo->size()/sizeof(uint32_t), GL_UNSIGNED_INT,0);
        mainWindow->skel_vbo.release();
        mainWindow->skel_index_vbo->release();
    }
}


void SkelWidget::buildSkel()
{
    mainWindow->setActionsEnabled(false);
    mainWindow->progress->setValue(0);
    SkelWorker3 * worker = new SkelWorker3(mainWindow) ;
    mainWindow->progress->setMaximum(getProgressMax()+worker->getProgressMax());

    connect(worker,SIGNAL(finished()),this,SLOT(buildSkel2())) ;
    connect(worker,SIGNAL(progressIncrement(int)),mainWindow,SLOT(receiveProgressIncrement(int))) ;
            connect(this,SIGNAL(progressIncrement(int)),mainWindow,SLOT(receiveProgressIncrement(int))) ;

    worker->start();

}

void SkelWidget::buildSkel2()
    {
    //building skel vertices and colors
    makeCurrent();
    skel_colors.clear() ;


    QVector<float> vertices ;
    for (int i = 0 ; i < skel_points.count() ; i++)
    {
        vertices << skel_points[i].x() << skel_points[i].y() ;
       vertices << 0 << 1 << 0 << 1;
       emit progressIncrement(1);
    }

    if (mainWindow->skel_vbo.isCreated())
        mainWindow->skel_vbo.destroy();

    mainWindow->skel_vbo.create() ;
    mainWindow->skel_vbo.bind();
    mainWindow->skel_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mainWindow->skel_vbo.allocate(vertices.constData(),vertices.count()*sizeof(float));
    vertices.clear() ;

    skel_indices.clear() ;

    for (int i = 0 ; i < skel_points.count() ; i++)
    {
        for (int j = 0 ; j < skel_children[i].count() ; j++)
            if (i <  skel_children[i][j])
            {
                skel_indices.append(i);
                skel_indices.append( skel_children[i][j]);
            }
        emit progressIncrement(1);
    }

    if (mainWindow->skel_index_vbo->isCreated())
        mainWindow->skel_index_vbo->destroy();

    mainWindow->skel_index_vbo->create() ;
    mainWindow->skel_index_vbo->bind() ;
    mainWindow->skel_index_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mainWindow->skel_index_vbo->allocate(skel_indices.constData(),skel_indices.count()*sizeof(unsigned int));

    mainWindow->getDoubleSidedEdges().clear();
    vertexEdges.clear();

    int total_vertices = 0 ;

    for (int i = 0 ; i  < skel_children.count() ; i++)
    {
        if((skel_children[i].count()==1)||(skel_children[i].count()>=3))
        {
            explorePoint(i);
            total_vertices++ ;
        }
        emit progressIncrement(1);
    }

    mainWindow->getEdgeCloseness().clear();
    QVector<QVector<uint32_t> > distances;
    QVector<DoubleSidedEdge> & edges = mainWindow->getDoubleSidedEdges();
    QVector<uint32_t> tmp_distance(edges.count());
    for (int i = 0 ; i < edges.count() ; ++i)
    {
        for (int j = 0 ; j < edges.count();++j)
            if (i==j)
                tmp_distance[j] = 0 ;
            else
                tmp_distance[j] = std::numeric_limits<uint32_t>().max();

        QVector<uint32_t> fifo_in,fifo_out;
        fifo_in << i ;
        while (fifo_in.count())
        {
            fifo_out.clear();
            for (int j = 0 ; j < fifo_in.count() ; ++j)
            {
                DoubleSidedEdge & e1 = edges[fifo_in[j]];
                QVector<uint32_t> edges1 = vertexEdges.value(e1.str[0]);
                for (int k = 0 ; k < edges1.count() ; ++k)
                {
                    if (edges1[k] != fifo_in[j])
                    {
                        if (tmp_distance[fifo_in[j]]+1<tmp_distance[edges1[k]])
                        {
                            tmp_distance[edges1[k]] = tmp_distance[fifo_in[j]]+1 ;
                            fifo_out << edges1[k] ;
                        }
                    }
                }
                QVector<uint32_t> edges2 = vertexEdges.value(e1.str[e1.str.count()-1]);
                for (int k = 0 ; k < edges2.count() ; ++k)
                {
                    if (edges2[k] != fifo_in[j])
                    {
                        if (tmp_distance[fifo_in[j]]+1<tmp_distance[edges2[k]])
                        {
                            tmp_distance[edges2[k]] = tmp_distance[fifo_in[j]]+1 ;
                            fifo_out << edges2[k] ;
                        }
                    }
                }
            }
            fifo_in = fifo_out ;
        }
        QVector<uint32_t> tmp2_distance;
        uint32_t valid_edges = 0 ;
        double closeness=0;
        for (int j = 0 ; j < tmp_distance.count() ; ++j)
        {
            if (tmp_distance[j]<std::numeric_limits<uint32_t>().max())
            {
                valid_edges++;
                tmp2_distance << tmp_distance[j];
                closeness+=tmp_distance[j];
            }
        }
        if (valid_edges>edges.count()/10)
        {
            distances << tmp2_distance ;
            closeness = 1.0/closeness;
           mainWindow->getEdgeCloseness()<<closeness;
        }
    }

    mainWindow->setActionsEnabled(true);

   update() ;

   ((Page*)parent())->initDone = true ;
}

void SkelWidget::explorePoint(int i)
{
    for (int j = 0 ; j < skel_children[i].count() ; j++)
        exploreEdge(i,skel_children[i][j]);
}

void SkelWidget::exploreEdge(int first, int second)
{
    int current = second ;
    int prev = first ;
    int next;
    QPointF p0 = skel_points[prev] , p1 = p0 ;
    QVector<uint32_t> str ;
    str<<prev;
    double mean_width = skel_distance[prev] ;
    QPointF p2;
    while (skel_children[current].count()==2)
    {
        str<<current;
        mean_width += skel_distance[current] ;
        p2 = skel_points[current];
        next = (skel_children[current][0]==prev)?skel_children[current][1]:skel_children[current][0];
        prev = current;
        current = next ;
        p1=p2;
    }

    int last = current ;

    if (last>first)
    {
        str << last ;
        mean_width += skel_distance[last] ;
        mean_width /= str.count() ;

        DoubleSidedEdge edge ;
        edge.color = randColor() ;
        edge.str = str ;
        edge.mean_distance = mean_width ;
        mainWindow->getDoubleSidedEdges().append(edge);

        {
            QVector<uint32_t> edges = vertexEdges.value(str[0]);
            edges << mainWindow->getDoubleSidedEdges().count()-1;
            vertexEdges.insert(str[0],edges);
        }
        {
            QVector<uint32_t> edges = vertexEdges.value(str[str.count()-1]);
            edges << mainWindow->getDoubleSidedEdges().count()-1;
            vertexEdges.insert(str[str.count()-1],edges);
        }


    }

}

QRgb SkelWidget::randColor()
{
    /*
                                                     * float angle = 2*M_PI*rand()/32768.0 ;
                                                    return qRgb((unsigned char)(127*(1+sinf(angle))),
                                                                (unsigned char)(127*(1+sinf(angle+2*M_PI/3))),
                                                                (unsigned char)(127*(1+sinf(angle+4*M_PI/3)))
                                                                ) ;
                                                               */
    return qRgb((unsigned char)rand(),(unsigned char)rand(),(unsigned char)rand()) ;
}
