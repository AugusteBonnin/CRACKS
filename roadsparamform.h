#ifndef RoadsParamForm_H
#define RoadsParamForm_H

#include "form.h"
#include "paramform.h"
#include "roadswidget.h"

#include <QWidget>

namespace Ui {
class RoadsForm;
}

class MainWindow;
class RoadsPage;

class RoadsParamForm : public ParamForm
{
    Q_OBJECT

public:
    explicit RoadsParamForm(MainWindow *parent , RoadsPage *page);
    ~RoadsParamForm();

private slots:

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

private:
    Ui::RoadsForm *ui;
};

#endif // RoadsParamForm_H
