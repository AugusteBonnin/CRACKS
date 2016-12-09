#ifndef OPENINGFORM_H
#define OPENINGFORM_H

#include "form.h"
#include "mainwindow.h"
#include "openingscalewidget.h"
#include "openingzoomwidget.h"
#include "roiwidget.h"

#include <QWidget>

class OpeningPage :  public Page
{
    Q_OBJECT

    QImage originalQImage,outputQImage;

    ROIWidget * originalWidget,*resultWidget;

public:
    OpeningZoomWidget * originalZoomWidget ,*resultZoomWidget ;

    explicit OpeningPage(MainWindow *parent = 0);
    ~OpeningPage();
    DoubleImage * originalImage , * outputImage , *minImage ;
    QString getTitle(){return tr("Ouverture");}
    QString getNextTooltip(){return tr("Passer à la phase Contour");}
    QString getPrevTooltip(){return tr("Revenir à la phase Régularisation");}

void prevPhase() {} ;
void nextPhase() ;
void reinit();

OpeningScaleWidget * scaleWidget;
public slots:
void preview();
private slots:
    void initImages();
    void compute_clicked();

    void invert_clicked();
};

#endif // OPENINGFORM_H
