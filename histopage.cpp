#include "histodocform.h"
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

HistoPage::HistoPage(MainWindow *parent) :
    Page(parent)
{


    //    ui->label_5->setFont(font);
//    ui->label_6->setFont(font);


    layout = new QGridLayout;
    container = new QStackedWidget(this) ;

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

    layout->addWidget(container) ;
setLayout(layout);

paramForm = new HistoParamForm(parent,this) ;
docForm = new HistoDocForm(parent);

paramForm->setEnabled(true);
mainWindow->action_next->setEnabled(true);

}

HistoPage::~HistoPage()
{
}

void HistoPage::nextPhase()
{
    settings.beginReadArray("Histo") ;
    for (int i = 0 ; i < histoResults.count() ; i++)
    {
        settings.setArrayIndex(i);
        if (settings.value("SaveJPG",false).toBool())
            histoResults[i]->saveImage();
        if (settings.value("SaveCSV",false).toBool())
            histoResults[i]->saveCSV();
       }
    settings.endArray();
}

void HistoPage::setFont() {
    for (int i = 0 ; i < histoResults.count() ; i++)
         histoResults[i]->setFont() ;

}

void HistoPage::reinit()
{
    if (histoResults[0])
    {
        IntHistoResult *tmp = new IntHistoResult(this,tr("Degré des places"),mainWindow->histoIntData[0],0);
        layout->replaceWidget(histoResults[0],tmp);
        delete histoResults[0] ;
        histoResults[0] = tmp ;
    }
    else
    {
    histoResults[0] = new IntHistoResult(this,tr("Degré des places"),mainWindow->histoIntData[0],0);
    layout->addWidget(histoResults[0]);
    }
}


