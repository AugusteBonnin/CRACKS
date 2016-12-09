#include "scalewidget.h"
#include "colorbadge.h"

#include <QGridLayout>
#include <QPainter>
#include <QSettings>

ScaleWidget::ScaleWidget(QWidget *parent) : QWidget(parent)
{


}

void ScaleWidget::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);

    QSettings settings ;

double  unitx  = settings.value("CropForm-UnitX").toDouble();
double roundmm = unitx*.5;
int w = width()*roundmm/unitx;

QBrush black(Qt::black);
QBrush white(Qt::white);
QString str = QString("0") ;
    painter.drawText(-(painter.fontMetrics().width(str)>>1)+width()/2-w/2,24,str) ;
 str = "("+settings.value("Crop/UnitName","").toString()+")" ;
        painter.drawText(-(painter.fontMetrics().width(str)>>1)+width()*.875,16,str) ;
for (int i = 0 ; i < 5 ; i++)
{
    if (i%2)
        painter.setBrush(black);
    else
        painter.setBrush(white);

    painter.drawRect(i*w/5+width()/2-w/2,0,w/5,8);
QString str = QString("%1").arg(roundmm*(i+1)/5.0,0,'g',3);
    painter.drawText((i+1)*w/5-(painter.fontMetrics().width(str)>>1)+width()/2-w/2,24,str) ;
}
}
