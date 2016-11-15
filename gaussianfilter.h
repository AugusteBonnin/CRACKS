#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include "doubleimage.h"
#include <QImage>

class GaussianFilter
{

public:
    GaussianFilter(double radius);
    ~GaussianFilter();
    int half_width,width;
 double * data ;
    DoubleImage * convolute(QImage & image);
    };

#endif // GAUSSIANFILTER_H
