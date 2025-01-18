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

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

void MainWindow::LoadConfig()
{
    const QString config_file = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config";
    QSettings settings(config_file,QSettings::IniFormat);

    if ( QFile::exists(config_file) )
    {
        Current_Monitor = settings.value("monitor","").toString();
        Current_brightness = settings.value("brightness",1).toFloat();
        if ( Current_Monitor == "" )
            ui->horizontalSlider->setEnabled(false);
        else
            ui->horizontalSlider->setEnabled(true);
    }
    else
    {
        settings.setValue("monitor","");
        settings.setValue("brightness",1);
        Current_Monitor = "";
        Current_brightness = 1;
        settings.sync();
        ui->horizontalSlider->setEnabled(false);
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
    if (  Current_Monitor != "" )
        ui->horizontalSlider->setEnabled(true);
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

QStringList GetDispList() {
    QStringList list;

    Display* display = XOpenDisplay(nullptr);

    if (!display) {
        qInfo() << "Failed to open X display.";
        exit(0);
    }
    Window root = DefaultRootWindow(display);

    // Get screen resources
    XRRScreenResources* screenResources = XRRGetScreenResources(display, root);
    if (!screenResources) {
        qInfo() << "Failed to get screen resources.";
        return list;
    }

    // Iterate over outputs
    for (int i = 0; i < screenResources->noutput; ++i) {
        XRROutputInfo* outputInfo = XRRGetOutputInfo(display, screenResources, screenResources->outputs[i]);
        if (!outputInfo) continue;

        // Check if the output is connected
        if (outputInfo->connection == RR_Connected) {
            list.append(outputInfo->name);
        }

        // Free the output information
        XRRFreeOutputInfo(outputInfo);
    }

    // Free the screen resources
    XRRFreeScreenResources(screenResources);

    XCloseDisplay(display);
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
    QStringList monitors =  GetDispList();

    DialogMonitors *list = new DialogMonitors();
    list->AddMonitorsList( monitors );
    list->exec();
    QString select = list->GetSelectedMonitor();
    if ( select != "")
    {
        this->Current_Monitor = list->GetSelectedMonitor();
    }
    list->deleteLater();
    SaveConfig();
}

