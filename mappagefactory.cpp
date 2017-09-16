#include "intrangedmapresult.h"
#include "mappagefactory.h"
#include "doublemapresult.h"
#include "mapparamform.h"
#include "mainwindow.h"
#include "intmapresult.h"


MapPage *SimpleMapPageFactory::createMapPage(MainWindow * mainWindow)
{
    MapPage * mapPage = new MapPage(mainWindow) ;

    mapPage->addMapResult( new IntRangedMapResult(mapPage,QObject::tr("Degré des places"),mainWindow->histoIntData[0],0,2,5,0) );
    mapPage->addMapResult( new IntRangedMapResult(mapPage,QObject::tr("Degré second des places"),mainWindow->histoIntData[1],1,2,10,0) );
    mapPage->addMapResult( new IntRangedMapResult(mapPage,QObject::tr("Degré troisième des places"),mainWindow->histoIntData[2],2,10,20,0) );
    mapPage->addMapResult( new DoubleMapResult(mapPage,QObject::tr("Accessibilité des voies"),mainWindow->histoDoubleData[1],3,1));
    mapPage->addMapResult( new IntRangedMapResult(mapPage,QObject::tr("Degré des voies"),mainWindow->histoIntData[3],4,2,10,1));
    mapPage->addMapResult( new DoubleMapResult(mapPage,QObject::tr("Orthogonalité des voies"),mainWindow->histoDoubleData[2],5,1));
    mapPage->addMapResult( new DoubleMapResult(mapPage,QObject::tr("Espacement des voies"),mainWindow->histoDoubleData[3],6,1));
    mapPage->addMapResult( new DoubleMapResult(mapPage,QObject::tr("Longueur des voies"),mainWindow->histoDoubleData[4],7,1));
    mapPage->addMapResult( new IntRangedMapResult(mapPage,QObject::tr("Longueur topologique des voies"),mainWindow->histoIntData[4],8,1,10,1));


    mapPage->title =  QObject::tr("Cartes (8/8)") ;
    mapPage->prevTooltip = QObject::tr("Revenir à la phase 7 : Histogrammes") ;
    mapPage->nextTooltip = QObject::tr("Passer à la phase 9 : Bilan") ;

    mapPage->setParamForm( new MapParamForm(mainWindow,mapPage) );
    mapPage->setDocForm( new DocForm(mainWindow,QObject::tr("qrc:/docs/aide.html#Cartes")) );

    mapPage->paramForm->setEnabled(true);
    mainWindow->setActionsEnabled(true);


    return mapPage ;
}

MapPage *DynamicMapPageFactory::createMapPage(MainWindow *mainWindow)
{
    MapPage * mapPage = new MapPage(mainWindow) ;

    mapPage->addMapResult( new IntMapResult(mapPage,QObject::tr("Image de naissance des voies"),mainWindow->histoIntData[6],0,0) );
    mapPage->addMapResult( new IntMapResult(mapPage,QObject::tr("Image de mort des voies"),mainWindow->histoIntData[7],1,0) );
    mapPage->addMapResult( new IntMapResult(mapPage,QObject::tr("Durée de vie des voies"),mainWindow->histoIntData[8],2,0) );
    

    mapPage->title =  QObject::tr("Cartes dynamiques (2/2)") ;
    mapPage->prevTooltip = QObject::tr("Revenir à la phase 10 : Histogrammes dynamiques") ;
    mapPage->nextTooltip = QObject::tr("Quitter CRACKS!") ;

    mapPage->setParamForm( new MapParamForm(mainWindow,mapPage) );
    mapPage->setDocForm( new DocForm(mainWindow,QObject::tr("qrc:/docs/aide.html#Cartes")) );

    mapPage->paramForm->setEnabled(true);
    mainWindow->setActionsEnabled(true);


    return mapPage ;
}
