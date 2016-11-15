#include "cropquadwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QSettings>
#include <QDebug>

CropQuadWidget::CropQuadWidget(Form *parent) : ImageWidget(parent)
{
    selectedCorner = 0 ;
    dragging = false ;


}

void CropQuadWidget::initSourcePolygon()
{
    QSettings settings ;
    polygon.clear() ;
    polygon << settings.value("CropForm-Polygon0",QVariant(QPoint(0,0))).toPoint() ;
    polygon << settings.value("CropForm-Polygon1",QVariant(QPoint(background.width()-1,0))).toPoint() ;
    polygon << settings.value("CropForm-Polygon2",QVariant(QPoint(background.width()-1,background.height()-1))).toPoint() ;
    polygon << settings.value("CropForm-Polygon3",QVariant(QPoint(0,background.height()-1))).toPoint() ;

}

void CropQuadWidget::makeRectangle()
{
    polygon[1].setX(polygon[2].x()) ;
    polygon[1].setY(polygon[0].y()) ;
    polygon[3].setX(polygon[0].x()) ;
    polygon[3].setY(polygon[2].y()) ;
    QSettings settings ;
    settings.setValue("CropForm-Polygon0",polygon[0]);
    settings.setValue("CropForm-Polygon1",polygon[1]);
    settings.setValue("CropForm-Polygon2",polygon[2]);
    settings.setValue("CropForm-Polygon3",polygon[3]);

    emit quadChanged(polygon);

    repaint() ;
    }

void CropQuadWidget::RAZ()
{
    QSettings settings ;
    settings.setValue("CropForm-Polygon0",QPoint(0,0));
    settings.setValue("CropForm-Polygon1",QPoint(background.width()-1,0));
    settings.setValue("CropForm-Polygon2",QPoint(background.width()-1,background.height()-1));
    settings.setValue("CropForm-Polygon3",QPoint(0,background.height()-1));
    polygon.clear() ;
    polygon << QPoint(0,0) ;
    polygon << QPoint(background.width()-1,0) ;
    polygon << QPoint(background.width()-1,background.height()-1) ;
    polygon << QPoint(0,background.height()-1) ;

    emit quadChanged(polygon);

    repaint() ;
}

void CropQuadWidget::updateCorner(QPoint p)
{
    polygon[selectedCorner] = p ;
    emit quadChanged(polygon);
    repaint();
}

void CropQuadWidget::paintEvent(QPaintEvent *event)
{
    ImageWidget::paintEvent(event);

    QPainter painter(this) ;
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen = painter.pen() ;

    for (int i = 0 ; i < 4 ; i ++ )
    {
        int x,y;
        imageToScreen.map(polygon[i].x(),polygon[i].y(),&x,&y);

        pen.setColor(Qt::black);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(QRect(x-8,y-8,17,17));

        if (i==selectedCorner)
            pen.setColor(Qt::green);
        else
            pen.setColor(Qt::yellow);

        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(QRect(x-8,y-8,17,17));

        }


    QPolygon transformedPolygon = imageToScreen.map(polygon);
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawPolygon(transformedPolygon);
    pen.setColor(Qt::yellow);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawPolygon(transformedPolygon);
    }



void CropQuadWidget::mousePressEvent(QMouseEvent *event)
{
    for (int i = 0 ; i < 4 ; i ++ )
    {
        int x,y;
        imageToScreen.map(polygon[i].x(),polygon[i].y(),&x,&y);

        if ((abs(x-event->x())<9)&&(abs(y-event->y())<9))
        {
            selectedCorner = i ;
            dragging = true ;
            emit selectionChanged(i);
            break ;
        }
    }
    repaint();
}

void CropQuadWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging)
    {
        int x,y;
        screenToImage.map(event->x(),event->y(),&x,&y);
        polygon[selectedCorner].setX(x);
        polygon[selectedCorner].setY(y);
        emit POIchanged(polygon[selectedCorner]);

        repaint() ;
    }
}

void CropQuadWidget::mouseReleaseEvent(QMouseEvent *event)
{
    dragging = false ;
    emit POIchanged(polygon[selectedCorner]);
    emit quadChanged(polygon);
}

