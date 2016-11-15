#ifndef DOUBLEIMAGE_H
#define DOUBLEIMAGE_H

#include <QImage>

class DoubleImage
{
    unsigned int w,h;
    double *data ;
    double min,max ;
    bool minMaxComputed ;
public:
    DoubleImage(int width, int height);
    DoubleImage(QImage image);
    ~DoubleImage();
    inline double *getData(){return data;}
    inline double pixel(int x , int y){return data[y*w+x] ;}
    inline double subPixel(double x , double y){
        if ((x>=0)&&(x<w)&&(y>=0)&&(y<h))
        {
        int i = (int) x ;
        int j = (int) y ;
        double a = pixel(i,j);
        double b = pixel(i+1,j);
        double c = pixel(i+1,j+1);
        double d = pixel(i,j+1);
        double ab = a*(i+1-x)+b*(x-i);
        double cd = d*(i+1-x)+c*(x-i);

        return ab*(j+1-y)+cd*(y-j);
        }
        else return min;
    }
    inline void setPixel(int x,int y,double value){data[y*w+x] = value ;}
    inline unsigned int width() {return w;}
    inline unsigned int height() {return h;}
    inline double getMin() {return min;}
    inline double getMax() {return max;}
    void computeMinMax();
    void toQImage(QImage & result) ;
    DoubleImage * substraction(DoubleImage * operand);


    void invert();

    DoubleImage(DoubleImage &other);
};

#endif // DOUBLEIMAGE_H
