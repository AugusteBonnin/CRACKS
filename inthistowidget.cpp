#include "imagewidget.h"
#include "inthistowidget.h"

#include <QPainter>

IntHistoWidget::IntHistoWidget(QWidget *parent,QVector<int> & data) :
    QWidget(parent) , data(data)
{

    for (int i = 0 ; i  < data.count() ; i++)
    {
        Element e;
        e.value = data[i] ;
        e.index = i ;
        sorted.append(e);
    }

    std::sort(sorted.begin(),sorted.end());

    max_class_count = 0 ;
    int count ;
    int value ;
    int i = 0 ;

     while (i<sorted.count())
     {
         value = sorted[i].value ;
         count = 0 ;
         while ((i<sorted.count())&&(sorted[i].value==value))
        {
            count++;
            i++;
        }
        if (count>max_class_count)
        {
            max_class_count = count ;
        }
        class_count << count ;
        class_value << value ;
     }
 }

IntHistoWidget::~IntHistoWidget()
{
}

void IntHistoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QRectF rect(0,0,width(),height()) ;
    QBrush brush = painter.brush() ;
    QPen pen = painter.pen() ;

    rect.setWidth(rect.width()-1);
    rect.setHeight(rect.height()-1);
    painter.drawRect(rect);

    for (float x = 0 ; x <= 100.0f ; x+=10.0f)
    {
        pen = QPen(colorFor(1.0f-x*.01f));
        painter.setPen(pen);
        int y = (int)(x*height()*.01f);
        painter.drawLine(0,y,width(),y);

    }

    pen = QPen(Qt::black);
    painter.setPen(pen);

    double a = pow(max_class_count,1.0/9.0);
    double b = pow(max_class_count,10.0/9.0);

    for (int i = 0 ; i < class_count.count() ; i++)
    {
        if (class_count[i]==0) continue ;
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

QColor IntHistoWidget::colorFor(float alpha)
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
