#ifndef RoadMapWidget_H
#define RoadMapWidget_H


#include "mappage.h"
#include "mapwidget.h"
#include "scrollableopenglwidget.h"

#include <QObject>
#include <QOpenGLBuffer>

class RoadMapWidget : public MapWidget
{

    QOpenGLBuffer valid_roads_vbo ;
    QOpenGLBuffer * valid_roads_index_vbo ;
    QVector<QColor> & colors;
public:
    RoadMapWidget(MapPage * parent, QVector<QColor> &colors);
    void saveSVG(QString path) ;
protected:
    void paintGL();
    void resizeGL(int w, int h);
    void initializeGL();
};

#endif // RoadMapWidget_H
