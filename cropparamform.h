#ifndef CropParamForm_H
#define CropParamForm_H

#include "croppage.h"
#include "cropquadwidget.h"
#include "cropzoomwidget.h"
#include "form.h"

#include <QWidget>

namespace Ui {
class CropForm;
}

class CropParamForm : public ParamForm
{
    Q_OBJECT

public:
    explicit CropParamForm(MainWindow *parent, CropPage *page);
    ~CropParamForm();

public slots:

private slots:

    void on_checkBox_2_toggled(bool checked);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_checkBox_toggled(bool checked);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_checkBox_3_toggled(bool checked);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Page * page;
    Ui::CropForm *ui;
     };

#endif // CropParamForm_H
