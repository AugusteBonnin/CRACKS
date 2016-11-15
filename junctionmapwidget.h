#ifndef JUNCTIONMAPWIDGET_H
#define JUNCTIONMAPWIDGET_H


#include "mappage.h"
#include "mapwidget.h"
#include "scrollableopenglwidget.h"

#include <QObject>
#include <QOpenGLBuffer>

class JunctionMapWidget : public MapWidget
{
    QVector<QOpenGLBuffer*> junctions_vbos ;
    QVector<QColor> &colors;

public:
    JunctionMapWidget(MapPage * parent, QVector<QColor> &colors);
    void saveSVG(QString path);
protected:
    void paintGL();
    void resizeGL(int w, int h);
    void initializeGL();
};

#endif // JUNCTIONMAPWIDGET_H
