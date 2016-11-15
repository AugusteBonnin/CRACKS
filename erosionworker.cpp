#include "doubleimage.h"
#include "gaussianfilter.h"
#include "openingpage.h"
#include "erosionworker.h"
#include <math.h>

ErosionWorker::ErosionWorker(OpeningPage * parent, QRect &zone) : QRunnable() , QObject(parent)
{
    openingForm = parent ;
    this->zone = zone ;
}

void ErosionWorker::run()
{
    DoubleImage * image = openingForm->originalImage ;
    DoubleImage * minImage = openingForm->minImage ;

    QSettings settings;
    int radius = settings.value("OpeningParamForm-Radius",QVariant(10)).toInt() ;
    int squared_radius = radius * radius ;

    for (int j = zone.y() ; j < zone.y()+zone.height() ; j++)
    {
        for (int i = zone.x() ; i < zone.x()+zone.width() ; i++)
        {

            double min = INFINITY ;
            for (int k = -radius ; k <= radius ; k++)
                for (int l = -radius ; l <= radius ; l++)
                {
                    if ((i+l>=0)&&(i+l<minImage->width())&&
                            (j+k>=0)&&(j+k<minImage->height())&&
                            (k*k+l*l<=squared_radius)&&
                            (min>image->pixel(i+l,j+k)))
                        min = image->pixel(i+l,j+k) ;

                }
            minImage->setPixel(i,j,min);

        }
        emit setProgressIncrement(zone.width());
    }

}
