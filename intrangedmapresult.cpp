#include "intcolorbadge.h"
#include "intrangedmapresult.h"
#include "junctionmapwidget.h"
#include "openglscalewidget.h"
#include "roadmapwidget.h"
#include "ui_intmapresult.h"
#include "mainwindow.h"

#include <QPainter>

IntRangedMapResult::IntRangedMapResult(MapPage *parent, QString titre, QVector<int> &data,int index, int min, int max, int type) :
    MapResult(titre,parent,index) ,
    data(data)
{

    QSet<int> keys;

    for (int i = 0 ; i  < data.count() ; i++)
    {
        keys.insert(data[i]);
    }

    class_value = keys.toList() ;
    std::sort(class_value.begin(),class_value.end());

    QHash<int,int> classForValue;
    for (int i = 0 ; i < class_value.count() ; i++)
        classForValue.insert(class_value[i],i) ;

    for (int i = 0 ; i < data.count() ; i++)
        colors << colorFor((data[i]-min)/(float)(max-min)) ;

    for (int i = 0 ; i < class_value.count() ; i++)
    {
        if (class_value[i]-min>0)
        {
            if (class_value[i]-max<0)
                ui->verticalLayout->addWidget(new IntColorBadge(this,
                                                                colorFor((class_value[i]-min)/(float)(max-min)),
                                                                QString("%1").arg(class_value[i])
                                                                )
                                              );
            else if (class_value[i]==max)
                ui->verticalLayout->addWidget(new IntColorBadge(this,
                                                                colorFor((class_value[i]-min)/(float)(max-min)),
                                                                tr("%1 et +").arg(class_value[i])
                                                                )
                                              );
        }
        else if (class_value[i]==min)
            ui->verticalLayout->addWidget(new IntColorBadge(this,
                                                            colorFor((class_value[i]-min)/(float)(max-min)),
                                                            tr("%1 et -").arg(class_value[i])
                                                            )
                                          );
    }
    ui->titre->setAlignment(Qt::AlignCenter);
    ui->titre->setText(titre);

    switch (type)
    {
    case 0 :
        widget = new JunctionMapWidget(parent,colors);
        break ;
    case 1 :
        widget = new RoadMapWidget(parent,colors);
        break ;
    }
    ui->gridLayout->addWidget(widget,1,1);

    OpenGLScaleWidget *sw = new OpenGLScaleWidget(this,mainWindow->scale,mainWindow->openedQImage.size());
    sw->setFixedHeight(40);
    ui->gridLayout->addWidget(sw,2,1);
    QSettings settings;
    sw->setVisible(settings.value("Crop/Unit",false).toBool());

    connect(widget,SIGNAL(ScaleChanged(double)),sw,SLOT(ScaleChanged(double)));

}

IntRangedMapResult::~IntRangedMapResult()
{
    delete ui;
}


void IntRangedMapResult::paintEvent(QPaintEvent *e)
{
    QPainter painter(this) ;
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRect(0,0,width()-1,height()-1));

    QWidget::paintEvent(e) ;
}

QColor IntRangedMapResult::colorFor(float alpha)
{
    QColor color;
    if (alpha<.0f)
    {
        color.setBlueF(1.0f);
        color.setGreenF(0);
        color.setRedF(0);
    }
    else   if (alpha<.25f)
    {
        color.setBlueF(1.0f);
        color.setGreenF(4*alpha);
        color.setRedF(0);
    }
    else
        if (alpha<.5f)
        {
            color.setBlueF(1.0f-4*(alpha-.25f));
            color.setGreenF(1.0f);
            color.setRedF(0);
        }
        else
            if (alpha<.75f)

            {
                color.setGreenF(1.0f);
                color.setRedF(4.0f*(alpha-.5f));
                color.setBlueF(0);
            }
            else if (alpha<1.0f)
            {
                color.setRedF(1.0f);
                color.setGreenF(1.0f-4*(alpha-.75f));
                color.setBlueF(0);
            } else
            {
        color.setRedF(1.0f);
        color.setGreenF(0);
        color.setBlueF(0);
    }
    return color;
}
