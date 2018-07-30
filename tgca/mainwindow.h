#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "configuration.h"
#include "abstracttest.h"

namespace Ui {
class MainWindow;
}

struct SPrjSettings
{
    bool autoLoad;
    bool su;
    QString logFile;

    //vector struct
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;

    Configuration *devConf;

    bool prjLoaded;
    bool tstLoaded;
    bool su;
    bool inRun;
    bool inPause;

    QAction *act_devMode;  //"Секретный режим"
    //QSettings project;

    bool loadProject(QSettings& settings);

    QString logFile;
    QString helpFile;

    void clearProject();

private slots:
    void onAbout();
    void onPushConfig();
    void onRunTst();
    void onStop();
    void onPause();
    void onSavePrj();
    void onLoadPrj();
    void addDevice();
    void createTest();
    void loadTest();
    void onHelp();
    void onPrintRep();
    void onCreateRep();
    void onMenuDevices(QPoint);
    void onMenuTests(QPoint);
    void actDevMode();

};

#endif // MAINWINDOW_H
