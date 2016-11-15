#include "dynamicalanalysis.h"

#include <QDir>
#include <QSettings>

DynamicalAnalysis::DynamicalAnalysis()
{
QSettings settings;

uint mask = settings.value(QString("IntroForm-Mask-2"),QVariant(0)).toUInt();

dir = QDir(settings.value("IntroForm-File-2",QVariant("")).toString()) ;
files = dir.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.tiff"
                                   << "*.TIFF" << "*.tif" << "*.TIF");

counter = 0 ;
}
