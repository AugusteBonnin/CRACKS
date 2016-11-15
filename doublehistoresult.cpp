#include "histoparamform.h"
#include "imagewidget.h"
#include "doublehistoresult.h"
#include "ui_doublerangedhistoresult.h"
#include "doublehistowidget.h"

#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <math.h>

DoubleHistoResult::DoubleHistoResult(HistoPage *parent, QString name, QVector<double> &data, int index) :
    page(parent),
    HistoResult(name,parent,index) ,
    ui(new Ui::DoubleRangedHistoResult)

{
    ui->setupUi(this);

    QStringList strl ;
    for (int i = 0 ; i < data.count() ; i++)
            strl << QString("%1").arg(data[i]) ;

    csv = strl.join(',');

    DoubleHistoWidget * dhw = new DoubleHistoWidget(this,data) ;
    dhw->setFixedSize(QSize(400,300));
    ui->titre1->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->titre1->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                       .arg(dhw->getDataCount())
                       .arg(dhw->getMaxCount())
                       .arg(100.0f*dhw->getMaxCount()/dhw->getDataCount()));
    ui->label_4->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->label_4->setText(QString("%1%").arg((87.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;
    ui->label_5->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
     ui->label_5->setText(QString("%1%").arg((62.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;
    ui->label_6->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->label_6->setText(QString("%1%").arg((37.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;
    ui->label_7->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
     ui->label_7->setText(QString("%1%").arg((12.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;

    ui->titre3->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                       .arg(dhw->getDataCount())
                       .arg(dhw->getMaxCount())
                       .arg(100.0f*dhw->getMaxCount()/dhw->getDataCount()));

    ui->label_13->setText(QString("%1%").arg((87.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;
    ui->label_14->setText(QString("%1%").arg((62.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;
    ui->label_15->setText(QString("%1%").arg((37.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;
    ui->label_16->setText(QString("%1%").arg((12.5f*dhw->getMaxCount()/dhw->getDataCount()))) ;

    ui->titre2->setAlignment(Qt::AlignCenter);
    ui->titre2->setWordWrap(true);
    ui->titre2->setText(name);


    ui->label_1->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    ui->label_1->setText(QString("%1").arg(dhw->getMin())) ;
    ui->label_1->setFixedWidth(80);

    ui->label_2->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    ui->label_2->setText(QString("%1").arg(dhw->getMin()+(dhw->getMax()-dhw->getMin())*.25f)) ;
    ui->label_2->setFixedWidth(80);

    ui->label_3->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    ui->label_3->setText(QString("%1").arg(dhw->getMin()+(dhw->getMax()-dhw->getMin())*.5f)) ;
    ui->label_3->setFixedWidth(80);

    ui->label_8->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    ui->label_8->setText(QString("%1").arg(dhw->getMin()+(dhw->getMax()-dhw->getMin())*.75f)) ;
    ui->label_8->setFixedWidth(80);

    ui->label_9->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    ui->label_9->setText(QString("%1").arg(dhw->getMax())) ;
    ui->label_9->setFixedWidth(80);


    ui->gridLayout->addWidget(dhw,1,2,4,8);

setFont();

QTimer::singleShot(0,this,SLOT(makeImage())) ;
}

void DoubleHistoResult::paintEvent(QPaintEvent *e)
{
    QPainter painter(this) ;
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRect(0,0,width()-1,height()-1));

    QWidget::paintEvent(e) ;
}

void DoubleHistoResult::setFont()
{
    QSettings settings;

    QFont font(settings.value("Font",QFont().toString()).toString()) ;
    int fontSize = settings.value("FontSize",12).toInt() ;
    font.setPixelSize(fontSize);

    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
    ui->label_6->setFont(font);
    ui->label_7->setFont(font);
    ui->label_8->setFont(font);
    ui->label_9->setFont(font);
    ui->label_1->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_13->setFont(font);
    ui->label_14->setFont(font);
    ui->label_15->setFont(font);
    ui->label_16->setFont(font);

    font.setPixelSize((int)(font.pixelSize()*1.5f));
    font.setBold(true);
    font.setUnderline(true);
    ui->titre2->setFont(font);

}


