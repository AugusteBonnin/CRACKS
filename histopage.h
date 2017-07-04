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
