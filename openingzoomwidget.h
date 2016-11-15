#ifndef OPENINGZOOMWIDGET_H
#define OPENINGZOOMWIDGET_H

#include "imagewidget.h"

#include <QWidget>

class OpeningZoomWidget : public ImageWidget
{
    Q_OBJECT

    int scale ;
    QPoint center ;
    QPoint dragStart ;
public:
    OpeningZoomWidget(Form * parent);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
public slots :
    void setMagnification(int mag) ;
    void setROI(QRect rect) ;

signals :
    void ROIChanged(QRect) ;
    void mouseReleased() ;
protected :
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // OPENINGZOOMWIDGET_H
