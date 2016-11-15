#include "docform.h"
#include "histodocform.h"
#include "mainwindow.h"

HistoDocForm::HistoDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setSource(QUrl("doc/introduction.html"));
}

HistoDocForm::~HistoDocForm()
{
}
