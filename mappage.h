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
    QString getTitle() {return tr("Cartes (8/8)") ;}
    QString getPrevTooltip() {return tr("Revenir à la phase 7 : Histogrammes") ;}
    QString getNextTooltip() {return tr("Passer à la phase 9 : Bilan") ;}
    void nextPhase() ;
    void prevPhase() {} ;
    void reinit() ;

    QVector<MapResult*> results;
QStackedWidget * container;
public slots:
    void setFont();
private slots:

private:

};

#endif // MapPage_H
