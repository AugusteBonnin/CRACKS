#include "histogramslider.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <math.h>

HistogramSlider::HistogramSlider(QWidget *parent) :
    QAbstractSlider(parent)
{

    setOrientation(Qt::Horizontal);
    //setTracking(true);
    setMinimum(0);
    setMaximum(10000);
    setValue(5000);

    bins = NULL ;
    mouseDragged = false ;

    last_width = -1 ;

    image = NULL ;
}

QSize HistogramSlider::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize HistogramSlider::sizeHint() const
{
    return QSize(640, 50);
}

void HistogramSlider::setImage(DoubleImage *image)
{
    this->image = image ;


    computeBins() ;
}

HistogramSlider::~HistogramSlider()
{
    if (bins) delete bins ;
}

void HistogramSlider::KMeans()
{

    double class0 = width()>>1 , class1 = (width()>>1 )+ 1 ;

    bool somethingChanged = true ;

    while(somethingChanged)
    {
        somethingChanged = false ;
        double new_class0 = 0 , new_class1 = 0;

        long class0_weight = 0 , class1_weight = 0 ;

        for (int i = 0 ; i < width() ; i ++)
        {
            if (fabs(i-class0)<fabs(i-class1))
            {
                new_class0 += i * bins[i] ;
                class0_weight += bins[i] ;
            }
            else
            {
                new_class1 += i * bins[i] ;
                class1_weight += bins[i] ;
            }
        }

        new_class0 /= class0_weight ;
        new_class1 /= class1_weight ;

        qDebug() << new_class0 << " center " << new_class1 ;
        qDebug() << class0_weight << " weigth " << class1_weight ;

        if ((new_class0!=class0)||(new_class1!=class1))
            somethingChanged = true ;

        class0 = new_class0 ;
        class1 = new_class1 ;

    }

    setValue(5000*(class0+class1)/width());


    update() ;
}

void HistogramSlider::computeBins()
{
    if (bins) delete bins ;

    bins = new int[width()];

    for (int i = 0 ; i < width() ; i++)
        bins[i] = 0 ;
    image->computeMinMax();

    double factor = (width()-1)/(image->getMax()-image->getMin());

    for (int j = 0 ; j < image->height() ; j++)
        for (int i = 0 ; i < image->width() ; i++)
            bins[(int)((image->pixel(i,j)-image->getMin())*factor)]++;

    max = bins[0] ;
    for (int i = 1 ; i  < width() ; i++)
    {
        if (max<bins[i])
            max = bins[i] ;
    }

}

void HistogramSlider::paintEvent(QPaintEvent *e)
{
    if ((last_width!=width())&&(image))
    {
        computeBins() ;
        last_width=width() ;
    }



    QPainter painter(this);

    painter.drawRect(0,0,width()-1,height()-1);

    int threshold = width()*value()*.0001 ;

    for (int i = 0 ; i < threshold ; i++)
        painter.drawLine(i,0,i,height()-1-height()*bins[i]/max);

    for (int i = threshold ; i < width() ; i++)
        painter.drawLine(i,height(),i,height()-height()*bins[i]/max);

}

void HistogramSlider::mouseMoveEvent(QMouseEvent *e)
{
    if (mouseDragged);
    setValue((int)(10000*e->x()/width()));
}

void HistogramSlider::mousePressEvent(QMouseEvent *e)
{
    mouseDragged = true ;
    setValue((int)(10000*e->x()/width()));
}


void HistogramSlider::mouseReleaseEvent(QMouseEvent *e)
{
    mouseDragged = false ;
    setSliderDown(false);
}



