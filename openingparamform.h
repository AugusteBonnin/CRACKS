#ifndef OPENINGPARAMFORM_H
#define OPENINGPARAMFORM_H

#include "form.h"
#include "mainwindow.h"
#include "openingpage.h"
#include "openingzoomwidget.h"
#include "roiwidget.h"

#include <QWidget>

namespace Ui {
class OpeningForm;
}

class OpeningParamForm :  public ParamForm
{
    Q_OBJECT

OpeningPage * page ;
public:
    explicit OpeningParamForm(MainWindow *parent ,OpeningPage * page);
    ~OpeningParamForm();
private slots:
    void on_checkBox_2_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_checkBox_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

private:
    Ui::OpeningForm *ui;
};

#endif // OPENINGFORM_H
