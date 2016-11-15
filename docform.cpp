#include "docform.h"
#include "mainwindow.h"

#include <QGridLayout>
#include <QUrl>
DocForm::DocForm(MainWindow *mainWindow) : Form(mainWindow)
{
QGridLayout *layout = new QGridLayout(this) ;
browser = new QTextBrowser(this) ;
browser->setOpenLinks(false);
QObject::connect(browser,SIGNAL(anchorClicked(QUrl)),mainWindow,SLOT(openBrowser(QUrl)));
layout->addWidget(browser);
setLayout(layout);

}
