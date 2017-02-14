#include "docform.h"
#include "roadsdocform.h"
#include "mainwindow.h"

RoadsDocForm::RoadsDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Voies"));
}

RoadsDocForm::~RoadsDocForm()
{
}
