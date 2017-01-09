#include "docform.h"
#include "skeldocform.h"
#include "mainwindow.h"

SkelDocForm::SkelDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Squelettisation")));
}

SkelDocForm::~SkelDocForm()
{
}
