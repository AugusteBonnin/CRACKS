#include "histoparamform.h"
#include "imagewidget.h"
#include "logloghistoresult.h"
#include "ui_doublerangedhistoresult.h"
#include "logloghistowidget.h"

#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <math.h>

LogLogHistoResult::LogLogHistoResult(HistoPage *parent, QString name, QVector<double> &data, int index) :
    page(parent),
    HistoResult(name,parent,index)
{

    QStringList strl ;
    for (int i = 0 ; i < data.count() ; i++)
        strl << QString("%1").arg(data[i]) ;

    csv = strl.join(',');

    QGridLayout * grid = new QGridLayout;
    setLayout(grid);

    LogLogHistoWidget * dhw = new LogLogHistoWidget(this,data) ;
    //dhw->setFixedSize(QSize(400,300));
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

    QHBoxLayout * horizontal_layout = new QHBoxLayout;
    grid->addLayout(horizontal_layout,6,1,1,10);
    for (int i = 0 ; i < 5;i++)
    {
        QLabel * label = new QLabel(this);
        label->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label->setText(
                    QString::number(dhw->getMin()-1+pow(2,log2(dhw->getMax()-dhw->getMin()+1)*(i/4.0)),'f')) ;
        horizontal_layout->addWidget(label);
    }
    QTimer::singleShot(0,this,SLOT(makeImage())) ;
}

void LogLogHistoResult::paintEvent(QPaintEvent *e)
{
    QPainter painter(this) ;
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRect(0,0,width()-1,height()-1));

    QWidget::paintEvent(e) ;
}

void LogLogHistoResult::setFont()
{

}


