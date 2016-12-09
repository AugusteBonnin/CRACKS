#include "saveform.h"
#include "ui_saveform.h"

SaveForm::SaveForm(MainWindow *parent, QString prefx) :
    ParamForm(parent),
    ui(new Ui::SaveForm),
    my_prefix(prefx)
{
    ui->setupUi(this);

ui->checkBox->setChecked(settings.value(QString("%1/SaveJPG").arg(my_prefix),false).toBool());
ui->radioButton->setChecked(settings.value(QString("%1/Screenshot").arg(my_prefix),false).toBool());
ui->radioButton_2->setChecked(!settings.value(QString("%1/Screenshot").arg(my_prefix),false).toBool());
ui->groupBox_2->setEnabled(ui->checkBox->isChecked());
ui->checkBox_2->setChecked(settings.value(QString("%1/SaveSVG").arg(my_prefix),false).toBool());
ui->checkBox_3->setChecked(settings.value(QString("%1/SaveSHP").arg(my_prefix),false).toBool());

}

SaveForm::~SaveForm()
{
    delete ui;
}

void SaveForm::on_checkBox_clicked(bool checked)
{
    settings.setValue(QString("%1/SaveJPG").arg(my_prefix),checked);
    ui->groupBox_2->setEnabled(checked);
}

void SaveForm::on_checkBox_2_clicked(bool checked)
{
    settings.setValue(QString("%1/SaveSVG").arg(my_prefix),checked);
}

void SaveForm::on_checkBox_3_clicked(bool checked)
{
    settings.setValue(QString("%1/SaveSHP").arg(my_prefix),checked);
}

void SaveForm::on_radioButton_toggled(bool checked)
{
    settings.setValue(QString("%1/Screenshot").arg(my_prefix),checked);
}

