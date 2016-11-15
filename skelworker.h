#ifndef SkelWorker_H
#define SkelWorker_H

#include "doubleimage.h"
#include "scrollableopenglwidget.h"

#include <QRunnable>
#include <QThread>

class MainWindow ;

class SkelWorker : public QThread
{
    Q_OBJECT

    MainWindow * mainWindow;
public:
    SkelWorker(MainWindow * mainWindow);
    int getProgressMax();
~SkelWorker() ;
    // QOpenGLWidget interface

    void run();
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

    void crop();

signals :
    void progressIncrement(int);
};

#endif // SkelWorker_H
