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

    QFont font;
    int fontSize ;

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_toggled(bool checked);

    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_spinBox_3_valueChanged(int arg1);
    void on_radioButton_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

    void on_checkBox_2_clicked(bool checked);

private:
    Ui::HistoParamForm *ui;
    void updateCheckBox();
};

#endif // HISTOPARAMFORM_H
