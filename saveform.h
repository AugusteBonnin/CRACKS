#ifndef SAVEFORM_H
#define SAVEFORM_H

#include "paramform.h"

#include <QWidget>

namespace Ui {
class SaveForm;
}

class SaveForm : public ParamForm
{
    Q_OBJECT
QString my_prefix;
public:
    explicit SaveForm(MainWindow *parent, QString my_prefix);
    ~SaveForm();

private slots:
void on_checkBox_clicked(bool checked);
void on_checkBox_2_clicked(bool checked);
void on_checkBox_3_clicked(bool checked);
void on_radioButton_toggled(bool checked);
private:
Ui::SaveForm *ui;
};

#endif // SAVEFORM_H
