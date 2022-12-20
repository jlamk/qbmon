#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogmonitors.h"

#include <QProcess>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QInputDialog>
#include <QRegularExpressionMatch>

void MainWindow::LoadConfig()
{
    const QString config_file = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config";
    QSettings settings(config_file,QSettings::IniFormat);

    if ( QFile::exists(config_file) )
    {
        Current_Monitor = settings.value("monitor","VGA1").toString();
        Current_brightness = settings.value("brightness",1).toFloat();
    }
    else
    {
        settings.setValue("monitor","");
        settings.setValue("brightness",1);
        Current_Monitor = "";
        Current_brightness = 1;
        settings.sync();
    }
        ui->horizontalSlider->setValue(Current_brightness * 10);
}

void MainWindow::SaveConfig()
{
    const QString config_file = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config";
    QSettings settings(config_file,QSettings::IniFormat);
    settings.setValue("monitor",this->Current_Monitor);
    settings.setValue("brightness",Current_brightness);
    settings.sync();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setRange(5,15);
    ui->horizontalSlider->setSingleStep(1);
    LoadConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

float conv( int pos )
{
    return pos / 10.0;
}

/*Create a QStringList from the extraction of a QRegularExpression
https://forum.qt.io/post/382003
*/
static QStringList extractStr(const QString &s, QRegularExpression delim){
    QStringList output;
    QString buff;
    QRegularExpressionMatchIterator i = delim.globalMatch(s);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            buff = match.captured(0);
            output.push_back(buff);
        }
    }
    return output;
}

QStringList GetMonitorsList()
{
    QProcess process;
    QStringList args;
    process.setProgram("/bin/xrandr");
    process.setArguments(args);
    process.start();
    process.waitForReadyRead();
    QString output = process.readAll();

    QRegularExpression exp("([A-Z]{1}[A-Za-z]*)([0-9]*)");
    QStringList list = extractStr(output,exp);
    return list;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if ( value == 0 || Current_Monitor.length() == 0 )
        return;
    QProcess process;
    QStringList args;
    Current_brightness = conv(value);
    args << "--output" << Current_Monitor << "--brightness" << QString::number(Current_brightness);
    process.setProgram("/bin/xrandr");
    process.setArguments(args);
    process.startDetached();
    SaveConfig();
    setWindowTitle( "qbmon ver. "+QString(APP_VERSION)+" ( "+QString::number(Current_brightness)+" )" );
}


void MainWindow::on_pushButton_clicked()
{
    DialogMonitors *list = new DialogMonitors();
    list->AddMonitorsList( QStringList() << this->Current_Monitor << GetMonitorsList());
    list->exec();
    this->Current_Monitor = list->GetSelectedMonitor();
    list->deleteLater();
    SaveConfig();
}

