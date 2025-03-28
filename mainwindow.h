#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddMonitorsList( QStringList List );
    void SelectedMonitor(QString monitor){ this->Current_Monitor = monitor; SaveConfig(); }
    QString xrandr;

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void LoadConfig();
    void SaveConfig();
    QString Current_Monitor;
    float Current_brightness;
};
#endif // MAINWINDOW_H
