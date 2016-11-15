#ifndef SCROLLABLEWIDGET_H
#define SCROLLABLEWIDGET_H

#include "imagewidget.h"



class ScrollableWidget : public ImageWidget
{
    Q_OBJECT

    QPointF center ;
    QPoint dragStart ;
    double scale ;
public:
    ScrollableWidget(Form *parent);
    ~ScrollableWidget();
    void resetImage(QImage &image);
signals:

public slots:

protected :
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // SCROLLABLEWIDGET_H
