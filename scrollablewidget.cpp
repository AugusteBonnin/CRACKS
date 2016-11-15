#include "scrollablewidget.h"

#include <QMouseEvent>
#include <QPainter>

ScrollableWidget::ScrollableWidget(Form * parent) : ImageWidget(parent)
{

}

ScrollableWidget::~ScrollableWidget()
{

}

void ScrollableWidget::mousePressEvent(QMouseEvent *event)
{
    dragStart = event->pos() ;
}

void ScrollableWidget::mouseMoveEvent(QMouseEvent *event)
{
    center -= (event->pos() - dragStart)/scale ;
    dragStart = event->pos() ;

    imageToScreen = QTransform(scale,0,0,scale,
                               (width()>>1)-center.x()*scale,(height()>>1)-center.y()*scale);
    screenToImage = imageToScreen.inverted() ;

    repaint();
}
void ScrollableWidget::mouseReleaseEvent(QMouseEvent *) {
}




void ScrollableWidget::wheelEvent(QWheelEvent *event)
{
    scale *= (1000.0/(1000.0+event->delta())) ;

    imageToScreen = QTransform(scale,0,0,scale,
                               (width()>>1)-center.x()*scale,(height()>>1)-center.y()*scale);
    screenToImage = imageToScreen.inverted() ;
    repaint() ;
}

void ScrollableWidget::resetImage(QImage & image)
{
    background = image;
    if (image.width()/(float)image.height()<width()/(float)height())
    {
        scale = height()/(float)image.height() ;
        imageToScreen = QTransform(scale,0,0,scale,(width()-image.width()*height()/(float)image.height())*.5,0);

    }
    else
    {
        scale = width()/(float)image.width() ;
        imageToScreen = QTransform(scale,0,0,scale,0,(height()-image.height()*width()/(float)image.width())*.5);
    }
    screenToImage = imageToScreen.inverted() ;

    center = QPointF(image.width()*.5,image.height()*.5);
    repaint() ;
}

void ScrollableWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawRect(QRect(0,0,width()-1,height()-1));
    painter.setTransform(imageToScreen);
    painter.drawImage(0,0,background);
}
