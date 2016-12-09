#include "introparamform.h"
#include "mainwindow.h"
#include "ui_introparamform.h"

#include <QSettings>
#include <QFileDialog>
#include <QStatusBar>

IntroParamForm::IntroParamForm(MainWindow *parent, IntroPage *page) :
    ParamForm(parent),
    ui(new Ui::IntroForm)
{
    ui->setupUi(this);

    this->page = page ;


    connect(ui->radioButton,SIGNAL(clicked(bool)),this,SLOT(operationChoosed()));
    connect(ui->radioButton_2,SIGNAL(clicked(bool)),this,SLOT(operationChoosed()));
    connect(ui->radioButton_3,SIGNAL(clicked(bool)),this,SLOT(operationChoosed()));

    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(chooseFile()));

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(validateFile()));


    QSettings settings;
    switch (settings.value("IntroParamForm-Operation",QVariant(0)).toUInt())
    {
    case 0 :
        ui->radioButton->setChecked(true);
        break ;
    case 1 :
        ui->radioButton_2->setChecked(true);
        break ;
    case 2 :
        ui->radioButton_3->setChecked(true);
        break ;
    }

    operationChoosed() ;
}

IntroParamForm::~IntroParamForm()
{
    delete ui;
}


void IntroParamForm::operationChoosed()
{

    QString file ;

    if ( ui->radioButton->isChecked())
    {
        file = settings.value("IntroParamForm-File-0",QVariant(tr("Pas de fichier sélectionné."))).toString();
        settings.setValue("IntroParamForm-Operation",QVariant(0));
    }
    else if (ui->radioButton_2->isChecked())
    {
        file = settings.value("IntroParamForm-File-1",QVariant(tr("Pas de dossier sélectionné."))).toString();
        settings.setValue("IntroParamForm-Operation",QVariant(1));
    }
    else {
        file = settings.value("IntroParamForm-File-2",QVariant(tr("Pas de dossier sélectionné."))).toString();
        settings.setValue("IntroParamForm-Operation",QVariant(2));
    }

    settings.setValue("File",file);

    ui->lineEdit->setText(file);
}

void IntroParamForm::chooseFile()
{
    QSettings settings;
    QString  filename ;

    switch (settings.value("IntroParamForm-Operation",QVariant(0)).toUInt())
    {
    case 0 :
        filename = QFileDialog::getOpenFileName(this,tr("Choissez une image"),
                                                settings.value("IntroParamForm-File-0",QVariant(QDir::homePath())).toString(),
                                                QString("*.jpg *.JPG *.png *.PNG *.tiff *.TIFF *.tif *.TIF")) ;
        break ;
    case 1 :
        filename = QFileDialog::getExistingDirectory(this,tr("Choisissez un dossier"),
                                                     settings.value("IntroParamForm-File-1",QVariant(QDir::homePath())).toString()) ;
        break ;
    case 2 :
        filename = QFileDialog::getExistingDirectory(this,tr("Choisissez un dossier d'images"),
                                                     settings.value("IntroParamForm-File-2",QVariant(QDir::homePath())).toString()) ;
        break ;
    }

    if (!filename.isEmpty())
        ui->lineEdit->setText(filename);

    validateFile();

}

void IntroParamForm::validateFile()
{
    QSettings settings;

    bool ok = false ;
    QStringList list;
    switch (settings.value("IntroParamForm-Operation",QVariant(0)).toUInt())
    {
    case 0 :
        if (page->image.load(ui->lineEdit->text()))
        {
            settings.setValue("File",ui->lineEdit->text());
            mainWindow->statusBar()->findChild<QLabel*>()->setText(tr("<FONT COLOR=GREEN>Le fichier %1 est bien une image.").arg(
                                                                       settings.value("IntroParamForm-File-0",QVariant("")).toString()));
            settings.setValue("IntroParamForm-File-0",QVariant(ui->lineEdit->text()));
            ok = true ;
        }
        else
        {
            mainWindow->statusBar()->findChild<QLabel*>()->setText(
                        tr("<FONT COLOR=RED>Le fichier %1 n'est pas une image valide.").arg(ui->lineEdit->text()));
            page->image = QImage() ;

        }
        break ;
    case 1 :
        list = QDir(ui->lineEdit->text())
                .entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.tiff"
                           << "*.TIFF" << "*.tif" << "*.TIF");
        if (list.count()&&page->image.load(QString("%1/%2").arg(ui->lineEdit->text()).arg(list[0])))
        {
            settings.setValue("File",QString("%1/%2").arg(ui->lineEdit->text()).arg(list[0]));
            mainWindow->statusBar()->findChild<QLabel*>()->setText(tr("<FONT COLOR=GREEN>Le dossier %1 contient %2 images.").arg(
                                                                       settings.value("IntroParamForm-File-1",QVariant("")).toString())
                                                                   .arg(list.count()));
            settings.setValue("IntroParamForm-File-1",QVariant(ui->lineEdit->text()));
            ok = true ;
        }
        else
        {
            mainWindow->statusBar()->findChild<QLabel*>()->setText(tr("<FONT COLOR=RED>Le dossier %1 ne contient aucune image.").arg(
                                                                       settings.value("IntroParamForm-File-1",QVariant("")).toString()));
            page->image = QImage() ;

        }
        break ;
    case 2 :
        list = QDir(ui->lineEdit->text())
                .entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.tiff"
                           << "*.TIFF" << "*.tif" << "*.TIF");
        if (list.count()&&page->image.load(QString("%1/%2").arg(ui->lineEdit->text()).arg(list[list.count()-1])))
        {
            settings.setValue("File",QString("%1/%2").arg(ui->lineEdit->text()).arg(list[list.count()-1]));
            mainWindow->statusBar()->findChild<QLabel*>()->setText(tr("<FONT COLOR=GREEN>Le dossier %1 contient %2 images.").arg(
                                                                       settings.value("IntroParamForm-File-2",QVariant("")).toString())
                                                                   .arg(list.count()));

            settings.setValue("IntroParamForm-File-2",QVariant(ui->lineEdit->text()));
            ok = true ;
        }
        else
        {
            mainWindow->statusBar()->findChild<QLabel*>()->setText(tr("<FONT COLOR=RED>Le dossier %1 ne contient aucune image.").arg(
                                                                       settings.value("IntroParamForm-File-2",QVariant("")).toString()));
            page->image = QImage() ;
        }
        break ;
    }

    settings.setValue("FileList",list);

    page->setImage(page->image);


}
