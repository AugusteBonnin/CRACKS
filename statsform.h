#ifndef STATSFORM_H
#define STATSFORM_H

#include <QWidget>

namespace Ui {
class StatsForm;
}

class StatsForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatsForm(QWidget *parent = 0);
    ~StatsForm();

private:
    Ui::StatsForm *ui;
};

#endif // STATSFORM_H
