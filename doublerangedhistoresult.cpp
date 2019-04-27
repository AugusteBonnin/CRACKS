#include "doublehistowidget.h"
#include "doublerangedhistoresult.h"
#include "rangeddoublehistowidget.h"
#include "ui_doublerangedhistoresult.h"
#include <QPainter>
#include <QTimer>
#include <math.h>

DoubleRangedHistoResult::DoubleRangedHistoResult(HistoPage *parent, QString name, QVector<double> &data, int index, double min, double max) :
    HistoResult(name,parent,index),
    ui(new Ui::DoubleRangedHistoResult)
{

    QStringList strl ;
    for (int i = 0 ; i < data.count() ; i++)
        strl << QString("%1").arg(data[i]) ;

    csv = strl.join(';');

    QGridLayout * grid = new QGridLayout;
    setLayout(grid);

    RangedDoubleHistoWidget * ihw = new RangedDoubleHistoWidget(this,data,min,max) ;
    grid->addWidget(ihw,1,2,5,8);

    QLabel * titre1 = new QLabel(this);

    titre1->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    titre1->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                    .arg(ihw->getDataCount())
                    .arg(ihw->getMaxCount())
                    .arg(100.0f*ihw->getMaxCount()/ihw->getDataCount()));
    grid->addWidget(titre1,0,0,1,2);

    int row = 1 ;
    for (double y = .9 ; y >0.0 ; y-=.2)
    {
        double x = pow (ihw->getMaxCount(),(10.0*y/9.0)-(1.0/9.0));
        QLabel * label = new QLabel(this);
        label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label->setText(QString("%1%").arg((100.0f*x/ihw->getDataCount()))) ;
        grid->addWidget(label,row,0,1,2);

        QLabel * label2 = new QLabel(this);
        label2->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        label2->setText(QString("%1").arg(x)) ;
        grid->addWidget(label2,row,10,1,2);

        row++;
    }

    QLabel * titre3 = new QLabel(this);
    titre3->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                        .arg(ihw->getDataCount())
                        .arg(ihw->getMaxCount())
                        .arg(100.0f*ihw->getMaxCount()/ihw->getDataCount()));
    grid->addWidget(titre3,0,10,1,2);

    QLabel * titre2 = new QLabel(this);
    titre2->setAlignment(Qt::AlignCenter);
    titre2->setWordWrap(true);
    titre2->setText(name);
    grid->addWidget(titre2,0,2,1,8);

    for (int i = 0 ; i < 5;i++)
    {
        QLabel * label = new QLabel(this);
        label->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label->setText(QString::number((i/4.0)*(ihw->getMax()-ihw->getMin())+ihw->getMin(),'f')) ;
        label->setFixedWidth(80);
        grid->addWidget(label,6,1+i*2,1,2);
    }


}

DoubleRangedHistoResult::~DoubleRangedHistoResult()
{
    delete ui;
}

