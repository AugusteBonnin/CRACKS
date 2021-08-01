#include "histoparamform.h"
#include "imagewidget.h"
#include "inthistoresult.h"
#include "inthistowidget.h"
#include "ui_inthistowidget.h"

#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <math.h>

IntHistoResult::IntHistoResult(HistoPage *parent, QString name, QVector<uint32_t> &data, uint32_t index) :
    page(parent),
    HistoResult(name,parent,index) ,
    ui(new Ui::IntHistoWidget)

{


    QStringList strl ;
    for (int i = 0 ; i < data.count() ; i++)
        strl << QString("%1").arg(data[i]) ;

    csv = strl.join('\n');

    QGridLayout * grid = new QGridLayout;
    setLayout(grid);

    IntHistoWidget * dhw = new IntHistoWidget(this,data) ;
    grid->addWidget(dhw,1,2,5,8);

    QLabel * titre1 = new QLabel(this);

    titre1->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    titre1->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                    .arg(dhw->getDataCount())
                    .arg(dhw->getMaxCount())
                    .arg(100.0f*dhw->getMaxCount()/dhw->getDataCount()));
    grid->addWidget(titre1,0,0,1,2);

    int row = 1 ;
    for (double y = .9 ; y >0.0 ; y-=.2)
    {
        double x = pow (dhw->getMaxCount(),(10.0*y/9.0)-(1.0/9.0));
        QLabel * label = new QLabel(this);
        label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label->setText(QString("%1%").arg((100.0f*x/dhw->getDataCount()))) ;
        grid->addWidget(label,row,0,1,2);

        QLabel * label2 = new QLabel(this);
        label2->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        label2->setText(QString("%1").arg(x)) ;
        grid->addWidget(label2,row,10,1,2);

        row++;
    }

    QLabel * titre3 = new QLabel(this);
    titre3->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                        .arg(dhw->getDataCount())
                        .arg(dhw->getMaxCount())
                        .arg(100.0f*dhw->getMaxCount()/dhw->getDataCount()));
    grid->addWidget(titre3,0,10,1,2);

    QLabel * titre2 = new QLabel(this);
    titre2->setAlignment(Qt::AlignCenter);
    titre2->setWordWrap(true);
    titre2->setText(name);
    grid->addWidget(titre2,0,2,1,8);

    QTimer::singleShot(0,this,SLOT(makeImage())) ;
    QVector<int> & values = dhw->getValues() ;

    QHBoxLayout * horizontalLayout = new QHBoxLayout;
    grid->addLayout(horizontalLayout,6,2,1,8);
    for (int i = 0 ; i < values.count() ;i++)
    {
        QLabel *label = new QLabel(QString("%1").arg(values[i])) ;
        label->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        horizontalLayout->addWidget(label);
    }
}


