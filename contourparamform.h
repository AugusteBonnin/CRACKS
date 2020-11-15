#ifndef ContourParamForm_H
#define ContourParamForm_H

#include "contourpage.h"
#include "contourwidget.h"
#include "form.h"
#include "histogramslider.h"
#include "paramform.h"

#include <QWidget>

namespace Ui {
class ContourForm;
}

class MainWindow ;

class ContourParamForm : public ParamForm
{
    Q_OBJECT

    ContourWidget * widget;
    HistogramSlider * slider ;

    ContourPage * page ;
public:
    explicit ContourParamForm(MainWindow *parent, ContourPage *page);
    ~ContourParamForm();
    void maibeKMeans();
    void invert();

Ui::ContourForm *ui;

private slots:
void onSliderChanged(int value);

void on_checkBox_clicked(bool checked);

void on_checkBox_2_clicked(bool checked);

};

#endif // ContourParamForm_H
