#include "scrollableopenglwidget.h"

#include <QOpenGLTexture>
#include "customtypes.h"
#include "mainwindow.h"

ScrollableOpenGLWidget::ScrollableOpenGLWidget(Form * parent) :
    QOpenGLWidget(parent) , scale(parent->getMainWindow()->scale) ,
    center(parent->getMainWindow()->center)
{
    mainWindow = parent->getMainWindow() ;

    isDragging = false ;

    setMouseTracking(true);
}

void ScrollableOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    dragStart = event->pos() ;
    isDragging = true ;
}

void ScrollableOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        center -= (event->pos() - dragStart)/scale ;
        dragStart = event->pos() ;
    }
//    else
//    {
//        if (mainWindow->kdtree)
//        {
//            float hovered[]={(event->pos().x()-width()/2)/scale+center.x(),
//                       (event->pos().y()-height()/2)/scale+center.y()} ;
//            qDebug() << hovered[0] << "," << hovered[1] ;
//            size_t out_indice[1];
//            float out_distance[1];
//            mainWindow->kdtree->knnSearch(hovered,1,out_indice,out_distance) ;
//            Vertex_handle 	handle = mainWindow->treeIdx2Handle[out_indice[0]];

//            Triangulation::Face_circulator start = mainWindow->cdt.incident_faces(handle) ;
//            Triangulation::Face_circulator current = start ;

//            QVector<float> vbo , circle_vbo;
//            QPointF A(hovered[0],hovered[1]) ;

//            do {
//                QPolygonF tri ;
//                tri << QPointF(current->vertex(0)->point().x(),current->vertex(0)->point().y()) ;
//                tri << QPointF(current->vertex(1)->point().x(),current->vertex(1)->point().y()) ;
//                tri << QPointF(current->vertex(2)->point().x(),current->vertex(2)->point().y()) ;
//                if (tri.containsPoint(A,Qt::OddEvenFill)&&current->info().in_domain())
//                {
//                    vbo << tri[0].x() << tri[0].y() ;
//                    vbo << 0 << 1 << 0 << 1;
//                    vbo << tri[1].x() << tri[1].y() ;
//                    vbo << 0 << 1 << 0 << 1;
//                    vbo << tri[2].x() << tri[2].y() ;
//                    vbo << 0 << 1 << 0 << 1;
//                    vbo << tri[0].x() << tri[0].y() ;
//                    vbo << 0 << 1 << 0 << 1;

//                    CGALTriangle t(current->vertex(0),current->vertex(1),current->vertex(2));
//                    if (mainWindow->centersIdx.contains(t))
//                    {
//                        int idx = mainWindow->centersIdx[t] ;
//                        QPointF center = mainWindow->getSkelVertices()[idx] ;
//                        float dist = mainWindow->getSkelDistanceToBoundary()[idx];

//                        for (int i = 0 ; i <= 60 ; i++)
//                        {
//                            float angle = i * M_PI /30 ;
//                            circle_vbo << center.x() + dist * cosf(angle) << center.y() + dist * sinf(angle) ;
//                            circle_vbo << 0 << 1 << 0 << 1;

//                        }
//                    }
//                    else mainWindow->circle_vbo.destroy();

//                    break ;
//                }
//                current++ ;
//            }  while (current!=start);

//            if (mainWindow->selection_vbo.isCreated())
//                mainWindow->selection_vbo.destroy();

//            mainWindow->selection_vbo.create() ;
//            mainWindow->selection_vbo.bind();
//            mainWindow->selection_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//            mainWindow->selection_vbo.allocate(vbo.constData(),vbo.count()*sizeof(float));

//            if (mainWindow->circle_vbo.isCreated())
//                mainWindow->circle_vbo.destroy();

//            mainWindow->circle_vbo.create() ;
//            mainWindow->circle_vbo.bind();
//            mainWindow->circle_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//            mainWindow->circle_vbo.allocate(circle_vbo.constData(),circle_vbo.count()*sizeof(float));

//        }
//    }
    repaint();
}




void ScrollableOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    scale *= (1000.0/(1000.0+event->delta())) ;

    emit ScaleChanged(scale);

    repaint() ;
}

void ScrollableOpenGLWidget::mouseReleaseEvent(QMouseEvent *)
{
    isDragging = false ;
    repaint() ;
}

void ScrollableOpenGLWidget::saveState()
{
    settings.setValue("Scale",scale) ;
    settings.setValue("Center",center); ;
}

void ScrollableOpenGLWidget::resetToSavedState()
{
    scale = settings.value("Scale").toFloat() ;
    center = settings.value("Center").toPointF() ;
    emit ScaleChanged(scale);
}

void ScrollableOpenGLWidget::resetToSize(QSize size)
{
//    if (size.width()/(float)size.height()<width()/(float)height())
//        scale = height()/(float)size.height() ;
//    else
        scale = width()/(float)size.width() ;

    emit ScaleChanged(scale);

    center = QPointF(size.width()*.5,size.height()*.5);
}

void ScrollableOpenGLWidget::resetImage(QImage & image)
{
//    if (image.width()/(float)image.height()<width()/(float)height())
//        scale = height()/(float)image.height() ;
//    else
        scale = width()/(float)image.width() ;

    emit ScaleChanged(scale);

    center = QPointF(image.width()*.5,image.height()*.5);
    update() ;
}

QImage ScrollableOpenGLWidget::getImage()
{
    QImage result ;
    QSize oldSize = size() ;
    saveState() ;
    resize(mainWindow->openedQImage.size()) ;
    resetImage(mainWindow->openedQImage);
    result = grabFramebuffer();
    resize(oldSize);
    resetToSavedState();
    return result ;
}
QImage ScrollableOpenGLWidget::getScreenshot()
{
    return grabFramebuffer();
}
