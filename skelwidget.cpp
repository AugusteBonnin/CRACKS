#include "skelwidget.h"
#include "customtypes.h"
#include "mainwindow.h"
#include "kdtree2d.h"
#include "skelpage.h"
#include "skelworker.h"
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

    glLineWidth(3);

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
    if (mainWindow->getSkelIndices().count())
    {
        mainWindow->line_program->bind();
        mainWindow->skel_vbo.bind();
        mainWindow->line_program->setUniformValue("matrix", m);
        mainWindow->line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        mainWindow->line_program->enableAttributeArray(PROGRAM_COLOR_ATTRIBUTE);
        mainWindow->line_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 2, 6 * sizeof(GLfloat));
        mainWindow->line_program->setAttributeBuffer(PROGRAM_COLOR_ATTRIBUTE, GL_FLOAT, 2 * sizeof(GLfloat), 4, 6 * sizeof(GLfloat));

        mainWindow->skel_index_vbo->bind();
        glDrawElements(GL_LINES, mainWindow->getSkelIndices().count(), GL_UNSIGNED_INT,0);
        mainWindow->skel_vbo.release();
        mainWindow->skel_index_vbo->release();
    }
}


void SkelWidget::buildSkel()
{
    mainWindow->setActionsEnabled(false);
    mainWindow->progress->setValue(0);
    SkelWorker * worker = new SkelWorker(mainWindow) ;
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

    qreal max_d = 0;
    for (int i = 0 ; i < skel_distance.count() ; i++)
    {
        if (skel_distance[i] > max_d)
            max_d = skel_distance[i] ;
        emit progressIncrement(1);
    }

    QVector<float> vertices ;
    float inverse_max_d = 1.0/max_d ;
    for (int i = 0 ; i < skel_points.count() ; i++)
    {
        vertices << skel_points[i].x() << skel_points[i].y() ;
        float c = skel_distance[i]*inverse_max_d;
        //vertices << c << 0 << 1-c << 1;
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

    //line strings computing
    mainWindow->getDoubleSidedEdges().clear();

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

    mainWindow->setActionsEnabled(true);

   update() ;
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
    QVector<int> str ;
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
