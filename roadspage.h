#ifndef ROADSFORM_H
#define ROADSFORM_H

#include "form.h"
#include "page.h"
#include "roadswidget.h"

#include <QWidget>

namespace Ui {
class RoadsForm;
}

class MainWindow;

class RoadsPage : public Page
{
    Q_OBJECT

    RoadsWidget * widget ;

    void saveSHP();
    void saveSVG();

public:
    explicit RoadsPage(MainWindow *parent = 0);
    ~RoadsPage();

    QString getTitle(){return tr("Voies et places");}
    QString getNextTooltip(){return tr("Passer à la phase Histogrammes");}
    QString getPrevTooltip(){return tr("Revenir à la phase Squelettisation");}

    void nextPhase();
    void prevPhase() {}
    void reinit() ;

public slots:
    void on_pushButton_2_clicked();

private:
};

#endif // ROADSFORM_H
