#include "doubleimage.h"
#include "gaussianfilter.h"
#include "openingpage.h"
#include "dilatationworker.h"
#include <math.h>

DilatationWorker::DilatationWorker(OpeningPage * parent, QRect &zone) : QRunnable() , QObject(parent)
{
    openingForm = parent ;
    this->zone = zone ;
}

void DilatationWorker::run()
{
    DoubleImage * minImage = openingForm->minImage ;
    DoubleImage * outputImage = openingForm->outputImage ;

    QSettings settings ;
    int radius = settings.value("OpeningParamForm-Radius",QVariant(10)).toInt()  ;
    int squared_radius = radius * radius ;

    for (int j = zone.y() ; j < zone.y()+zone.height() ; j++)
    {
        for (int i = zone.x() ; i < zone.x()+zone.width() ; i++)
        {

            double max = -INFINITY ;
            for (int k = -radius ; k <= radius ; k++)
                for (int l = -radius ; l <= radius ; l++)
                {
                    if ((i+l>=0)&&(i+l<minImage->width())&&
                            (j+k>=0)&&(j+k<minImage->height())&&
                            (k*k+l*l<=squared_radius)&&
                            (max<minImage->pixel(i+l,j+k)))
                        max = minImage->pixel(i+l,j+k) ;

                }
            outputImage->setPixel(i,j,max);

        }
        emit setProgressIncrement(zone.width());
    }

}
