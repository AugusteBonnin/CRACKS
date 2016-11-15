#include "statsform.h"
#include "ui_statsform.h"

StatsForm::StatsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatsForm)
{
    ui->setupUi(this);
}

StatsForm::~StatsForm()
{
    delete ui;
}
