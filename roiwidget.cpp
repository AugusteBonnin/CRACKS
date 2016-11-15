#include "roiwidget.h"

#include <QMouseEvent>
#include <QPainter>

ROIWidget::ROIWidget(Form * parent) : ImageWidget(parent)
{

}

void ROIWidget::setROI(const QRect &inRect)
{
    ROI = inRect ;

    repaint() ;
}

void ROIWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawRect(QRect(0,0,width()-1,height()-1));
    painter.setTransform(imageToScreen);
    painter.drawImage(0,0,background);
    QPen pen = painter.pen() ;
    pen.setColor(Qt::yellow);
    pen.setWidthF(1.0/imageToScreen.m11());
    painter.setPen(pen);
    painter.drawRect(ROI);

}

void ROIWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging)
    {
        int x,y;
        screenToImage.map(event->x(),event->y(),&x,&y);

        ROI.moveTo(x-selectedOffset.x(),y-selectedOffset.y());

        emit ROIChanged(ROI);

        repaint() ;
    }
}

void ROIWidget::mousePressEvent(QMouseEvent *event)
{

    int x,y;
    screenToImage.map(event->x(),event->y(),&x,&y);

    if (ROI.contains(x,y,false))
    {
        selectedOffset = QPoint(x-ROI.x(),y-ROI.y());
        dragging = true ;
    }

    repaint();
}

void ROIWidget::mouseReleaseEvent(QMouseEvent *event)
{
    dragging = false ;
    emit ROIChanged(ROI);
    emit mouseReleased();
    repaint() ;
}

QRect ROIWidget::getROI()
{
    return ROI ;
}
