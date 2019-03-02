#include "imagewidget.h"
#include "rangeddoublehistowidget.h"

#include <QPainter>
#include <QSettings>
#include <math.h>
#include <QDebug>

RangedDoubleHistoWidget::RangedDoubleHistoWidget(QWidget *parent, QVector<double> & data, double min, double max) :
    QWidget(parent) , data(data)
{


    this->min = min ;
    this->max = max ;


    QSettings settings;
    class_count = QVector<int>(settings.value("ClassCount",100).toInt(),0);
    max_class_count = 0 ;
    for (int i = 0 ; i < data.count() ; i++)
    {
        if ((data[i]>=min)&&(data[i]<=max))
        {
            int idx = (int)(class_count.count()*(data[i]-min)/(max-min)) ;
            if (idx==class_count.count()) idx-- ;
            class_count[idx]++;
            max_class_count = qMax(max_class_count,class_count[idx]) ;
        }
        else qDebug() << "data out of range [" << min << "," << max << "] in RangedDoubleHistoWidget" ;
    }
}

RangedDoubleHistoWidget::~RangedDoubleHistoWidget()
{
}


void RangedDoubleHistoWidget::paintEvent(QPaintEvent *event)
{
    QSettings settings;
    class_count = QVector<int>(settings.value("ClassCount",100).toInt(),0);
    max_class_count = 0 ;
    for (int i = 0 ; i < data.count() ; i++)
    {
        int idx = (int)(class_count.count()*(data[i]-min)/(max-min)) ;
        if (idx==class_count.count()) idx-- ;
        class_count[idx]++;
        max_class_count = qMax(max_class_count,class_count[idx]) ;

    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QRectF rect(0,0,width(),height()) ;
    QBrush brush = painter.brush() ;
    QPen pen = painter.pen() ;

    rect.setWidth(rect.width()-1);
    rect.setHeight(rect.height()-1);
    painter.drawRect(rect);

    for (float x = 0 ; x <= 100.0f ; x+=12.5f)
    {
        pen = QPen(colorFor(1.0f-x*.01f));
        painter.setPen(pen);
        int y = (int)(x*height()*.01f);
        painter.drawLine(0,y,width(),y);
        int xx = (int)(width()*(1-x*.01f));
        painter.drawLine(xx,0,xx,height());

    }

    pen = QPen(Qt::black);
    painter.setPen(pen);

    double a = pow(max_class_count,1/9.0);
    double b = pow(max_class_count,10.0/9.0);

    for (int i = 0 ; i < class_count.count() ; i++)
    {
        int x,y;
        y = height()*(1-log(a*class_count[i])/log(b)) ;
        rect.setTop(y);
        rect.setLeft(width()*(i/(float)class_count.count()));
        rect.setWidth(width()/(float)class_count.count()-1) ;
        rect.setHeight((height()-1)-rect.top()) ;
        brush = QBrush(colorFor(i/(float)(class_count.count()-1)));
        painter.setBrush(brush);
        painter.drawRect(rect);

    }

}

QColor RangedDoubleHistoWidget::colorFor(float alpha)
{
    QColor color;
    if (alpha<.5f)
    {
        color.setBlueF(1.0f-2.0f*alpha);
        color.setGreenF(2.0f*alpha);
        color.setRedF(0);
    }
    else
    {
        color.setGreenF(2.0f-2.0f*alpha);
        color.setRedF(2.0f*(alpha-.5f));
        color.setBlueF(0);
    }
    return color;
}
