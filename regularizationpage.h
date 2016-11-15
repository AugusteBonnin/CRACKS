#ifndef REGULARIZATIONFORM_H
#define REGULARIZATIONFORM_H

#include "doubleimage.h"
#include "imagewidget.h"
#include "page.h"

#include <QWidget>

namespace Ui {
class RegularizationForm;
}

class RegularizationPage : public Page
{
    Q_OBJECT


    ImageWidget * originalWidget,*resultWidget ;
public:
    explicit RegularizationPage(MainWindow *parent = 0);
    ~RegularizationPage();
    QImage originalImage;
    DoubleImage * outputImage;
    void nextPhase();
    void prevPhase();
    void reinit() ;
    QString getTitle(){return tr("Régularisation");}
    QString getPrevTooltip(){return tr("Revenir à la phase Recadrage");}
    QString getNextTooltip(){return tr("Passer à la phase Ouverture");}


private slots:
    void on_compute_clicked();

private:
    bool computed ;
};

#endif // REGULARIZATIONFORM_H
