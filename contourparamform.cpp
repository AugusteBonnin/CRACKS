#include "contourparamform.h"
#include "mainwindow.h"
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
    slider->setImage(page->originalImage);
    slider->setFixedHeight(64);

    ui->verticalLayout->addWidget(slider);

    ui->checkBox->setChecked(settings.value("Contour/Invert",false).toBool());
    ui->checkBox_2->setChecked(settings.value("Contour/KMeans",false).toBool());

    slider->setEnabled(!settings.value("Contour/KMeans",false).toBool());


     ui->checkBox->setChecked(settings.value("Contour/Invert",false).toBool());

     connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(invert()));
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

    maibeKMeans();
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

