#include "cropzoomwidget.h"

#include <QMouseEvent>

CropZoomWidget::CropZoomWidget(Form *parent) : CropQuadWidget(parent)
{
    connect(this,SIGNAL(POIchanged(QPoint)),this,SLOT(updatePOI(QPoint)));
}

QSize CropZoomWidget::sizeHint() const
{
    return QSize(256,256);
}
QSize CropZoomWidget::minimumSizeHint() const
{
    return QSize(256,256);
}
QSize CropZoomWidget::maximumSizeHint() const
{
    return QSize(256,256);
}

void CropZoomWidget::setZoomImage(QImage &image)
{
    background = image;

    imageToScreen = QTransform(16,0,0,16,width()>>1,height()>>1);
    screenToImage = imageToScreen.inverted() ;

    repaint() ;
}

void CropZoomWidget::updateSelection(int s)
{
    selectedCorner = s ;
    repaint() ;
}

void CropZoomWidget::updatePOI(QPoint p)
{
    imageToScreen = QTransform(16,0,0,16,(width()>>1)-16*p.x(),(height()>>1)-16*p.y());
    screenToImage = imageToScreen.inverted() ;
    polygon[selectedCorner] = p ;
    repaint() ;
}

void CropZoomWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x,y;
    screenToImage.map(event->x(),event->y(),&x,&y);
    polygon[selectedCorner].setX(x);
    polygon[selectedCorner].setY(y);

    repaint() ;
}
