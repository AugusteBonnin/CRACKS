#include "mappage.h"
#include "mapparamform.h"
#include "ui_mapparamform.h"

MapParamForm::MapParamForm(MainWindow *mainWindow,MapPage*page) :
    ParamForm(mainWindow),
    page(page),
    ui(new Ui::MapParamForm)
{
    ui->setupUi(this);

    for (int i = 0 ; i< page->results.count() ; i++)
    {

            ui->comboBox->addItem(page->results[i]->getName(),page->results[i]->getName());

    }

}

MapParamForm::~MapParamForm()
{
    delete ui;
}

void MapParamForm::on_comboBox_currentIndexChanged(int index)
{
    page->container->setCurrentIndex(index);
}

