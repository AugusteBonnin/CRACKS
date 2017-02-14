#include "mapdocform.h"

MapDocForm::MapDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Cartes"));
}

MapDocForm::~MapDocForm()
{
}
