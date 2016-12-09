#ifndef HISTORESULT_H
#define HISTORESULT_H

#include "result.h"

#include <QString>

class HistoPage ;
class MainWindow;

class HistoResult : public Result
{
    Q_OBJECT
    MainWindow *mainWindow ;
protected :
    QImage image;
    QString csv ;
    int index ;

public:
    HistoResult(QString name, HistoPage *parent, int index);
    void saveCSV();
    void saveImage() ;
    QImage & getImage(){return image;}

public slots:
    void makeImage();
    };

#endif // HISTORESULT_H
