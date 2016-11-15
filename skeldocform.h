#ifndef SkelDocForm_H
#define SkelDocForm_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class SkelDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit SkelDocForm(MainWindow *parent);
    ~SkelDocForm();

};

#endif // SkelDocForm_H
