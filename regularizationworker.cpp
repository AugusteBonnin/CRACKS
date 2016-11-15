#include "doubleimage.h"
#include "gaussianfilter.h"
#include "regularizationpage.h"
#include "regularizationworker.h"

RegularizationWorker::RegularizationWorker(RegularizationPage * parent, QRect &zone) : QRunnable() , QObject(parent)
{
    regularizationForm = parent ;
    this->zone = zone ;
}

void RegularizationWorker::run()
{
    QImage & image = regularizationForm->originalImage ;

    QSettings settings ;

    DoubleImage *filterVertical = new DoubleImage(image.width(),zone.height());

    GaussianFilter g(settings.value("RegularizationForm-Radius",10).toInt());

    for (int i = 0 ; i < image.width() ; i ++)
    {
        for (int j = zone.y() ; j < zone.y()+zone.height() ; j ++)
        {
            double pixel = 0.0 ;
            for (int k = 0 ; k < g.width ; k++)
            {
                int l = j+k-g.half_width ;
                if (l < 0)
                    l = -l ;
                else
                if (l>=image.height())
                    l = 2*image.height() - 2 - l ;

                uint p = image.pixel(i,l) ;
                pixel += (qBlue(p)+qGreen(p)+qRed(p))*g.data[k] ;
            }
            filterVertical->setPixel(i,j-zone.y(),pixel);
        }
        emit setProgressIncrement(zone.height());
    }

    DoubleImage *result = regularizationForm->outputImage;

    for (int j = 0 ; j < zone.height() ; j ++)
     {
        for (int i = 0 ; i < result->width() ; i ++)
        {
            double pixel = 0.0 ;
            for (int k = 0 ; k < g.width ; k++)
            {
                int l = i+k-g.half_width ;
                if (l < 0)
                    l = -l ;
                else
                if (l>=filterVertical->width())
                    l = 2*filterVertical->width() - 2 - l ;

                pixel += filterVertical->pixel(l,j)*g.data[k] ;
            }
            result->setPixel(i,j+zone.y(),pixel);
        }
        emit setProgressIncrement(image.width());
    }

    delete filterVertical ;

}
