#include "histoparamform.h"
#include "ui_histoparamform.h"
#include "inthistoresult.h"

#include <QSettings>

HistoParamForm::HistoParamForm(MainWindow *parent,HistoPage * page) :
    ParamForm(parent),
    ui(new Ui::HistoParamForm),
    page(page)
{
    ui->setupUi(this);

    for (int i = 0 ; i < page->histoResults.count() ; i++ )
    {
        ui->comboBox->addItem(page->histoResults[i]->getName(),page->histoResults[i]->getName());
    }

    ui->spinBox->setValue(settings.value("ClassCount",100).toInt());

}

HistoParamForm::~HistoParamForm()
{
    delete ui;
}

void HistoParamForm::on_comboBox_currentIndexChanged(int index)
{
page->container->setCurrentIndex(index);
}

void HistoParamForm::on_spinBox_valueChanged(int arg1)
{
    settings.setValue("ClassCount",arg1);
    page->repaint();
}

