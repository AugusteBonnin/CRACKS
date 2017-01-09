#include "docform.h"
#include "regularizationdocform.h"
#include "mainwindow.h"
#include "ui_introdocform.h"

RegularizationDocForm::RegularizationDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Regularisation")));
}

RegularizationDocForm::~RegularizationDocForm()
{
}
