#include "reportpage.h"
#include "reportparamform.h"
#include "ui_reportparamform.h"

ReportParamForm::ReportParamForm(MainWindow *mainWindow, ReportPage *page) :
    ParamForm(mainWindow),
    ui(new Ui::ReportParamForm)
{
    ui->setupUi(this);
}

ReportParamForm::~ReportParamForm()
{
    delete ui;
}
