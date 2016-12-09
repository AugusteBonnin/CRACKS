#ifndef SKELFORM_H
#define SKELFORM_H

#include "page.h"
#include "skelwidget.h"

#include <QWidget>


class SkelPage : public Page
{
    Q_OBJECT

    SkelWidget * widget ;
    void saveSVG();
    void saveSHP();
public:
    explicit SkelPage(MainWindow *parent = 0);
    ~SkelPage();
    QString getTitle(){return tr("Squelettisation") ; }
    QString getPrevTooltip(){return tr("Revenir à la phase Contour") ; }
    QString getNextTooltip(){return tr("Passer à la phase Voies et Places") ; }
void nextPhase() ;
void prevPhase() ;
void reinit();
private slots:

private:
};

#endif // SKELFORM_H
