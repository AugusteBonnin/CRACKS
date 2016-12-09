#include "openingscalewidget.h"
#include "colorbadge.h"

#include <QGridLayout>
#include <QPainter>
#include <QSettings>

OpeningScaleWidget::OpeningScaleWidget(QWidget *parent, QSize size) : QWidget(parent)
{
    imageSize = size ;

}

void OpeningScaleWidget::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);

    QSettings settings ;

    double  unitx  = settings.value("CropForm-UnitX").toDouble();
    double roundmm = width()*unitx/(2*settings.value("OpeningParamForm-Zoom",1).toInt()*imageSize.width());
    int w = width()>>1;

    QBrush black(Qt::black);
    QBrush white(Qt::white);
    QString str = QString("0") ;
    painter.drawText(-(painter.fontMetrics().width(str)>>1)+width()/2-w/2,16,str) ;
    str = "("+settings.value("Crop/UnitName","").toString()+")" ;
    painter.drawText(-(painter.fontMetrics().width(str)>>1)+width()*.875,24,str) ;
    for (int i = 0 ; i < 5 ; i++)
    {
        if (i%2)
        {
            painter.setBrush(black);
            QString str = QString("%1").arg(roundmm*(i+1)/5.0,0,'g',3);
            painter.drawText((i+1)*w/5-(painter.fontMetrics().width(str)>>1)+width()/2-w/2,16,str) ;
        }
        else
        {
            painter.setBrush(white);
            QString str = QString("%1").arg(roundmm*(i+1)/5.0,0,'g',3);
            painter.drawText((i+1)*w/5-(painter.fontMetrics().width(str)>>1)+width()/2-w/2,40,str) ;
        }
        painter.drawRect(i*w/5+width()/2-w/2,20,w/5,8);
    }
}
