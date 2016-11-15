#ifndef FORM_H
#define FORM_H

#include <QSettings>
#include <QWidget>


class MainWindow ;

class Form :  public QWidget
{
protected :
    MainWindow * mainWindow;
    QSettings settings ;

public:
    Form(MainWindow * parent);
inline MainWindow * getMainWindow() const {return mainWindow;}
};

#endif // FORM_H
