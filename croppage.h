#ifndef CROPFORM_H
#define CROPFORM_H

#include "cropquadwidget.h"
#include "cropzoomwidget.h"
#include "page.h"
#include "scalewidget.h"

#include <QWidget>

namespace Ui {
class CropPage;
}

class CropPage : public Page
{
    Q_OBJECT

public:
    explicit CropPage(MainWindow *parent = 0);
    ~CropPage();
    QString getTitle(){return tr("Recadrage (1/8)");}
    QString getNextTooltip(){return tr("Passer à la phase 2 : Régularisation");}
    QString getPrevTooltip(){return tr("Revenir à la phase 0 : Introduction");}

    void nextPhase();
    void prevPhase();
    void reinit();

    CropQuadWidget * quadWidget ;
    ScaleWidget * scaleWidget;
public slots:
    void remap(QPolygon p);
    void RAZ();
    void makeRectangle();
private slots:
    void initImages();
private:
    CropZoomWidget * zoomWidget ;
    ImageWidget * resultWidget ;

};

#endif // CROPFORM_H
