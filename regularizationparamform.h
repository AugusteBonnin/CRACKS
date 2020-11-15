#ifndef RegularizationParamForm_H
#define RegularizationParamForm_H

#include "doubleimage.h"
#include "form.h"
#include "imagewidget.h"
#include "regularizationpage.h"

#include <QWidget>

namespace Ui {
class RegularizationForm;
}

class RegularizationParamForm : public ParamForm
{
    Q_OBJECT


    ImageWidget * originalWidget,*resultWidget ;
public:
    explicit RegularizationParamForm(MainWindow *parent,RegularizationPage*page);
    ~RegularizationParamForm();
    QImage originalImage;
    DoubleImage * outputImage;
int getRadius() ;
bool isApplied() ;
private slots:

    void initImages();

    void on_checkBox_2_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

private:
     Ui::RegularizationForm *ui;
     RegularizationPage*page;
};

#endif // RegularizationParamForm_H
