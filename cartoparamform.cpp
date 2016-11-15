#include "cartoparamform.h"
#include "ui_cartoparamform.h"

#include <QSettings>

CartoParamForm::CartoParamForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CartoParamForm)
{
    ui->setupUi(this);

    ui->comboBox->setCurrentIndex(0);

    QSettings settings ;

    settings.beginReadArray("Map");

    for (int i = 0 ; i < ui->comboBox->count() ; i++ )
    {
        ui->comboBox->setItemData(i,ui->comboBox->itemText(i));
        settings.setArrayIndex(i);
        if (settings.value("checked",false).toBool())
            ui->comboBox->setItemText(i,"-"+ui->comboBox->itemText(i));
    }
    settings.endArray();

    updateCheckBox();
}

CartoParamForm::~CartoParamForm()
{
    delete ui;
}

void CartoParamForm::on_comboBox_currentIndexChanged(int index)
{

    updateCheckBox();
}

void CartoParamForm::updateCheckBox()
{
    QSettings settings ;
    settings.beginReadArray("Map");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    ui->checkBox->setChecked(settings.value("checked",false).toBool());
    settings.endArray();
}

void CartoParamForm::on_checkBox_toggled(bool checked)
{
    QSettings settings ;
    settings.beginWriteArray("Map");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    settings.setValue("checked",checked);
    settings.endArray();

    if (!checked)
        ui->comboBox->setItemText(ui->comboBox->currentIndex(),ui->comboBox->currentData().toString());
    else
        ui->comboBox->setItemText(ui->comboBox->currentIndex(),"-"+ui->comboBox->currentData().toString());
}
