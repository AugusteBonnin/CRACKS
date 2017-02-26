#ifndef HISTOPAGE_H
#define HISTOPAGE_H

#include "historesult.h"
#include "page.h"

#include <QGridLayout>
#include <QStackedWidget>
#include <QWidget>


class HistoPage : public Page
{
    Q_OBJECT
    QGridLayout * layout;

public:
    explicit HistoPage(MainWindow *parent = 0);
    ~HistoPage();
    QString getTitle() {return tr("Histogrammes (7/8)") ;}
    QString getPrevTooltip() {return tr("Revenir à la phase 6 : Voies et Places") ;}
    QString getNextTooltip() {return tr("Passer à la phase 8 : Cartes") ;}
    void nextPhase() ;
    void prevPhase() {} ;
    void reinit() ;

    QVector<HistoResult *> histoResults;
    QStackedWidget * container;
    void addHistoResult(HistoResult *result);
public slots:
    void setFont();
private slots:

private:

};

#endif // HISTOPAGE_H
