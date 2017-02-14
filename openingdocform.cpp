#include "docform.h"
#include "openingdocform.h"
#include "mainwindow.h"

OpeningDocForm::OpeningDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Ouverture"));
}

OpeningDocForm::~OpeningDocForm()
{
}
