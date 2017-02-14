#include "reportdocform.h"
#include "mainwindow.h"

ReportDocForm::ReportDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(tr("qrc:/docs/aide.html#Bilan"));
}

ReportDocForm::~ReportDocForm()
{
}
