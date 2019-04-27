#include "mappage.h"
#include "mapparamform.h"
#include "mainwindow.h"
#include "intmapresult.h"
#include "junctionmapwidget.h"
#include "intrangedmapresult.h"
#include "doublemapresult.h"

#include <QTimer>

MapPage::MapPage(MainWindow *parent) :
    Page(parent)
{

    layout = new QGridLayout;
    container = new QStackedWidget(this) ;
    layout->addWidget(container) ;

setLayout(layout);




initDone = true ;
parent->setActionsEnabled(true);
}

MapPage::~MapPage()
{
}

void MapPage::addMapResult(MapResult*result)
{
    results << result ;
    result->setFixedHeight(600);
    container->addWidget(result) ;
}

void MapPage::nextPhase()
{
    QString path = title +"/" ;
   for (int i = 0 ; i < results.count() ; i++)
    {
        QString path2 = path+results[i]->getName() +"/" ;
        if (settings.value(path2+"SaveJPG",false).toBool())
            results[i]->saveImage(settings.value(path2+"Screenshot",true).toBool());
        //if (settings.value(path2+"SaveSVG",false).toBool())
            results[i]->saveSVG();
        }
    settings.endArray();
}

void MapPage::setFont() {
//    for (int i = 0 ; i < histoResults.count() ; i++)
//         histoResults[i]->setFont() ;

}

void MapPage::reinit()
{
    for (int i = 0 ; i < results.count() ; i++)
        container->removeWidget(results[i]);
    results.clear();

    results << new IntRangedMapResult(this,tr("Degré des places"),mainWindow->histoIntData[0],0,2,5,1,0) ;
    results << new IntRangedMapResult(this,tr("Degré second des places"),mainWindow->histoIntData[1],1,2,10,2,0) ;
    results << new IntRangedMapResult(this,tr("Degré troisième des places"),mainWindow->histoIntData[1],2,10,20,2,0) ;
    results << new DoubleMapResult(this,tr("Accessibilité des voies"),mainWindow->histoDoubleData[1],3,1);
    results << new IntRangedMapResult(this,tr("Degré des voies"),mainWindow->histoIntData[3],4,2,10,2,1);
    results << new DoubleMapResult(this,tr("Orthogonalité des voies"),mainWindow->histoDoubleData[2],5,1);
    results << new DoubleMapResult(this,tr("Espacement des voies"),mainWindow->histoDoubleData[3],6,1);
    results << new DoubleMapResult(this,tr("Longueur des voies"),mainWindow->histoDoubleData[4],7,1);
    results << new IntRangedMapResult(this,tr("Longueur topologique des voies"),mainWindow->histoIntData[4],8,1,10,2,1);

    for (int i = 0 ; i < results.count() ; i++)
    {
        results[i]->setFixedHeight(600);
        container->addWidget(results[i]) ;
    }

}


