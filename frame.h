#ifndef FRAME_H
#define FRAME_H

#include "page.h"

#include <QObject>
#include <QSettings>
#include <QSplitter>
#include <QWidget>

class Frame : public QWidget
{
    Q_OBJECT

    QSplitter * HSplitter , *VSplitter;

    QSettings settings ;



public:
    explicit Frame(QWidget *parent , Page *page);
Page *page;
signals:

private slots :
    void saveSplittersState();
public slots:
};

#endif // FRAME_H
