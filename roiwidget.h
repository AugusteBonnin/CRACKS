#ifndef ROIWIDGET_H
#define ROIWIDGET_H

#include "imagewidget.h"

#include <QWidget>

class ROIWidget : public ImageWidget
{
    Q_OBJECT

    QRect ROI ;
    QPoint selectedOffset ;
    bool dragging;
public:
    ROIWidget(Form *parent);

    QRect getROI();
signals :
    void ROIChanged(QRect);
    void mouseReleased() ;
public slots:
    void setROI(const QRect & inRect) ;
protected :
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // ROIWIDGET_H
