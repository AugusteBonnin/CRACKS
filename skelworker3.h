#ifndef SkelWorker3_H
#define SkelWorker3_H

#include "doubleimage.h"
#include "scrollableopenglwidget.h"

#include <QRunnable>
#include <QThread>

class MainWindow ;

class SkelWorker3 : public QThread
{
    Q_OBJECT

    MainWindow * mainWindow;
public:
    SkelWorker3(MainWindow * mainWindow);
    int getProgressMax();
~SkelWorker3() ;
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
    QVector<bool> & skel_point_is_exit ;

    QMap<QPair<Vertex_handle,Vertex_handle>,int> middles_idxs;

    inline bool isFramePoint(DoubleImage * image,QPointF & point)
        {
        return ((fabs(point.x())<.01f)||(fabs(point.x()-image->width()+1)<.01f)||
                (fabs(point.y())<.01f)||(fabs(point.y()-image->height()+1)<.01f)) ;
    }


    int create_cendroid(const Vertex_handle &v0, const Vertex_handle &v1, const Vertex_handle &v2, const Vertex_handle &v3);
    int maibe_create_middle(const Vertex_handle &v1, const Vertex_handle &v2, bool point_is_exit);
    int create_cendroid(const Vertex_handle &v0, const Vertex_handle &v1, const Vertex_handle &v2);
signals :
    void progressIncrement(int);
};

#endif // SkelWorker3_H
