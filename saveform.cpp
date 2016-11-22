#include "saveform.h"
#include "ui_saveform.h"

SaveForm::SaveForm(MainWindow *parent, QString prefix) :
    ParamForm(parent),
    ui(new Ui::SaveForm),
    prefix(prefix)
{
    ui->setupUi(this);

ui->checkBox->setChecked(settings.value(QString("%1/SaveJPG").arg(prefix),false).toBool());
ui->radioButton->setChecked(settings.value(QString("%1/Screenshot").arg(prefix),false).toBool());
ui->radioButton_2->setChecked(!ui->radioButton->isChecked());
ui->groupBox_2->setEnabled(ui->checkBox->isChecked());
ui->checkBox_2->setChecked(settings.value(QString("%1/SaveSVG").arg(prefix),false).toBool());
ui->checkBox_3->setChecked(settings.value(QString("%1/SaveSHP").arg(prefix),false).toBool());

}

SaveForm::~SaveForm()
{
    delete ui;
}

void SaveForm::on_checkBox_clicked(bool checked)
{
    settings.setValue(QString("%1/SaveJPG").arg(prefix),checked);
    ui->groupBox_2->setEnabled(checked);
}

void SaveForm::on_checkBox_2_clicked(bool checked)
{
    settings.setValue(QString("%1/SaveSVG").arg(prefix),checked);
}

void SaveForm::on_checkBox_3_clicked(bool checked)
{
    settings.setValue(QString("%1/SaveSHP").arg(prefix),checked);
}

void SaveForm::on_radioButton_clicked()
{
    settings.setValue(QString("%1/Screeenshot").arg(prefix),true);
}

void SaveForm::on_radioButton_2_clicked()
{
    settings.setValue(QString("%1/Screeenshot").arg(prefix),false);
}
