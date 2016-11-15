#ifndef MAPPARAMFORM_H
#define MAPPARAMFORM_H

class MapPage ;
class MainWindow ;

#include "paramform.h"

#include <QWidget>

namespace Ui {
class MapParamForm;
}

class MapParamForm : public ParamForm
{
    Q_OBJECT

    MapPage *page;
public:
    explicit MapParamForm(MainWindow *mainWindow,MapPage* page);
    ~MapParamForm();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_checkBox_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

    void on_radioButton_4_toggled(bool checked);

private:
    Ui::MapParamForm *ui;
};

#endif // MAPPARAMFORM_H
