#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "get_various_info.h"
#include <QDateTime>
#include <QMessageBox>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setObjectName("The_Mainwindow_Object");
    clock_label = new QLabel(this);
    clock_label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    clock_label->setFrameShape(QFrame::Shape::Box);
    clock_label->setFont(QFont("Carlito", 20));
    ui->statusbar->addPermanentWidget(clock_label, 1);
    name_label = new QLabel(this);
    name_label->setText("Klaus Fütterer. ");
    name_label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    name_label->setFrameShape(QFrame::Shape::Box);
    name_label->setFont(QFont("Carlito", 20));
    ui->statusbar->addPermanentWidget(name_label, 1);
    QTimer *clock_timer = new QTimer(this);
    connect(clock_timer, &QTimer::timeout, this, &MainWindow::update_clock);
    clock_timer->start(1000);
    tray_icon = new QSystemTrayIcon(this);
    tray_icon->setIcon(QIcon(":/images/kf.jpg"));
    tray_icon->show();
    setWindowIcon(QIcon(":/images/kf.jpg"));
    setWindowTitle("my_Cpupower_gui");
    ui->btn_exit->installEventFilter(this);
    ui->btn_powersave->installEventFilter(this);
    ui->btn_performance->installEventFilter(this);
    ui->btn_frequencies->installEventFilter(this);
    ui->btn_governor->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui; ui = nullptr;
    delete clock_label; clock_label = nullptr;
    delete name_label; name_label = nullptr;
    delete clock_timer; clock_timer = nullptr;
    delete tray_icon; tray_icon = nullptr;
    delete my_cpupower_process; my_cpupower_process = nullptr;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_btn_exit_clicked()
{
    close();
}

void MainWindow::update_clock()
{
    clock_label->setText(QDateTime::currentDateTime().toString("ddd dd.MMM.yyyy hh:mm:ss "));
    std::string file = "/sys/devices/system/cpu/cpufreq/policy0/scaling_governor";
    std::ifstream file_output_stream(file, std::ios::in);
    if (!file_output_stream.is_open()) {
        // std::cerr << "Error: Unable to open /sys/devices/system/cpu/cpufreq/policy0/scaling_governor" << std::endl;
        QMessageBox::warning(nullptr,
                             "Error: Unable to open /sys/devices/system/cpu/cpufreq/policy0/scaling_governor",
                             "Error: \nUnable to open /sys/devices/system/cpu/cpufreq/policy0/scaling_governor"
                             "\nApplication must close.",
                             QMessageBox::StandardButton(QMessageBox::Ok),
                             QMessageBox::StandardButton(QMessageBox::NoButton));
        //return "Error: Unable to open /sys/devices/system/cpu/cpufreq/policy0/scaling_governor";
        close();
    }
    std::string file_content;
    std::getline(file_output_stream, file_content);
    if (file_content == "powersave")
    {
        ui->btn_powersave->setText("Powersave set.");
        ui->btn_performance->setText("Performance");
    }
    else
        if (file_content == "performance")
        {
            ui->btn_powersave->setText("Powersave");
        ui->btn_performance->setText("Performance set.");
        }
}

void MainWindow::on_actionLicence_triggered()
{
    QMessageBox *show_license = new QMessageBox(this);
    show_license->setWindowIcon(QIcon(":/image_files/kf.jpg"));
    show_license->setWindowTitle("License Donald:");
    show_license->setText("Donald:\n"
    "Copyright (C) <2026>  <Klaus Fütterer>\n"
    "MIT License:\n"
    "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
    "of this software and associated documentation files (the \"Software\"), to deal\n"
    "in the Software without restriction, including without limitation the rights\n"
    "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
    "copies of the Software, and to permit persons to whom the Software is\n"
    "furnished to do so, subject to the following conditions:\n\n"
    "The above copyright notice and this permission notice shall be included in all\n"
    "copies or substantial portions of the Software.\n\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
    "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
    "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
    "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
    "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
    "SOFTWEARE.\n"
    "klaus.fuetterer__at__arcor.de\n");
    show_license->setStandardButtons(QMessageBox::Ok);
    show_license->setDefaultButton(QMessageBox::Ok);
    show_license->defaultButton()->setText("Got It !   ");
    show_license->exec();
    delete show_license; show_license = nullptr;
}

void MainWindow::on_actionAbout_triggered()
{
    QSysInfo *sys_info = new QSysInfo;
    QMessageBox *about_box = new QMessageBox(this);
    about_box->setWindowIcon(QIcon(":/images/kf.jpg"));
    about_box->setWindowTitle("About my_Cpupower_gui: ");
    about_box->setText("my_Cpupower_gui by Klaus Fütterer\n\n"
                       "Created with:\tQt Creator\n"
                       "Qt Version:\t\t" + QString(qVersion()) + "\n"
                       + "Compiler:\t\t" + QString::fromStdString(Get_various_Info::getCompilerInfo()) + "\n"
                       + "CPU:\t\t" + QString::fromStdString(Get_various_Info::getCpuModel()) + "\n\t\t"
                       + QString::fromStdString(Get_various_Info::getConcurrentThreadsSupported())
                       + " concurrent Threads supported.       "+ "\n\n"
                       + "Hooray, we're hosted by: "
                       + QString(sys_info->prettyProductName()) + ", "
                       + QString(sys_info->currentCpuArchitecture()) + ".\n");
    about_box->setStandardButtons(QMessageBox::Ok);
    about_box->setDefaultButton(QMessageBox::Ok);
    about_box->defaultButton()->setText("Got It !   ");
    about_box->exec();
    delete sys_info;
    sys_info = nullptr;
    delete about_box;
    about_box = nullptr;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::HoverEnter)
    {
        if (obj->objectName() == "btn_powersave")
            ui->btn_powersave->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 144, 255);"));
        if (obj->objectName() == "btn_performance")
            ui->btn_performance->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 144, 255);"));
        if (obj->objectName() == "btn_frequencies")
            ui->btn_frequencies->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 144, 255);"));
        if (obj->objectName() == "btn_exit")
            ui->btn_exit->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 144, 255);"));
        if (obj->objectName() == "btn_governor")
            ui->btn_governor->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 144, 255);"));
        return true;
    }
    else
        if ( ev->type() == QEvent::HoverLeave)
        {
            if (obj->objectName() == "btn_powersave")
                ui->btn_powersave->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 196, 222);"));
            if (obj->objectName() == "btn_performance")
                ui->btn_performance->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 196, 222);"));
            if (obj->objectName() == "btn_frequencies")
                ui->btn_frequencies->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 196, 222);"));
            if (obj->objectName() == "btn_exit")
                ui->btn_exit->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 196, 222);"));
            if (obj->objectName() == "btn_governor")
                ui->btn_governor->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 196, 222);"));
            return true;
        }
        else
        {
            // standard event processing
            return QObject::eventFilter(obj, ev);
        }
}

void MainWindow::on_btn_powersave_clicked()
{
    if (my_cpupower_process)
    {
        delete my_cpupower_process; my_cpupower_process = nullptr;
    }
    my_cpupower_process = new QProcess(this);
    QStringList arguments_list;
    arguments_list << "cpupower" << "frequency-set" << "-g" << "powersave";
    my_cpupower_process->start("pkexec", arguments_list);
}

void MainWindow::on_btn_performance_clicked()
{
    if (my_cpupower_process)
    {
        delete my_cpupower_process; my_cpupower_process = nullptr;
    }
    QProcess *my_cpupower_process = new QProcess(this);
    QStringList arguments_list;
    arguments_list << "cpupower" << "frequency-set" << "-g" << "performance";
    my_cpupower_process->start("pkexec", arguments_list);
}

void MainWindow::on_btn_frequencies_clicked()
{
    QProcess *my_cpupower_process = new QProcess(this);
    QStringList arguments_list;
    arguments_list << "-e"
                   << "bash"
                   << "-c"
                   << "watch -n1 \"grep 'cpu MHz' /proc/cpuinfo\"";

    my_cpupower_process->start("/usr/bin/konsole", arguments_list);
}

void MainWindow::on_btn_governor_clicked()
{
    QProcess *my_cpupower_process = new QProcess(this);
    QStringList arguments_list;
    arguments_list << "-e"
                   << "bash"
                   << "-c"
                   << "cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor; "
                      "echo; echo \"Press ENTER to close...\"; read";
    my_cpupower_process->start("/usr/bin/konsole", arguments_list);
}
