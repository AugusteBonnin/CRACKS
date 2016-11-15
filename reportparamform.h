#ifndef REPORTPARAMFORM_H
#define REPORTPARAMFORM_H

#include "paramform.h"

#include <QWidget>

namespace Ui {
class ReportParamForm;
}

class ReportPage ;

class ReportParamForm : public ParamForm
{
    Q_OBJECT

public:
    explicit ReportParamForm(MainWindow *mainWindow, ReportPage *page);
    ~ReportParamForm();

private:
    Ui::ReportParamForm *ui;
};

#endif // REPORTPARAMFORM_H
