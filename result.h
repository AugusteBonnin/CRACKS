#ifndef RESULT_H
#define RESULT_H

#include <QImage>
#include <QString>
#include <QWidget>



class Result : public QWidget
{
Q_OBJECT

     protected:
    QString resultName;

public:
    Result(QString name, QWidget *parent);
    QString getName(){return resultName;}

public slots:
    virtual void setFont() {}

};

#endif // RESULT_H
