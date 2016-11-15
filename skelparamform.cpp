#include "mainwindow.h"
#include "saveform.h"
#include "skelparamform.h"
#include "ui_skelform.h"
#include <QTimer>

SkelParamForm::SkelParamForm(MainWindow *parent, SkelPage *page) :
    ParamForm(parent),
    ui(new Ui::SkelForm)
{
    ui->setupUi(this);

    ui->verticalLayout->addWidget(new SaveForm(parent,QString("Skel")));
}

SkelParamForm::~SkelParamForm()
{
    delete ui;
}


