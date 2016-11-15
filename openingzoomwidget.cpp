#include "openingzoomwidget.h"

#include <QMouseEvent>

OpeningZoomWidget::OpeningZoomWidget(Form * parent) : ImageWidget(parent)
{
scale = 1 ;
center = QPoint(0,0);
}

QSize OpeningZoomWidget::sizeHint() const
{
    return QSize(256,256);
}

QSize OpeningZoomWidget::minimumSizeHint() const
{
    return QSize(256,256);
}

void OpeningZoomWidget::setMagnification(int mag)
{
    scale = mag ;

    int  w2 = (width() / scale)>>1 ;
    int  h2 = (height() / scale)>>1 ;

    QRect ROI(QPoint(center.x()-w2,center.y()-h2),QPoint(center.x()+w2,center.y()+h2));

    emit ROIChanged(ROI);

    //    imageToScreen = QMatrix(magnification,0,0,magnification,
    //                            (width()>>1)-magnification*center.x(),(height()>>1)-magnification*center.y());
    //    screenToImage = imageToScreen.inverted() ;
        imageToScreen = QTransform(scale,0,0,scale,
                                (width()/2)-(center.x())*scale,(height()/2)-(center.y())*scale);
        screenToImage = imageToScreen.inverted() ;

    repaint() ;
}

void OpeningZoomWidget::setROI(QRect rect)
{
    center = rect.center() ;

    imageToScreen = QTransform(scale,0,0,scale,
                            (width()>>1)-center.x()*scale,(height()>>1)-center.y()*scale);
    screenToImage = imageToScreen.inverted() ;

    repaint() ;
}

void OpeningZoomWidget::mousePressEvent(QMouseEvent *event)
{
    dragStart = event->pos() ;
}

void OpeningZoomWidget::mouseMoveEvent(QMouseEvent *event)
{
    center -= (event->pos() - dragStart)/scale ;
    dragStart = event->pos() ;

    int  w2 = (width() / scale)>>1 ;
    int  h2 = (height() / scale)>>1 ;

    int x1 = center.x()-w2 ;
    int y1 = center.y()-h2 ;

    QRect ROI(QPoint(x1,y1),QPoint(center.x()+w2,center.y()+h2));
    emit ROIChanged(ROI);

    imageToScreen = QTransform(scale,0,0,scale,
                            (width()>>1)-center.x()*scale,(height()>>1)-center.y()*scale);
    screenToImage = imageToScreen.inverted() ;

    repaint();
}
    void OpeningZoomWidget::mouseReleaseEvent(QMouseEvent *) {
        emit mouseReleased();
    }
