#include "docform.h"
#include "histodocform.h"
#include "mainwindow.h"

HistoDocForm::HistoDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Histogrammes"));
}

HistoDocForm::~HistoDocForm()
{
}
