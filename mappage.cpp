#include "mapdocform.h"
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

    results << new IntRangedMapResult(this,tr("Degré des places"),mainWindow->histoIntData[0],0,2,5,0) ;
    results << new IntRangedMapResult(this,tr("Degré second des places"),mainWindow->histoIntData[1],1,2,10,0) ;
    results << new IntRangedMapResult(this,tr("Degré troisième des places"),mainWindow->histoIntData[1],2,10,20,0) ;
    results << new DoubleMapResult(this,tr("Accessibilité des voies"),mainWindow->histoDoubleData[1],3,1);
    results << new IntRangedMapResult(this,tr("Degré des voies"),mainWindow->histoIntData[3],4,2,10,1);
    results << new DoubleMapResult(this,tr("Orthogonalité des voies"),mainWindow->histoDoubleData[2],5,1);
    results << new DoubleMapResult(this,tr("Espacement des voies"),mainWindow->histoDoubleData[3],6,1);
    results << new DoubleMapResult(this,tr("Longueur des voies"),mainWindow->histoDoubleData[4],7,1);
    results << new IntRangedMapResult(this,tr("Longueur topologique des voies"),mainWindow->histoIntData[4],8,1,10,1);



setLayout(layout);

for (int i = 0 ; i < results.count() ; i++)
{
    results[i]->setFixedHeight(600);
    container->addWidget(results[i]) ;
}

paramForm = new MapParamForm(parent,this) ;
docForm = new MapDocForm(parent);

mainWindow->setActionsEnabled(true);

initDone = true ;

}

MapPage::~MapPage()
{
}

void MapPage::nextPhase()
{
    settings.beginReadArray("Maps") ;
    for (int i = 0 ; i < results.count() ; i++)
    {
        settings.setArrayIndex(i);
        if (settings.value("SaveJPG",false).toBool())
            results[i]->saveImage();
        if (settings.value("SaveSVG",false).toBool())
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

    results << new IntRangedMapResult(this,tr("Degré des places"),mainWindow->histoIntData[0],0,2,5,0) ;
    results << new IntRangedMapResult(this,tr("Degré second des places"),mainWindow->histoIntData[1],1,2,10,0) ;
    results << new IntRangedMapResult(this,tr("Degré troisième des places"),mainWindow->histoIntData[1],2,10,20,0) ;
    results << new DoubleMapResult(this,tr("Accessibilité des voies"),mainWindow->histoDoubleData[1],3,1);
    results << new IntRangedMapResult(this,tr("Degré des voies"),mainWindow->histoIntData[3],4,2,10,1);
    results << new DoubleMapResult(this,tr("Orthogonalité des voies"),mainWindow->histoDoubleData[2],5,1);
    results << new DoubleMapResult(this,tr("Espacement des voies"),mainWindow->histoDoubleData[3],6,1);
    results << new DoubleMapResult(this,tr("Longueur des voies"),mainWindow->histoDoubleData[4],7,1);
    results << new IntRangedMapResult(this,tr("Longueur topologique des voies"),mainWindow->histoIntData[4],8,1,10,1);

    for (int i = 0 ; i < results.count() ; i++)
    {
        results[i]->setFixedHeight(600);
        container->addWidget(results[i]) ;
    }

}


