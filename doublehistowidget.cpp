#include "imagewidget.h"
#include "doublehistowidget.h"

#include <QPainter>
#include <QSettings>
#include <math.h>

DoubleHistoWidget::DoubleHistoWidget(QWidget *parent,QVector<double> & data) :
    QWidget(parent) , data(data)
{


    min = INFINITY ;
    max = -INFINITY ;
    for (int i = 0 ; i < data.count() ; i++)
    {
        if (isnan(data[i])||isinf(data[i])) continue ;
        if (min>data[i]) min = data[i];
        if (max<data[i]) max = data[i];
    }

    QSettings settings;
    class_count = QVector<int>(settings.value("ClassCount",100).toInt(),0);
    max_class_count = 0 ;
    for (int i = 0 ; i < data.count() ; i++)
    {
        int idx ;
        if (isnan(data[i])||isinf(data[i])) continue ;

            idx= (int)(class_count.count()*(data[i]-min)/(max-min)) ;
            if (idx==class_count.count()) idx-- ;
            class_count[idx]++;
            max_class_count = qMax(max_class_count,class_count[idx]) ;


    }
}
DoubleHistoWidget::~DoubleHistoWidget()
{
}

double DoubleHistoWidget::mean(int i)
{
    QSettings settings;
    int radius = settings.value("HistoPage-MeanRadius",5).toInt() ;

    int count = 0 ;
    double total = 0 ;
    for (int j = qMax(0,i-radius) ; j <= qMin(class_count.count()-1,i+radius) ; j++)
    {
        count++ ;
        total += class_count[j] ;
    }

    return total/count ;
}
void DoubleHistoWidget::paintEvent(QPaintEvent *event)
{
    QSettings settings;
    class_count = QVector<int>(settings.value("ClassCount",100).toInt(),0);
    max_class_count = 0 ;
    for (int i = 0 ; i < data.count() ; i++)
    {
        if (isnan(data[i])||isinf(data[i])) continue ;

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

    for (int i = 0 ; i < class_count.count() ; i++)
    {
        int x,y;
        y = height()*(1-class_count[i]/(float)max_class_count) ;
        rect.setTop(y);
        rect.setLeft(width()*(i/(float)class_count.count()));
        rect.setWidth(width()/(float)class_count.count()-1) ;
        rect.setHeight((height()-1)-rect.top()) ;
        brush = QBrush(colorFor(i/(float)(class_count.count()-1)));
        painter.setBrush(brush);
        painter.drawRect(rect);

    }

    //mean
    int x,y;
    y = height()*(1-mean(0)/(float)max_class_count) ;
    x = width() * .5f/(float)class_count.count() ;
    for (int i = 0 ; i < class_count.count() ; i++)
    {
        int x2,y2;
        y2 = height()*(1-mean(i)/(float)max_class_count) ;
        x2 = width() * (i+.5f)/(float)class_count.count() ;
        painter.drawLine(x,y,x2,y2);
        x = x2 ;
        y = y2 ;
    }

}

QColor DoubleHistoWidget::colorFor(float alpha)
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
