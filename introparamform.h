#ifndef IntroParamForm_H
#define IntroParamForm_H

#include "page.h"
#include "paramform.h"

#include <QWidget>

namespace Ui {
class IntroForm;
}

class IntroPage ;
class MainWindow ;

class IntroParamForm : public ParamForm
{
    Q_OBJECT
IntroPage * page;
public:
    explicit IntroParamForm(MainWindow *parent , IntroPage * page);
    ~IntroParamForm();
    Ui::IntroForm *ui;
bool isApplied(){return true ;}
private:
private slots:
    void operationChoosed();
    void chooseFile();
    void validateFile();
};

#endif // IntroParamForm_H
