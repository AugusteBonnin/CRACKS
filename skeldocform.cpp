#include "docform.h"
#include "skeldocform.h"
#include "mainwindow.h"

SkelDocForm::SkelDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/skeletton.html"));
}

SkelDocForm::~SkelDocForm()
{
}
