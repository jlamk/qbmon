#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qbmon_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QString xrandr_path;

    if ( QFile::exists("/usr/bin/xrandr")  )
        xrandr_path = "/usr/bin/xrandr";
    else
        if ( QFile::exists("/bin/xrandr")  )
            xrandr_path = "/bin/xrandr";

    if ( xrandr_path == "" )
    {
        QMessageBox msgError;
        msgError.setText("xrandr not found!");
        msgError.setIcon(QMessageBox::Warning);
        msgError.setWindowTitle("Error");
        msgError.exec();
        exit(-1);
    }

    MainWindow w;
    w.xrandr = xrandr_path;
    qInfo() << xrandr_path;
    w.show();
    return a.exec();
}
