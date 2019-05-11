#ifndef HISTORESULT_H
#define HISTORESULT_H

#include "result.h"

#include <QStackedWidget>
#include <QString>

class HistoPage ;
class MainWindow;

class HistoResult : public Result
{
    Q_OBJECT
    MainWindow *mainWindow ;
protected :
    QPixmap image;
    QString csv ;
    int index ;

public:
    HistoResult(QString name, HistoPage *parent, int index);
    void saveCSV();
    void saveImage() ;
    QPixmap & getImage(){return image;}

    void saveSVG();
public slots:
    void makeImage();
protected:
    void paintEvent(QPaintEvent *);

    };

#endif // HISTORESULT_H
