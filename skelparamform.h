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

private:
    Ui::SkelForm *ui;
};

#endif // SkelParamForm_H
