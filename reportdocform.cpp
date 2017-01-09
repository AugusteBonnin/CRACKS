#include "reportdocform.h"
#include "mainwindow.h"

ReportDocForm::ReportDocForm(MainWindow *parent) :
    DocForm(parent)
{
    browser->setUrl(QUrl("file://"+QApplication::applicationDirPath()+tr("/doc/aide.html#Bilan")));
}

ReportDocForm::~ReportDocForm()
{
}
