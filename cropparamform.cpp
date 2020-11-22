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

    ui->checkBox_2->setChecked(settings.value("Apply0",QVariant(true)).toBool());
    ui->mygroupbox->setEnabled(ui->checkBox_2->isChecked());
    page->setEnabled(ui->checkBox_2->isChecked());

    ui->checkBox_3->setChecked(settings.value("Crop/Unit",false).toBool());
    ui->groupBox->setVisible(settings.value("Crop/Unit",false).toBool());

    ui->doubleSpinBox->setValue(settings.value("CropForm-UnitY",QVariant(300.0)).toDouble());
    ui->doubleSpinBox_2->setValue(settings.value("CropForm-UnitX",QVariant(300.0)).toDouble());

    ui->comboBox->setEditText(settings.value("Crop/UnitName","mm").toString());
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
    ((CropPage*)page)->remap(((CropPage*)page)->quadWidget->polygon);
    page->repaint();

}

void CropParamForm::on_doubleSpinBox_2_valueChanged(double arg1)
{
    QSettings settings;
    settings.setValue("CropForm-UnitX",arg1);
    ((CropPage*)page)->remap(((CropPage*)page)->quadWidget->polygon);
    page->repaint();

}

void CropParamForm::on_checkBox_3_toggled(bool checked)
{
    QSettings settings;
    settings.setValue("Crop/Unit",checked);
    ((CropPage*)page)->remap(((CropPage*)page)->quadWidget->polygon);
    ((CropPage*)page)->scaleWidget->setVisible(checked);
    page->repaint();
    ui->groupBox->setVisible(checked);
 }

void CropParamForm::on_comboBox_currentTextChanged(const QString &arg1)
{
    QSettings settings;
    settings.setValue("Crop/UnitName",arg1);
    ui->label_2->setText(arg1);
    ui->label_4->setText(arg1);
    ((CropPage*)page)->scaleWidget->repaint();

}
