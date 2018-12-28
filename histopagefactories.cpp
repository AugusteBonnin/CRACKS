#include "histopagefactories.h"
#include "histopage.h"
#include "mainwindow.h"
#include "histoparamform.h"
#include <QAction>

HistoPage *
SimpleHistoPageFactory::createHistoPage(MainWindow*mainWindow)
{
    HistoPage * histoPage = new HistoPage(mainWindow);

    IntHistoResult * ihr =  new IntHistoResult(histoPage,QObject::tr("Degré des places"),mainWindow->histoIntData[0],0) ;
    histoPage->addHistoResult(ihr);

    IntHistoResult * second =  new IntHistoResult(histoPage,QObject::tr("Degré second des places"),mainWindow->histoIntData[1],1) ;
    histoPage->addHistoResult(second);

    IntHistoResult * third =  new IntHistoResult(histoPage,QObject::tr("Degré troisième des places"),mainWindow->histoIntData[2],2) ;
    histoPage->addHistoResult(third);

    DoubleRangedHistoResult * dhr =  new DoubleRangedHistoResult(histoPage,QObject::tr("Différence d'angle aux places"),mainWindow->histoDoubleData[0],3,0,180) ;
    histoPage->addHistoResult(dhr);

    DoubleHistoResult * closeness =  new DoubleHistoResult(histoPage,QObject::tr("Accessibilité des voies"),mainWindow->histoDoubleData[1],4) ;
    histoPage->addHistoResult(closeness);

    IntHistoResult * degree =  new IntHistoResult(histoPage,QObject::tr("Degré des voies"),mainWindow->histoIntData[3],5) ;
    histoPage->addHistoResult(degree);

    DoubleHistoResult * ortho =  new DoubleHistoResult(histoPage,QObject::tr("Orthogonalité des voies"),mainWindow->histoDoubleData[2],6) ;
    histoPage->addHistoResult(ortho);

    DoubleHistoResult * spacing =  new DoubleHistoResult(histoPage,QObject::tr("Espacement des voies"),mainWindow->histoDoubleData[3],7) ;
    histoPage->addHistoResult(spacing);


    DoubleHistoResult * length_roads =  new DoubleHistoResult(histoPage,QObject::tr("Longeur odométrique des voies"),mainWindow->histoDoubleData[4],8) ;
    histoPage->addHistoResult(length_roads);

    DoubleHistoResult * length_edges =  new DoubleHistoResult(histoPage,QObject::tr("Longeur odométrique des arcs"),mainWindow->histoDoubleData[5],9) ;
    histoPage->addHistoResult(length_edges);

    IntHistoResult * length_topo =  new IntHistoResult(histoPage,QObject::tr("Longeur topologique des voies"),mainWindow->histoIntData[4],10) ;
    histoPage->addHistoResult(length_topo);

    histoPage->title = QObject::tr("Histogrammes (7/8)") ;
    histoPage->prevTooltip = QObject::tr("Revenir à la phase 6 : Voies et Places") ;
    histoPage->nextTooltip = QObject::tr("Passer à la phase 8 : Cartes") ;

    histoPage->setParamForm( new HistoParamForm(mainWindow,histoPage)) ;
    histoPage->setDocForm( new DocForm(mainWindow,QObject::tr("qrc:/docs/aide.html#Histogrammes")) );

    histoPage->paramForm->setEnabled(true);
    mainWindow->next->setEnabled(true);

    return histoPage ;

}

HistoPage *
DynamicHistoPageFactory::createHistoPage(MainWindow*mainWindow)
{
    HistoPage * histoPage = new HistoPage(mainWindow);

    IntHistoResult * birth =
            new IntHistoResult(histoPage,QObject::tr("Image de naissance des voies"),mainWindow->histoIntData[6],10) ;
    histoPage->addHistoResult(birth);

    IntHistoResult * death =
            new IntHistoResult(histoPage,QObject::tr("Image de mort des voies"),mainWindow->histoIntData[7],10) ;
    histoPage->addHistoResult(death);

    IntHistoResult * lifespan =
            new IntHistoResult(histoPage,QObject::tr("Durée de vie des voies"),mainWindow->histoIntData[8],10) ;
    histoPage->addHistoResult(lifespan);

    histoPage->title = QObject::tr("Histogrammes dynamiques (1/2)") ;
    histoPage->prevTooltip = QObject::tr("Revenir à la phase 9 : Bilan") ;
    histoPage->nextTooltip = QObject::tr("Passer à la phase 2 : Cartes dynamiques") ;


    histoPage->setParamForm( new HistoParamForm(mainWindow,histoPage)) ;
    histoPage->setDocForm( new DocForm(mainWindow,QObject::tr("qrc:/docs/aide.html#Histogrammes")) );

    histoPage->paramForm->setEnabled(true);
    mainWindow->next->setEnabled(true);

    return histoPage ;

}
