#ifndef CARTOPARAMFORM_H
#define CARTOPARAMFORM_H

#include <QWidget>

namespace Ui {
class CartoParamForm;
}

class CartoParamForm : public QWidget
{
    Q_OBJECT

public:
    explicit CartoParamForm(QWidget *parent = 0);
    ~CartoParamForm();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_comboBox_currentIndexChanged(int index);
private:
    Ui::CartoParamForm *ui;
    void updateCheckBox();
};

#endif // CARTOPARAMFORM_H
