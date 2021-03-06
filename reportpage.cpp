#include "reportpage.h"
#include "reportparamform.h"
#include "ui_reportpage.h"
#include "mainwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QProgressDialog>
#include <QTimer>

ReportPage::ReportPage(MainWindow *parent) :
    Page(parent),
    ui(new Ui::ReportPage)
{
    ui->setupUi(this);

    paramForm = new ReportParamForm(parent,this) ;
    docForm = new DocForm(parent,tr("qrc:/docs/aide.html#Bilan"));

    ui->textEdit->append(mainWindow->savedFilesPaths.join('\n'));
    ui->textEdit->append(tr("Soit un total de %1 fichiers.\n").arg(mainWindow->savedFilesPaths.count()));

    switch (settings.value("IntroParamForm-Operation",QVariant(0)).toUInt())
    {
    case 0 :
        ui->textEdit->append(tr("Vous avez opté pour le traitement d'une image unique, qui est à présent terminé. "
                                "Cliquez sur la flèche de droite pour quitter l'application."));

        break ;
    case 1 :
        ui->textEdit->append(tr("Vous avez opté pour le traitement d'un lot d'images. "
                                "Il reste %1 images à traiter qui généreront %2 fichiers. "
                                "Cliquez sur la flèche de droite pour lancer le traitement.")
                             .arg(settings.value("FileList").toStringList().count()-1)
                             .arg((settings.value("FileList").toStringList().count()-1)*mainWindow->savedFilesPaths.count()));

        break ;
    case 2 :
        ui->textEdit->append(tr("Vous avez opté pour le traitement d'une séquence d'images. "
                                "Il reste %1 images à traiter qui généreront %2 fichiers. "
                                "Cliquez sur la flèche de droite pour lancer le traitement.")
                             .arg(settings.value("FileList").toStringList().count()-1)
                             .arg((settings.value("FileList").toStringList().count()-1)*mainWindow->savedFilesPaths.count()));

        break ;
    }

    QTimer::singleShot(0,this,SLOT(postInit()));

}

void ReportPage::postInit(void)
{
    mainWindow->setActionsEnabled(true);
}

void ReportPage::batchLoop()
{

}

ReportPage::~ReportPage()
{
    delete ui;
}

void ReportPage::nextPhase()
{

    switch (settings.value("IntroParamForm-Operation",QVariant(0)).toUInt())
    {
    case 0 :
        exit(0);
        break ;
    case 1 :
        mainWindow->batchProcess();
        exit(0);
        break ;
    case 2 :
        mainWindow->dynamicalAnalysis();
         break ;
    }

}
