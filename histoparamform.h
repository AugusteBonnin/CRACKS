#ifndef HISTOPARAMFORM_H
#define HISTOPARAMFORM_H

#include "histopage.h"
#include "paramform.h"

#include <QWidget>

namespace Ui {
class HistoParamForm;
}

class HistoParamForm : public ParamForm
{
    Q_OBJECT
HistoPage *page ;
public:
    explicit HistoParamForm(MainWindow *parent, HistoPage *page);
    ~HistoParamForm();


private slots:

    void on_spinBox_valueChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);
private:
    Ui::HistoParamForm *ui;
};

#endif // HISTOPARAMFORM_H
