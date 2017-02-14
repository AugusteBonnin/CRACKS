#include "docform.h"
#include "regularizationdocform.h"
#include "mainwindow.h"
#include "ui_introdocform.h"

RegularizationDocForm::RegularizationDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Regularisation"));
}

RegularizationDocForm::~RegularizationDocForm()
{
}
