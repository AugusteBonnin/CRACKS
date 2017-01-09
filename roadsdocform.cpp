#include "docform.h"
#include "roadsdocform.h"
#include "mainwindow.h"

RoadsDocForm::RoadsDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Voies")));
}

RoadsDocForm::~RoadsDocForm()
{
}
