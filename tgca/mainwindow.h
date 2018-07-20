#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //bool isConnected(int i);

private:
    Ui::MainWindow *ui;

    /// для установки и проверки соединения
    bool connected[2];

private slots:
    void onPushAbout();
    void onPushConfig();
    void onPushConnect();
    void onPushLoadTests();
    void onPushProject();
    void onPushReport();
    void addDevice();
};

#endif // MAINWINDOW_H
