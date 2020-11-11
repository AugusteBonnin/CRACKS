#include "intcolorbadge.h"
#include "intrangedmapresult.h"
#include "junctionmapwidget.h"
#include "openglscalewidget.h"
#include "roadmapwidget.h"
#include "ui_intmapresult.h"
#include "mainwindow.h"

#include <QPainter>

IntRangedMapResult::IntRangedMapResult(MapPage *parent, QString titre, QVector<uint32_t> &data, int index, int min, int max, int step, int type) :
    MapResult(titre,parent,index) ,
    data(data)
{
 QVector<int> classes_sup;


    for (int i = min ; i < max ; i+= step)
        classes_sup << i ;

    QSet<int> set;
    for (int i = 0 ; i < data.count() ; i++)
        set.insert(data[i]);

     QList<int> values = set.toList();

    QHash<int,int> classForValue;
    QHash<int,QVector<int> > valuesForClass;
    for (int i = 0 ; i < values.count() ; i++)
    {
        int classe = 0 ;
        while ((classe<classes_sup.count())&&(values[i]>classes_sup[classe]))
            classe++;
        classForValue.insert(values[i],classe) ;
        QVector<int> v = valuesForClass.value(classe,QVector<int>());
        v << values[i] ;
        valuesForClass.insert(classe,v);
    }

    QVector<QColor> colorForClass;
    int class_count = valuesForClass.keys().count();
    for (int i =0 ; i < class_count;i++)
    {
        colorForClass << colorFor(i/(float)(class_count-1.0f));
        QVector<int> class_values = valuesForClass.value(i);
        std::sort(class_values.begin(),class_values.end());
        if (class_values.count())
            ui->verticalLayout->addWidget(new IntColorBadge(this,
                                                        colorForClass[i],
                                                        QString("[%1,%2]")
                                                        .arg(class_values[0])
                                      .arg(class_values[class_values.count()-1])
                                                        )
                                      );
    }

    for (int i = 0 ; i < data.count() ; i++)
        colors << colorForClass[classForValue.value(data[i])] ;


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
