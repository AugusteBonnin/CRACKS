#ifndef HistoDocForm_H
#define HistoDocForm_H

#include "docform.h"
#include "form.h"

#include <QWidget>

class HistoDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit HistoDocForm(MainWindow *parent);
    ~HistoDocForm();

};

#endif // HistoDocForm_H
