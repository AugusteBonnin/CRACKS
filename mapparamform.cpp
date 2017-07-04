#include "mappage.h"
#include "mapparamform.h"
#include "ui_mapparamform.h"

MapParamForm::MapParamForm(MainWindow *mainWindow,MapPage*page) :
    ParamForm(mainWindow),
    page(page),
    ui(new Ui::MapParamForm)
{
    ui->setupUi(this);

    QString path = page->getTitle() + "/" ;

    for (int i = 0 ; i< page->results.count() ; i++)
    {
        QString path2 = path + page->results[i]->getName() + "/" ;
        if (settings.value(path2+"SaveJPG",false).toBool()||
                settings.value(path2+"SaveSVG",false).toBool()||
                settings.value(path2+"SaveSHP",false).toBool())
            ui->comboBox->addItem(QString("+%1").arg(page->results[i]->getName()),page->results[i]->getName());
        else
            ui->comboBox->addItem(page->results[i]->getName(),page->results[i]->getName());

    }

    QString path2 = path + page->results[0]->getName() + "/" ;
    ui->checkBox->setChecked(settings.value(path2+"SaveJPG",false).toBool());
    ui->groupBox_2->setEnabled(ui->checkBox->isChecked());
    ui->radioButton_4->setChecked(settings.value(path2+"Screenshot",true).toBool());
    ui->radioButton_3->setChecked(!ui->radioButton_4->isChecked());
    ui->checkBox_3->setChecked(settings.value(path2+"SaveSVG",false).toBool());


}

MapParamForm::~MapParamForm()
{
    delete ui;
}

void MapParamForm::on_comboBox_currentIndexChanged(int index)
{
    page->container->setCurrentIndex(index);
    QString path = page->getTitle() + "/" ;
    QString path2 = path + page->results[index]->getName() + "/" ;
    ui->checkBox->setChecked(settings.value(path2+"SaveJPG",false).toBool());
    ui->groupBox_2->setEnabled(ui->checkBox->isChecked());
    bool tmp = settings.value(path2+"Screenshot",true).toBool() ;
    ui->radioButton_4->setChecked(tmp);
    ui->radioButton_3->setChecked(!tmp);

}

void MapParamForm::on_checkBox_clicked(bool checked)
{
    QString path = page->getTitle() + "/" ;
    QString path2 = path + page->results[ui->comboBox->currentIndex()]->getName() + "/" ;

    settings.setValue(path2+"SaveJPG",checked);
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
    QString path = page->getTitle() + "/" ;
    QString path2 = path + page->results[ui->comboBox->currentIndex()]->getName() + "/" ;
    settings.setValue(path2+"SaveSVG",checked);
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
    QString path = page->getTitle() + "/" ;
    QString path2 = path + page->results[ui->comboBox->currentIndex()]->getName() + "/" ;
    settings.setValue(path2+"Screenshot",checked);

}


