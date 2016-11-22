#include "mainwindow.h"
#include "saveform.h"
#include "skelparamform.h"
#include "ui_skelform.h"
#include <QTimer>

SkelParamForm::SkelParamForm(MainWindow *parent, SkelPage *page) :
    ParamForm(parent),
    ui(new Ui::SkelForm),page(page)
{
    ui->setupUi(this);

    ui->doubleSpinBox->setValue(settings.value("Skel/Prune",2.0).toDouble());

    ui->verticalLayout->addWidget(new SaveForm(parent,QString("Skel")));
}

SkelParamForm::~SkelParamForm()
{
    delete ui;
}



void SkelParamForm::on_doubleSpinBox_valueChanged(double arg1)
{
    settings.setValue("Skel/Prune",arg1);
}

void SkelParamForm::on_pushButton_clicked()
{
    page->reinit();
}
