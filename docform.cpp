#include "docform.h"
#include "mainwindow.h"

#include <QGridLayout>
#include <QUrl>
DocForm::DocForm(QWidget *parent, QString url) : QTextBrowser(parent)
{
setSource(QUrl(url));
setOpenLinks(true);
}
