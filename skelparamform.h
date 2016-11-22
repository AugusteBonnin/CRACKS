#ifndef SkelParamForm_H
#define SkelParamForm_H

#include "paramform.h"
#include "skelpage.h"
#include "skelwidget.h"

#include <QWidget>

namespace Ui {
class SkelForm;
}

class SkelParamForm : public ParamForm
{
    Q_OBJECT

public:
    explicit SkelParamForm(MainWindow *parent , SkelPage *page);
    ~SkelParamForm();

private slots:

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_clicked();

private:
    Ui::SkelForm *ui;
    SkelPage * page;
};

#endif // SkelParamForm_H
