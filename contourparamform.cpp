#include "contourparamform.h"
#include "mainwindow.h"
#include "saveform.h"
#include "ui_contourform.h"

#include <QDir>
#include <QSettings>
#include <QTimer>

ContourParamForm::ContourParamForm(MainWindow *parent,ContourPage * page) :
    ParamForm(parent),
    ui(new Ui::ContourForm)
{
    ui->setupUi(this);

    this->page = page ;

    slider = new HistogramSlider(this) ;
    slider->setImage(parent->openedImage);
    slider->setFixedHeight(64);

    ui->verticalLayout_2->addWidget(slider);

    ui->verticalLayout->addWidget(new SaveForm(parent,"Contour"));

    ui->checkBox->setChecked(settings.value("Contour/Invert",false).toBool());
    ui->checkBox_2->setChecked(settings.value("Contour/KMeans",false).toBool());

    slider->setEnabled(!settings.value("Contour/KMeans",false).toBool());

    connect(page,SIGNAL(invertSignal()),this,SLOT(invert()));
    connect(page,SIGNAL(kMeansSignal()),slider,SLOT(KMeans()));
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(onSliderChanged(int)));
    connect(slider,SIGNAL(valueChanged(int)),page,SLOT(updateThreshold(int)));
}


void
ContourParamForm::onSliderChanged(int value)
{
    settings.setValue("ThresholdForm-Threshold",value) ;
}

ContourParamForm::~ContourParamForm()
{
    delete ui;
}




void ContourParamForm::on_checkBox_clicked(bool checked)
{
    settings.setValue("Contour/Invert",checked) ;

    invert();
    page->updateThreshold(slider->value());

}

void ContourParamForm::invert()
{
    page->originalImage->invert();
    page->originalImage->computeMinMax();
    slider->computeBins();
    slider->setValue(10000-slider->value());
}

void ContourParamForm::on_checkBox_2_clicked(bool checked)
{
    settings.setValue("Contour/KMeans",checked) ;
    maibeKMeans();

}

void ContourParamForm::maibeKMeans()
{
    if (ui->checkBox_2->isChecked())
    {
        slider->KMeans();
        slider->setEnabled(false);
    }
    else
        slider->setEnabled(true);
}
