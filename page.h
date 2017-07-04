#ifndef PAGE_H
#define PAGE_H

#include "docform.h"
#include "paramform.h"

class MainWindow ;

class Page : public Form
{
public:
    Page(MainWindow *parent);

    ParamForm * getParamForm(){ return paramForm ; }
    DocForm * getDocForm(){ return docForm ; }
    virtual QString getTitle() {return title ;}
    virtual QString getNextTooltip() {return nextTooltip ;}
    virtual QString getPrevTooltip() {return prevTooltip ;}
    virtual void nextPhase() = 0 ;
    virtual void prevPhase() = 0 ;
    virtual void reinit() = 0 ;

    void screenshot(QImage & image);
    ParamForm * paramForm;
    DocForm * docForm;
    QString title,nextTooltip,prevTooltip;

    bool computed ;
    bool initDone;
    void setParamForm(ParamForm *value);
    void setDocForm(DocForm *value);
};

#endif // PAGE_H
