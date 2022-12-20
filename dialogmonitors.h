#ifndef DIALOGMONITORS_H
#define DIALOGMONITORS_H

#include <QDialog>

namespace Ui {
class DialogMonitors;
}

class DialogMonitors : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMonitors(QWidget *parent = nullptr);
    ~DialogMonitors();
    void AddMonitorsList( QStringList List );
    QString GetSelectedMonitor(){ return this->SelectedMonitor; }

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_helpRequested();

private:
    Ui::DialogMonitors *ui;
    QString SelectedMonitor;
};

#endif // DIALOGMONITORS_H
