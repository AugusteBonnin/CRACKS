#include "docform.h"
#include "histodocform.h"
#include "mainwindow.h"

HistoDocForm::HistoDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Histogrammes")));
}

HistoDocForm::~HistoDocForm()
{
}
