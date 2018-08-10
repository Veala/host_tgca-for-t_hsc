#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "configuration.h"
#include "device.h"
#include "tests/abstracttest.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public GlobalState
{
    Q_OBJECT
signals:
    void newDev(QString);
    void setTestStateIcon(int);

private:
    enum TreeState {
        running,
        next,
        stopped,
        bigStop,
        smallStop,
        finished
    } treeState;
    int curIndex;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *e);
    void setTreeState(TreeState);
    TreeState getTreeState() const;
    void message(QString);
    void run(int);

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
    QString logFileDefault;
    QString helpFile;

    bool clearProject();

private slots:
    void onAbout();
    void onPushConfig();
    void onRunTst();
    void onStop();
    //void onStopMonitor();
    void setSlot(QPushButton*, QPushButton*, QPushButton*);
    void unsetSlot(QPushButton*, QPushButton*, QPushButton*);
    void onPause();
    bool onSavePrj();
    void onLoadPrj();

    void addDevice();
    void createTest();
    void loadTest();

    void onHelp();
    void onPrintRep();
    bool onCreateRep();
    void onMenuDevices(QPoint);
    void onMenuTests(QPoint);
    void actDevMode();

};

#endif // MAINWINDOW_H
