#include "mappage.h"
#include "mapparamform.h"
#include "ui_mapparamform.h"

MapParamForm::MapParamForm(MainWindow *mainWindow,MapPage*page) :
    ParamForm(mainWindow),
    page(page),
    ui(new Ui::MapParamForm)
{
    ui->setupUi(this);
    settings.beginReadArray("Maps");

    for (int i = 0 ; i< page->results.count() ; i++)
    {
        settings.setArrayIndex(i);
        if (settings.value("SaveJPG",false).toBool()||
                settings.value("SaveSVG",false).toBool()||
                settings.value("SaveSHP",false).toBool())
        ui->comboBox->addItem(QString("+%1").arg(page->results[i]->getName()),page->results[i]->getName());
        else
            ui->comboBox->addItem(page->results[i]->getName(),page->results[i]->getName());

    }
    settings.setArrayIndex(0);
    ui->checkBox->setChecked(settings.value("SaveJPG",false).toBool());
    ui->groupBox_2->setEnabled(ui->checkBox->isChecked());
    ui->radioButton_4->setChecked(settings.value("Screenshot",true).toBool());
    ui->radioButton_3->setChecked(!ui->radioButton_4->isChecked());
    ui->checkBox_3->setChecked(settings.value("SaveSVG",false).toBool());
    settings.endArray() ;
}

MapParamForm::~MapParamForm()
{
    delete ui;
}

void MapParamForm::on_comboBox_currentIndexChanged(int index)
{
page->container->setCurrentIndex(index);

settings.beginReadArray("Maps");
settings.setArrayIndex(index);
ui->checkBox->setChecked(settings.value("SaveJPG",false).toBool());
        ui->groupBox_2->setEnabled(ui->checkBox->isChecked());
        bool tmp = settings.value("Screenshot",true).toBool() ;
        ui->radioButton_4->setChecked(tmp);
        ui->radioButton_3->setChecked(!tmp);
settings.endArray();

}

void MapParamForm::on_checkBox_clicked(bool checked)
{
    settings.beginWriteArray("Maps");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    settings.setValue("SaveJPG",checked);
    settings.endArray();

    ui->groupBox_2->setEnabled(checked);

    if (checked||ui->checkBox_3->isChecked())
        ui->comboBox->setItemText(ui->comboBox->currentIndex(),
                                  QString("+%1").arg(ui->comboBox->currentData().toString()));
    else
         ui->comboBox->setItemText(ui->comboBox->currentIndex(),
                                      ui->comboBox->currentData().toString());

}

void MapParamForm::on_checkBox_3_clicked(bool checked)
{
    settings.beginWriteArray("Maps");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    settings.setValue("SaveSVG",checked);
    settings.endArray();

    if (checked||ui->checkBox->isChecked())
        ui->comboBox->setItemText(ui->comboBox->currentIndex(),
                                  QString("+%1").arg(ui->comboBox->currentData().toString()));
    else
         ui->comboBox->setItemText(ui->comboBox->currentIndex(),
                                      ui->comboBox->currentData().toString());

}

void MapParamForm::on_radioButton_4_toggled(bool checked)
{
    settings.beginWriteArray("Maps");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    settings.setValue("Screenshot",checked);
    settings.endArray();
}


