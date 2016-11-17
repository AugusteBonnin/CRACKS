#include "intcolorbadge.h"
#include "doublemapresult.h"
#include "junctionmapwidget.h"
#include "ui_intmapresult.h"
#include "mainwindow.h"
#include "roadmapwidget.h"

#include <QPainter>

DoubleMapResult::DoubleMapResult(MapPage *parent, QString titre, QVector<double> &data, int index, int type) :
    MapResult(titre,parent,index),
    data(data)
{

    QVector<Element> values;

    for (int i = 0 ; i < data.count() ; i++)
    {
        Element e ;
        e.value = data[i] ;
        e.length = parent->getMainWindow()->histoDoubleData[4][i] ;
        values << e ;
    }

    std::sort(values.begin(),values.end());

    double l = 0 ;

    for (int i = 0 ; i < values.count() ; i++)
    {
        l+=values[i].length ;
        if (l>=(parent->getMainWindow()->valid_roads_total_length*classes_limits.count()/(double)(CLASS_COUNT)))
            classes_limits << values[i].value ;
    }

    for (int i = 0 ; i < data.count() ; i++)
    {
        int c = 1 ;
        for (;c<classes_limits.count()-1;c++)
        if (data[i]<classes_limits[c])
            break ;
        colors << colorFor((c-1)/(float)(classes_limits.count()-2)) ;
}

    for (int i = 0 ; i < classes_limits.count() -1; i++)
    {
        ui->verticalLayout->addWidget(new QLabel(QString("%1").arg((float)classes_limits[i]),this));
        ui->verticalLayout->addWidget(new ColorBadge(this,colorFor(i/(float)(classes_limits.count()-2))));
    }
    ui->verticalLayout->addWidget(new QLabel(QString("%1").arg((float)classes_limits[classes_limits.count()-1]),this));

    ui->titre->setAlignment(Qt::AlignCenter);
    ui->titre->setText(titre);

    switch (type)
    {
    case 0 :
        widget = new JunctionMapWidget(parent,colors);
        break ;
    case 1 :
        widget = new RoadMapWidget(parent,colors);
        break;

    }
    ui->gridLayout->addWidget(widget,1,1);
}

DoubleMapResult::~DoubleMapResult()
{
    delete ui;
}


void DoubleMapResult::paintEvent(QPaintEvent *e)
{
    QPainter painter(this) ;
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRect(0,0,width()-1,height()-1));

    QWidget::paintEvent(e) ;
}

QColor DoubleMapResult::colorFor(float alpha)
{
    QColor color;
    if (alpha<.166f)
    {
        color.setBlueF(.5f*(1+6*alpha));
        color.setGreenF(0);
        color.setRedF(.5f-3*alpha);
    }
    else
    if (alpha<.5f)
    {
        color.setBlueF(1.0f-3.0f*(alpha-.166f));
        color.setGreenF(3.0f*(alpha-.166f));
        color.setRedF(0);
    }
    else
        if (alpha<.833f)

    {
        color.setGreenF(1.0f-3.0f*(alpha-.5f));
        color.setRedF(3.0f*(alpha-.5f));
        color.setBlueF(0);
    }
    else
        {
            color.setRedF(1.0f-4.0f*(alpha-.833f));
            color.setGreenF(2*(alpha-.833f));
            color.setBlueF(2*(alpha-.833f));
        }
    return color;
}
