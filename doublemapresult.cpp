#include "intcolorbadge.h"
#include "doublemapresult.h"
#include "junctionmapwidget.h"
#include "ui_intmapresult.h"
#include "mainwindow.h"
#include "roadmapwidget.h"
#include "openglscalewidget.h"

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

    for (int i = 0 ; i < values.count()-1 ; i++)
    {
        l+=values[i].length ;
        if (l>(parent->getMainWindow()->valid_roads_total_length*classes_limits.count()/(double)(LIMITS_COUNT)))
            classes_limits << values[i].value ;
    }
    classes_limits << values[values.count()-1].value ;

    for (int i = 0 ; i < data.count() ; i++)
    {
        int c = 1 ;
        for (;c<classes_limits.count()-1;c++)
        if (data[i]<classes_limits[c])
            break ;
        colors << colorFor((c-1)/(float)(classes_limits.count()-2)) ;
}
QLabel * label;
    for (int i = 0 ; i < classes_limits.count() -1; i++)
    {
        label = new QLabel(QString("%1").arg((float)classes_limits[i]),this);
        label->setMaximumWidth(96);
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->addWidget(new ColorBadge(this,colorFor(i/(float)(classes_limits.count()-2))));
    }
    label = new QLabel(QString("%1").arg((float)classes_limits[classes_limits.count()-1]),this);
    label->setMaximumWidth(96);
     ui->verticalLayout->addWidget(label);

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

    OpenGLScaleWidget *sw = new OpenGLScaleWidget(this,mainWindow->scale,mainWindow->openedQImage.size());
    sw->setFixedHeight(40);
    ui->gridLayout->addWidget(sw,2,1);
    QSettings settings;
    sw->setVisible(settings.value("Crop/Unit",false).toBool());

    connect(widget,SIGNAL(ScaleChanged(double)),sw,SLOT(ScaleChanged(double)));

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
    if (alpha<.25f)
    {
        color.setBlueF(1.0f-alpha*4.0f);
        color.setGreenF(alpha*4.0f);
        color.setRedF(.0f);
    }
    else
    if (alpha<.5f)
    {
        color.setBlueF(.0f);
        color.setGreenF(1.0f);
        color.setRedF((alpha-.25f)*4.0f);
    }
    else
    {
        color.setGreenF(1.0f-2.0f*(alpha-.5f));
        color.setRedF(1.0f);
        color.setBlueF(0);
    }

    return color;
}
