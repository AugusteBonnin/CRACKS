#ifndef CROPQUADWIDGET_H
#define CROPQUADWIDGET_H

#include "imagewidget.h"



class CropQuadWidget : public ImageWidget
{
    Q_OBJECT

    bool dragging ;
public:
    CropQuadWidget(Form *parent = 0 );
    void initSourcePolygon() ;
    void makeRectangle() ;
    QPolygon polygon ;
signals:
    void quadChanged(QPolygon) ;
    void POIchanged(QPoint);
    void selectionChanged(int);
public slots:
    void updateCorner(QPoint);
    void RAZ();
protected :
    int selectedCorner ;
    virtual void	paintEvent(QPaintEvent * event);
    virtual void	mouseMoveEvent(QMouseEvent * event);
    virtual void	mousePressEvent(QMouseEvent * event);
    virtual void	mouseReleaseEvent(QMouseEvent * event);

};

#endif // CROPQUADWIDGET_H
