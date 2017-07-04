#ifndef MapPage_H
#define MapPage_H

#include "mapresult.h"
#include "page.h"

#include <QGridLayout>
#include <QStackedWidget>
#include <QWidget>


class MapPage : public Page
{
    Q_OBJECT
    QGridLayout * layout;

public:
    explicit MapPage(MainWindow *parent = 0);
    ~MapPage();
    void nextPhase() ;
    void prevPhase() {} ;
    void reinit() ;

    QVector<MapResult*> results;
    QStackedWidget * container;
    void addMapResult(MapResult *result);
public slots:
    void setFont();
private slots:

private:

};

#endif // MapPage_H
