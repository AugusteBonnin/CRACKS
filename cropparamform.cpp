#include "cropparamform.h"
#include "mainwindow.h"
#include "ui_cropform.h"

#include <QDir>
#include <QPainter>
#include <QSettings>
#include <QTimer>

CropParamForm::CropParamForm(MainWindow *parent,CropPage*page) :
    ParamForm(parent),
    ui(new Ui::CropForm)
{
    this->page = page ;

    ui->setupUi(this);

    connect(ui->pushButton_2,SIGNAL(clicked()),page,SLOT(RAZ()));
    connect(ui->pushButton,SIGNAL(clicked()),page,SLOT(makeRectangle()));

    QSettings settings;

    ui->checkBox->setChecked(settings.value("CropForm-SaveJPG",QVariant(true)).toBool());

    ui->checkBox_2->setChecked(settings.value("Apply0",QVariant(true)).toBool());
    ui->mygroupbox->setEnabled(ui->checkBox_2->isChecked());
    page->setEnabled(ui->checkBox_2->isChecked());

    ui->doubleSpinBox->setValue(settings.value("CropForm-UnitY",QVariant(1.0)).toDouble());
    ui->doubleSpinBox_2->setValue(settings.value("CropForm-UnitX",QVariant(1.0)).toDouble());
}

CropParamForm::~CropParamForm()
{
    delete ui;
}

void CropParamForm::on_checkBox_2_toggled(bool checked)
{
    ui->mygroupbox->setEnabled(checked);
    page->setEnabled(checked);
    QSettings settings;
    settings.setValue("Apply0",ui->checkBox_2->isChecked());
}

void CropParamForm::on_doubleSpinBox_valueChanged(double arg1)
{
    QSettings settings;
    settings.setValue("CropForm-UnitY",arg1);

}

void CropParamForm::on_checkBox_toggled(bool checked)
{
    QSettings settings;
    settings.setValue("CropForm-SaveJPG",checked);

}

void CropParamForm::on_doubleSpinBox_2_valueChanged(double arg1)
{
    QSettings settings;
    settings.setValue("CropForm-UnitX",arg1);

}
