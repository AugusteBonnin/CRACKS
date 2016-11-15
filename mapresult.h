#ifndef MAPRESULT_H
#define MAPRESULT_H

#include "mapwidget.h"
#include "result.h"
#include "scrollableopenglwidget.h"


namespace Ui {
class IntMapResult;
}

class MapPage;

class MapResult : public Result
{
    Q_OBJECT
protected:
    int index ;
    MapWidget * widget ;
    MainWindow * mainWindow;
public:
    MapResult(QString name, MapPage *parent, int index);
public slots:
    void saveImage();
    void saveSVG();
protected :
    Ui::IntMapResult *ui;

};

#endif // MAPRESULT_H
