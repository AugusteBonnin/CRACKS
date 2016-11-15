#include "colorbadge.h"

#include <QPainter>

ColorBadge::ColorBadge(QWidget *parent, QColor color) : QWidget(parent)
{
    this->color = color ;
}


void ColorBadge::paintEvent(QPaintEvent *e)
{
    QPainter painter(this) ;
    painter.setBrush(QBrush(color));
    painter.drawRect(QRect(0,0,width()-1,height()-1));

    QWidget::paintEvent(e) ;
}
