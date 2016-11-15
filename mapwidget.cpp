#include "mappage.h"
#include "mapwidget.h"

MapWidget::MapWidget(MapPage * parent) : ScrollableOpenGLWidget(parent)
{
mainWindow = parent->getMainWindow() ;
}
