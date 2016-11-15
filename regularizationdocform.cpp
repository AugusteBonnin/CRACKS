#include "docform.h"
#include "regularizationdocform.h"
#include "mainwindow.h"
#include "ui_introdocform.h"

RegularizationDocForm::RegularizationDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/regularisation.html"));
}

RegularizationDocForm::~RegularizationDocForm()
{
}
