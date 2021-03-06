#include "histogramlabel.h"
#include "histopage.h"
#include "histoparamform.h"
#include "imagewidget.h"
#include "ui_histopage.h"
#include "mainwindow.h"
#include "inthistoresult.h"
#include "inthistowidget.h"
#include "doublehistoresult.h"
#include "doublerangedhistoresult.h"
#include <QTimer>
#include <QDebug>

HistoPage::HistoPage(MainWindow *parent) :
    Page(parent)
{
    layout = new QGridLayout;
    container = new QStackedWidget(this) ;
layout->addWidget(container) ;
setLayout(layout);


initDone = true ;
parent->setActionsEnabled(true);

}



HistoPage::~HistoPage()
{
}

void
HistoPage::addHistoResult(HistoResult * result)
{
    histoResults << result ;
    //result->setFixedSize(QSize(800,400));
    container->addWidget(result) ;

}

void HistoPage::nextPhase()
{
    for (int i = 0 ; i < histoResults.count() ; i++)
    {
        container->setCurrentIndex(i);
             histoResults[i]->saveSVG();
            histoResults[i]->saveCSV();
       }

}

void HistoPage::setFont() {
    for (int i = 0 ; i < histoResults.count() ; i++)
         histoResults[i]->setFont() ;

}

void HistoPage::reinit()
{
    for (int i = 0 ; i < histoResults.count() ; i++)
        container->removeWidget(histoResults[i]);

    histoResults.clear();

    IntHistoResult * ihr =  new IntHistoResult(this,tr("Degré des places"),mainWindow->histoIntData[0],0) ;
    histoResults << ihr ;
    ihr->setFixedSize(QSize(800,400));

    IntHistoResult * second =  new IntHistoResult(this,tr("Degré second des places"),mainWindow->histoIntData[1],1) ;
    histoResults << second ;
    second->setFixedSize(QSize(800,400));

    IntHistoResult * third =  new IntHistoResult(this,tr("Degré troisième des places"),mainWindow->histoIntData[2],2) ;
    histoResults << third ;
    third->setFixedSize(QSize(800,400));

    DoubleRangedHistoResult * dhr =  new DoubleRangedHistoResult(this,tr("Différence d'angle aux places"),mainWindow->histoDoubleData[0],3,0,180) ;
    histoResults << dhr ;
    dhr->setFixedSize(QSize(800,400));

    DoubleHistoResult * closeness =  new DoubleHistoResult(this,tr("Accessibilité des voies"),mainWindow->histoDoubleData[1],4) ;
    histoResults << closeness ;
    closeness->setFixedSize(QSize(800,400));

    IntHistoResult * degree =  new IntHistoResult(this,tr("Degré des voies"),mainWindow->histoIntData[3],5) ;
    histoResults << degree ;
    degree->setFixedSize(QSize(800,400));

    DoubleHistoResult * ortho =  new DoubleHistoResult(this,tr("Orthogonalité des voies"),mainWindow->histoDoubleData[2],6) ;
    histoResults << ortho ;
    ortho->setFixedSize(QSize(800,400));

    DoubleHistoResult * spacing =  new DoubleHistoResult(this,tr("Espacement des voies"),mainWindow->histoDoubleData[3],7) ;
    histoResults << spacing ;
    spacing->setFixedSize(QSize(800,400));

    DoubleHistoResult * length_roads =  new DoubleHistoResult(this,tr("Longeur odométrique des voies"),mainWindow->histoDoubleData[4],8) ;
    histoResults << length_roads ;
    length_roads->setFixedSize(QSize(800,400));

    DoubleHistoResult * length_edges =  new DoubleHistoResult(this,tr("Longeur odométrique des arcs"),mainWindow->histoDoubleData[5],9) ;
    histoResults << length_edges ;
    length_edges->setFixedSize(QSize(800,400));

    IntHistoResult * length_topo =  new IntHistoResult(this,tr("Longeur topologique des voies"),mainWindow->histoIntData[4],10) ;
    histoResults << length_topo ;
    length_topo->setFixedSize(QSize(800,400));

      for (int i = 0  ; i < histoResults.count() ; i++)
        container->addWidget(histoResults[i]) ;

      qApp->processEvents();

      initDone = true ;
}


