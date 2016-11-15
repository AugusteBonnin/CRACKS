#include "roadspage.h"
#include "roadsparamform.h"
#include "saveform.h"
#include "ui_roadsform.h"
#include <QTimer>

RoadsParamForm::RoadsParamForm(MainWindow *parent,RoadsPage * page) :
    ParamForm(parent),
    ui(new Ui::RoadsForm)
{
    ui->setupUi(this);

    ui->doubleSpinBox->setValue(settings.value("RoadsParamForm-ScaleFactor",QVariant(1.0)).toDouble());
    ui->doubleSpinBox_2->setValue(settings.value("RoadsParamForm-MaxAngle",QVariant(45.0)).toDouble());

    connect(ui->pushButton_2,SIGNAL(pressed()),page,SLOT(on_pushButton_2_clicked()));

    ui->verticalLayout->addWidget(new SaveForm(parent,"Roads"));
}

RoadsParamForm::~RoadsParamForm()
{
    delete ui;
}


void RoadsParamForm::on_doubleSpinBox_valueChanged(double arg1)
{
    settings.setValue("RoadsParamForm-ScaleFactor",arg1);
}

void RoadsParamForm::on_doubleSpinBox_2_valueChanged(double arg1)
{
    settings.setValue("RoadsParamForm-MaxAngle",arg1);
}
