#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include "page.h"

#include <QWidget>

namespace Ui {
class ReportPage;
}

class ReportPage : public Page
{
    Q_OBJECT

public:
    explicit ReportPage(MainWindow *parent = 0);
    ~ReportPage();
    QString getTitle() {return tr("Bilan (8/8)") ;}
    QString getPrevTooltip() {return tr("Revenir à la phase 7 : Histogrammes") ;}
    QString getNextTooltip() {return tr("Passer à la phase 9 : Bilan") ;}
    void nextPhase() {}
    void prevPhase() {}
    void reinit() {}
private:
    Ui::ReportPage *ui;
};

#endif // REPORTPAGE_H
