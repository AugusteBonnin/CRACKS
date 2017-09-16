#ifndef MAPPAGEFACTORY_H
#define MAPPAGEFACTORY_H

#include "mappage.h"




class MainWindow;

class MapPageFactory
{
public:
    virtual MapPage * createMapPage(MainWindow*) = 0 ;
};

class SimpleMapPageFactory : public MapPageFactory
{
public :
    MapPage * createMapPage(MainWindow *mainWindow);
};

class DynamicMapPageFactory : public MapPageFactory
{
public :
    MapPage * createMapPage(MainWindow *mainWindow);
};

#endif // MAPPAGEFACTORY_H
