#include "docform.h"
#include "skeldocform.h"
#include "mainwindow.h"

SkelDocForm::SkelDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Squelettisation"));
}

SkelDocForm::~SkelDocForm()
{
}
