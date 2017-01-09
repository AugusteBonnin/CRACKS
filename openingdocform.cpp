#include "docform.h"
#include "openingdocform.h"
#include "mainwindow.h"

OpeningDocForm::OpeningDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Ouverture")));
}

OpeningDocForm::~OpeningDocForm()
{
}
