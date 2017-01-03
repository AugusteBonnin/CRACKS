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
    virtual QString getTitle() = 0 ;
    virtual QString getNextTooltip() = 0 ;
    virtual QString getPrevTooltip() = 0 ;
    virtual void nextPhase() = 0 ;
    virtual void prevPhase() = 0 ;
    virtual void reinit() = 0 ;

    void screenshot(QImage & image);
    ParamForm * paramForm;
    DocForm * docForm;

    bool computed ;
    bool initDone;
    };

#endif // PAGE_H
