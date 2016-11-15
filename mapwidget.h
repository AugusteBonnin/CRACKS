#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "scrollableopenglwidget.h"

class MapPage;

class MapWidget : public ScrollableOpenGLWidget
{
public:
    MapWidget(MapPage *parent);
    virtual void saveSVG(QString path) = 0;
protected :
    MainWindow * mainWindow;
};

#endif // MAPWIDGET_H
