#ifndef MapDocForm_H
#define MapDocForm_H

#include "docform.h"

class MapDocForm : public DocForm
{
    Q_OBJECT

public:
    explicit MapDocForm(MainWindow *parent);
    ~MapDocForm();

};

#endif // MapDocForm_H
