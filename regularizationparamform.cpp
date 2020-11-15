#include "doubleimage.h"
#include "mainwindow.h"
#include "regularizationparamform.h"
#include "regularizationworker.h"
#include "ui_regularizationform.h"

#include <QSettings>
#include <QThreadPool>
#include <QDir>
#include <QTimer>

RegularizationParamForm::RegularizationParamForm(MainWindow *parent,RegularizationPage*page) :
    ParamForm(parent),
    ui(new Ui::RegularizationForm)
{
    ui->setupUi(this);

    this->page = page ;

    connect(ui->pushButton,SIGNAL(pressed()),page,SLOT(on_compute_clicked()));

    QSettings settings;
    ui->checkBox_2->setChecked( settings.value("Apply1",QVariant(true)).toBool() );
    ui->spinBox->setValue(settings.value("RegularizationForm-Radius",QVariant(10)).toInt());

}

void RegularizationParamForm::initImages()
{

}


RegularizationParamForm::~RegularizationParamForm()
{
    delete ui;
}

int RegularizationParamForm::getRadius()
{
    return ui->spinBox->value() ;
}

bool RegularizationParamForm::isApplied()
{
    return ui->checkBox_2->isChecked() ;
}

void RegularizationParamForm::on_checkBox_2_toggled(bool checked)
{
    QSettings settings ;
    ui->groupBox->setEnabled(checked);
    settings.setValue("Apply1",checked);
}


void RegularizationParamForm::on_spinBox_valueChanged(int arg1)
{
    settings.setValue("RegularizationForm-Radius",arg1);

}
