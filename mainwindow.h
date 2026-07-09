#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QEvent>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
private slots:
    void on_actionExit_triggered();
    void on_btn_exit_clicked();
    void update_clock();
    void on_actionLicence_triggered();
    void on_actionAbout_triggered();
    void on_btn_powersave_clicked();
    void on_btn_performance_clicked();
    void on_btn_frequencies_clicked();
    void on_btn_governor_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *clock_label = nullptr;
    QLabel *name_label = nullptr;
    QTimer *clock_timer = nullptr;
    QSystemTrayIcon *tray_icon = nullptr;
    QProcess *my_cpupower_process = nullptr;

};
#endif // MAINWINDOW_H
