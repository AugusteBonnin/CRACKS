#ifndef CONTOURFORM_H
#define CONTOURFORM_H

#include "contourwidget.h"
#include "form.h"
#include "histogramslider.h"
#include "page.h"

#include <QWidget>

namespace Ui {
class ContourForm;
}

class MainWindow ;

class ContourPage : public Page
{
    Q_OBJECT

    ContourWidget * widget;

    QImage originalQImage ;

    void saveSVG() ;
    void saveSHP() ;
public:
    DoubleImage * originalImage ;

    explicit ContourPage(MainWindow *parent = 0);
    ~ContourPage();
    QString getTitle(){return tr("Contour (4/8)");}
    QString getPrevTooltip(){return tr("Revenir à la phase Ouverture");}
    QString getNextTooltip(){return tr("Passer à la phase Squelettisation");}
void nextPhase();
void prevPhase();
void reinit();

public slots:
    void updateThreshold(int);

private slots:
    void setImages();

signals :
    void invertSignal();
    void kMeansSignal();

private:

};

#endif // CONTOURFORM_H
