#include "histoparamform.h"
#include "imagewidget.h"
#include "inthistoresult.h"
#include "inthistowidget.h"
#include "ui_inthistowidget.h"

#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <math.h>

IntHistoResult::IntHistoResult(HistoPage *parent, QString name, QVector<int> &data, int index) :
    page(parent),
    HistoResult(name,parent,index) ,
    ui(new Ui::IntHistoWidget)

{

    ui->setupUi(this);

    QStringList strl ;
    for (int i = 0 ; i < data.count() ; i++)
            strl << QString("%1").arg(data[i]) ;

    csv = strl.join(',');

    IntHistoWidget * ihw = new IntHistoWidget(this,data) ;
    ihw->setFixedSize(QSize(400,300));
    ui->label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->label->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                       .arg(ihw->getDataCount())
                       .arg(ihw->getMaxCount())
                       .arg(100*ihw->getMaxCount()/ihw->getDataCount()));
    ui->label_3->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->label_3->setText(QString("%1%").arg(round(87.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
    ui->label_4->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
     ui->label_4->setText(QString("%1%").arg(round(62.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
    ui->label_5->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->label_5->setText(QString("%1%").arg(round(37.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
    ui->label_6->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
     ui->label_6->setText(QString("%1%").arg(round(12.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
    ui->label_7->setAlignment(Qt::AlignRight|Qt::AlignTop);
    ui->label_7->setText("") ;
    ui->label_8->setText(tr("Effectif : %1\nMax : %2 (%3%)")
                       .arg(ihw->getDataCount())
                       .arg(ihw->getMaxCount())
                       .arg(100*ihw->getMaxCount()/ihw->getDataCount()));
    ui->label_9->setText(QString("%1%").arg(round(87.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
   ui->label_10->setText(QString("%1%").arg(round(62.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
    ui->label_11->setText(QString("%1%").arg(round(37.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
    ui->label_12->setText(QString("%1%").arg(round(12.5f*ihw->getMaxCount()/ihw->getDataCount()))) ;
    ui->label_13->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    ui->label_13->setText(QString()) ;

    ui->titre->setAlignment(Qt::AlignCenter);
    ui->titre->setWordWrap(true);
    ui->titre->setText(name);

    QVector<int> & values = ihw->getValues() ;

    for (int i = 0 ; i < values.count() ;i++)
    {
        QLabel *label = new QLabel(QString("%1").arg(values[i])) ;
        label->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        ui->horizontalLayout->addWidget(label);
        horizontalLabels << label ;
    }

    ui->gridLayout->addWidget(ihw,1,1,4,1);

setFont();


         QTimer::singleShot(0,this,SLOT(makeImage())) ;
}

void IntHistoResult::paintEvent(QPaintEvent *e)
{
    QPainter painter(this) ;
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRect(0,0,width()-1,height()-1));

    QWidget::paintEvent(e) ;
}

QSize IntHistoResult::sizeHint() const{return QSize(640,480);}
QSize IntHistoResult::minimumSizeHint() const{return QSize(640,480);}
QSize IntHistoResult::maximumSizeHint() const{return QSize(640,480);}

void IntHistoResult::setFont()
{
    QSettings settings;

    QFont font(settings.value("Font",QFont().toString()).toString()) ;
    int fontSize = settings.value("FontSize",12).toInt() ;
    font.setPixelSize(fontSize);

    ui->label->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
    ui->label_6->setFont(font);
    ui->label_8->setFont(font);
    ui->label_9->setFont(font);
    ui->label_10->setFont(font);
    ui->label_11->setFont(font);
    ui->label_12->setFont(font);

    for (int i = 0 ; i < horizontalLabels.count() ; i++)
        horizontalLabels[i]->setFont(font);

    font.setPixelSize((int)(font.pixelSize()*1.5f));
    font.setBold(true);
    font.setUnderline(true);
    ui->titre->setFont(font);

}


