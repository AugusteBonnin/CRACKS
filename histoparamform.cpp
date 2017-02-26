#include "histoparamform.h"
#include "ui_histoparamform.h"
#include "inthistoresult.h"

#include <QSettings>

HistoParamForm::HistoParamForm(MainWindow *parent,HistoPage * page) :
    ParamForm(parent),
    ui(new Ui::HistoParamForm),
    page(page)
{
    ui->setupUi(this);

    ui->comboBox->setCurrentIndex(0);

    QString path=page->getTitle()+"/";

    for (int i = 0 ; i < page->histoResults.count() ; i++ )
    {
        ui->comboBox->addItem(page->histoResults[i]->getName(),page->histoResults[i]->getName());
        QString path2 = path + page->histoResults[i]->getName() +"/" ;
                if (settings.value(path2+"SaveJPG",false).toBool()||settings.value(path2+"SaveCSV",false).toBool())
            ui->comboBox->setItemText(i,"+"+ui->comboBox->itemData(i).toString());
        else
            ui->comboBox->setItemText(i,ui->comboBox->itemData(i).toString());
    }
    QString path2 = path + page->histoResults[0]->getName() +"/" ;

    ui->checkBox->setChecked(settings.value(path2+"SaveJPG",false).toBool());
    ui->groupBox->setEnabled(settings.value(path2+"SaveJPG",false).toBool());
    ui->radioButton->setChecked(settings.value(path2+"72dpi",true).toBool());
    ui->radioButton_2->setChecked(!ui->radioButton->isChecked());
    ui->checkBox->setChecked(settings.value(path2+"SaveCSV",false).toBool());

    //connect(ui->radioButton,SIGNAL(toggled(bool)),this,SLOT(onRadioButtonChange()));
    //connect(ui->radioButton_2,SIGNAL(toggled(bool)),this,SLOT(onRadioButtonChange()));

    font = QFont(settings.value(path+"Font",QFont().toString()).toString()) ;
    fontSize = settings.value(path+"FontSize",12).toInt() ;
    font.setPixelSize(fontSize);

    ui->spinBox_3->setValue(fontSize) ;
    ui->fontComboBox->setCurrentFont(font);

    connect(ui->spinBox_3,SIGNAL(valueChanged(int)),page,SLOT(setFont()));
    connect(ui->fontComboBox,SIGNAL(currentFontChanged(QFont)),page,SLOT(setFont()));

    ui->spinBox->setValue(settings.value("ClassCount",100).toInt());

    updateCheckBox();
}

HistoParamForm::~HistoParamForm()
{
    delete ui;
}

void HistoParamForm::on_comboBox_currentIndexChanged(int index)
{
page->container->setCurrentIndex(index);
    updateCheckBox();
}

void HistoParamForm::updateCheckBox()
{
    settings.beginReadArray("Histo");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    bool checked = settings.value("SaveJPG",false).toBool() ;
    ui->checkBox->setChecked(checked);
    ui->checkBox_2->setChecked(settings.value("SaveCSV",false).toBool());

    bool dpi72 = settings.value("72dpi",true).toBool() ;
    ui->radioButton->setChecked(dpi72);
    ui->radioButton_2->setChecked(!dpi72);

    settings.endArray();


    ui->groupBox->setEnabled(checked);
}

void HistoParamForm::on_checkBox_toggled(bool checked)
{
    settings.beginWriteArray("Histo");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    settings.setValue("SaveJPG",checked);

    if (settings.value("SaveJPG",false).toBool()||settings.value("SaveCSV",false).toBool())
        ui->comboBox->setCurrentText("+"+ui->comboBox->currentData().toString());
    else
        ui->comboBox->setCurrentText(ui->comboBox->currentData().toString());
    settings.endArray();
    ui->groupBox->setEnabled(checked);
}


void HistoParamForm::on_fontComboBox_currentFontChanged(const QFont &f)
{
    font = f ;

    font.setPixelSize(fontSize);

//    ui->label_5->setFont(font);
//    ui->label_6->setFont(font);

    settings.setValue("Font",font.toString()) ;

    repaint() ;
}

void HistoParamForm::on_spinBox_3_valueChanged(int arg1)
{
    fontSize = arg1 ;

    font.setPixelSize(fontSize);

    settings.setValue("FontSize",fontSize) ;

//    ui->label_5->setFont(font);
//    ui->label_6->setFont(font);

    repaint() ;

}

void HistoParamForm::on_radioButton_toggled(bool checked)
{
    settings.beginWriteArray("Histo");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    settings.setValue("72dpi",checked);
    settings.endArray();
    page->histoResults[ui->comboBox->currentIndex()]->makeImage() ;
}

void HistoParamForm::on_spinBox_valueChanged(int arg1)
{
    settings.setValue("ClassCount",arg1);
    page->repaint();
}


void HistoParamForm::on_checkBox_2_clicked(bool checked)
{
    settings.beginWriteArray("Histo");
    settings.setArrayIndex(ui->comboBox->currentIndex());
    settings.setValue("SaveCSV",checked);
    if (settings.value("SaveJPG",false).toBool()||settings.value("SaveCSV",false).toBool())
        ui->comboBox->setCurrentText("+"+ui->comboBox->currentData().toString());
    else
        ui->comboBox->setCurrentText(ui->comboBox->currentData().toString());

    settings.endArray();

}
