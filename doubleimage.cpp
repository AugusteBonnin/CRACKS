#include "doubleimage.h"
#include <QDebug>

DoubleImage::DoubleImage(int width, int height)
{
    h = height ;
    w = width ;

    data = new double[w*h];

    minMaxComputed = false ;
}

DoubleImage::DoubleImage(DoubleImage & other)
{
    h = other.height() ;
    w = other.width() ;

    data = new double[w*h];

    for (int i = 0 ; i < w*h ; i++)
        data[i] = other.data[i] ;

    minMaxComputed = false ;
}

DoubleImage::DoubleImage(QImage image)
{
h = image.height() ;
w = image.width() ;

data = new double[w*h];

for (int i = 0 ; i < h ; i ++)
    for (int j = 0 ; j < w ; j ++)
    {
        uint p = image.pixel(j,i) ;
        setPixel(j,i,qBlue(p)+qGreen(p)+qRed(p));
    }

minMaxComputed = false ;
}

DoubleImage::~DoubleImage()
{
    delete data ;
}

void DoubleImage::computeMinMax()
{
        min = max = data[0] ;
        for (int i = 1 ; i  < w*h ; i++)
        {
            if (min>data[i])
                min = data[i] ;
            else if (max<data[i])
                max = data[i] ;
        }
        minMaxComputed = true ;
}

void DoubleImage::toQImage(QImage & result)
{
    computeMinMax();

    double inverse_of_range = 1.0/(max-min) ;

    for (int j = 0 ; j  < h ; j++)
        for (int i = 0 ; i  < w ; i++)
        {
            unsigned int grey = 255*(pixel(i,j)-min)*inverse_of_range ;
            result.setPixel(i,j,qRgba(grey,grey,grey,255));
            //qDebug() << grey ;
        }
}

DoubleImage *DoubleImage::substraction(DoubleImage *operand)
{
    if ((operand->width()!=w)||(operand->height()!=h))
    {
        qDebug() << "Different sizes in double image substraction";
        return NULL ;
    }

    DoubleImage * result = new DoubleImage(w,h);

    for (int j = 0  ; j < h ; j++)
        for (int i = 0 ; i < w ; i++)
            result->setPixel(i,j,pixel(i,j)-operand->pixel(i,j)) ;

    return result ;
}

void DoubleImage::invert()
{
    for (int i = 0 ; i < w*h ; i++)
        data[i] = - data[i] ;

    double tmp = min ;
    min = -max ;
    max = -tmp ;
}




