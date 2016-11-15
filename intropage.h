#ifndef INTROPAGE_H
#define INTROPAGE_H

#include "form.h"
#include "imagewidget.h"
#include "introdocform.h"
#include "introparamform.h"
#include "page.h"

class MainWindow ;

class IntroPage : public Page
{
    Q_OBJECT

public:
    explicit IntroPage(MainWindow *parent = 0);
    ~IntroPage();
    void setFile(QString file);
    QString getTitle() {return tr("Introduction") ;}
    QString getPrevTooltip() {return tr("Pas de phase Précédente") ;}
    QString getNextTooltip() {return tr("Passer à la phase Recadrage") ;}
    void nextPhase();
    void prevPhase();
    void reinit() ;
    QImage image ;
    void setImage(QImage & image);

private slots:
    void setWidgetImage();
private:
    ImageWidget * widget ;

};

#endif // INTROFORM_H
