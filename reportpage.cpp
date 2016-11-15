#include "mapdocform.h"
#include "mapparamform.h"
#include "reportpage.h"
#include "reportparamform.h"
#include "ui_reportpage.h"

ReportPage::ReportPage(MainWindow *parent) :
    Page(parent),
    ui(new Ui::ReportPage)
{
    ui->setupUi(this);

    paramForm = new ReportParamForm(parent,this) ;
    docForm = new MapDocForm(parent);

}

ReportPage::~ReportPage()
{
    delete ui;
}
