#include "gaussianfilter.h"
#include <math.h>

#include <QDebug>

GaussianFilter::GaussianFilter(double radius)
{

    half_width = ceil(radius);
    width = 1 + 2 * half_width ;

    data = new double[width] ;

    double sigma = radius / 3.0 ;

    double negate_of_inverse_of_two_sigma_sqare = -1/(2*sigma*sigma) ;

    //setting up unnormalized values
    double sum  = 0 ;

    //left and right sides
    for (int i = 0 ; i < half_width ; i++)
    {
        double v = (half_width-i)*(half_width-i)*negate_of_inverse_of_two_sigma_sqare;
        data[i] = data[width-1-i] = exp(v) ;
        sum += data[i] ;
        sum += data[i] ;
    }

    //central element
    data[half_width] = 1.0 ;
    sum += 1.0 ;

    //normalizing
    sum = 1/sum ;
    for (int i = 0 ; i < width ; i++)
    {
        data[i] *= sum ;
        //qDebug() << data[i] ;
    }

}

GaussianFilter::~GaussianFilter()
{
    delete data ;
}

DoubleImage *GaussianFilter::convolute(QImage &image)
{
    if ((width>=image.width())||(width>=image.height()))
        return new DoubleImage(1,1) ;

    DoubleImage *filterVertical = new DoubleImage(image.width(),image.height());

    for (int i = 0 ; i < filterVertical->width() ; i ++)
        for (int j = 0 ; j < filterVertical->height() ; j ++)
        {
            double pixel = 0.0 ;
            for (int k = 0 ; k < width ; k++)
            {
                int l = j+k-half_width ;
                if (l < 0)
                    l = -l ;
                else
                if (l>=filterVertical->height())
                    l = 2*filterVertical->height() - 2 - l ;

                uint p = image.pixel(i,l) ;
                pixel += (qBlue(p)+qGreen(p)+qRed(p))*data[k] ;
            }
            filterVertical->setPixel(i,j,pixel);
        }

    DoubleImage *result = new DoubleImage(image.width(),image.height());

    for (int j = 0 ; j < result->height() ; j ++)
        for (int i = 0 ; i < result->width() ; i ++)
        {
            double pixel = 0.0 ;
            for (int k = 0 ; k < width ; k++)
            {
                int l = i+k-half_width ;
                if (l < 0)
                    l = -l ;
                else
                if (l>=filterVertical->width())
                    l = 2*filterVertical->width() - 2 - l ;

                pixel += filterVertical->pixel(l,j)*data[k] ;
            }
            result->setPixel(i,j,pixel);
        }

    delete filterVertical ;

    return result ;
}
