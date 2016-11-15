#ifndef SKELWIDGET_H
#define SKELWIDGET_H

#include "doubleimage.h"
#include "scrollableopenglwidget.h"

class SkelPage;

class SkelWidget : public ScrollableOpenGLWidget
{
    Q_OBJECT

    int getProgressMax();
public:
    SkelWidget(SkelPage *parent);
~SkelWidget() ;
    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
private:
    QVector<unsigned int> & connected_components;
    QVector<QVector<unsigned int> > & skel_children ;
    QVector<QPointF> & contour_vertices ;
    QVector<QPointF> & skel_points ;
    QVector<unsigned int> & next_point_index  ;
    QVector<float> & skel_distance ;
    QVector<unsigned char> & skel_colors ;
    QVector<unsigned int> & skel_indices ;

    inline bool isFramePoint(DoubleImage * image,QPointF & point)
        {
        return ((point.x()==0)||(point.x()==image->width()-1)||
                (point.y()==0)||(point.y()==image->height()-1)) ;
    }


    QRgb randColor();
    void exploreEdge(int first, int second);
    void explorePoint(int i);
public slots :
    void buildSkel();
signals :
    void progressIncrement(int);
private slots:
    void buildSkel2();
};

#endif // SKELWIDGET_H
