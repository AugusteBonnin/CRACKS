#ifndef CONTOURWIDGET_H
#define CONTOURWIDGET_H

#include "doubleimage.h"
#include "scrollableopenglwidget.h"

#include <QOpenGLFunctions>
#include <QOpenGLTexture>



class ContourWidget : public ScrollableOpenGLWidget
{
    Q_OBJECT

    inline QPointF point_lerp(double x1,double y1,double z1,double x2,double y2,double z2,double treshold)
        {
            double factor = (treshold-z1)/(z2-z1);
            return QPointF(x1+(x2-x1)*factor,y1+(y2-y1)*factor);
        }

    inline bool isFramePoint(DoubleImage * image,QPointF & point)
        {
        return ((point.x()==0)||(point.x()==image->width()-1)||
                (point.y()==0)||(point.y()==image->height()-1)) ;
    }

    inline bool isCornerPoint(DoubleImage * image,QPointF & point)
        {
        return (((point.x()==0)||(point.x()==image->width()-1))&&((point.y()==0)||(point.y()==image->height()-1))) ;
        }

    QVector<QPolygonF> simplified_holes ;

public:
    ContourWidget(Form *parent);
~ContourWidget() ;
public slots:
    void updateThreshold(double treshold);
signals:

    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // QWidget interface
public:

};

#endif // CONTOURWIDGET_H
