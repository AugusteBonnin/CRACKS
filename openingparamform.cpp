#include "dilatationworker.h"
#include "erosionworker.h"
#include "openingparamform.h"
#include "ui_openingform.h"

#include <QDir>
#include <QSettings>
#include <QThreadPool>
#include <QTimer>

OpeningParamForm::OpeningParamForm(MainWindow *parent,OpeningPage * page) :
    ParamForm(parent),
    ui(new Ui::OpeningForm)
{
    this->page = page ;
    ui->setupUi(this);

    ui->spinBox->setValue(settings.value("OpeningParamForm-Zoom",QVariant(1)).toInt()) ;
    ui->spinBox_2->setValue(settings.value("OpeningParamForm-Radius",QVariant(10)).toInt()) ;
    ui->checkBox->setChecked(settings.value("OpeningParamForm-SaveJPG",QVariant(false)).toBool()) ;
    ui->checkBox_2->setChecked(settings.value("OpeningParamForm-Apply",QVariant(true)).toBool()) ;
    ui->groupBox->setEnabled(ui->checkBox_2->isChecked());
    connect(ui->spinBox,SIGNAL(valueChanged(int)),page->originalZoomWidget,SLOT(setMagnification(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),page->resultZoomWidget,SLOT(setMagnification(int)));
    connect(ui->spinBox_2,SIGNAL(valueChanged(int)),page,SLOT(preview()));
    connect(ui->pushButton,SIGNAL(pressed()),page,SLOT(compute_clicked()));
    connect(ui->checkBox_3,SIGNAL(toggled(bool)),page,SLOT(invert_clicked()));

    ui->checkBox_3->setChecked(settings.value("OpeningParamForm-Invert",false).toBool()) ;

    }

OpeningParamForm::~OpeningParamForm()
{
    delete ui;
}


void OpeningParamForm::on_checkBox_2_toggled(bool checked)
{
    settings.setValue("OpeningParamForm-Apply",checked);

    ui->groupBox->setEnabled(checked);
}

void OpeningParamForm::on_checkBox_toggled(bool checked)
{
    settings.setValue("OpeningParamForm-SaveJPG",checked);


}

void OpeningParamForm::on_spinBox_valueChanged(int arg1)
{
    settings.setValue("OpeningParamForm-Zoom",arg1);
}
void OpeningParamForm::on_spinBox_2_valueChanged(int arg1)
{
    settings.setValue("OpeningParamForm-Radius",arg1);
}

void OpeningParamForm::on_checkBox_clicked(bool checked)
{
     settings.setValue("OpeningParamForm-SaveJPG",checked);
}

void OpeningParamForm::on_checkBox_3_clicked(bool checked)
{
    settings.setValue("OpeningParamForm-Invert",checked);

}
