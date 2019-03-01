#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QPrinter>
#include "configuration.h"
#include "device.h"
#include "tests/abstracttest.h"

#include <QDebug>

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
    //bool tstLoaded;
    bool su;
    int prepareFirstStart;

    QAction *act_devMode1, *act_devMode2;  //"Секретный режим"
    //QSettings project;

    bool loadProject(QSettings& settings);
    void loadTest(AbstractTest* test);

    QString logFile;
    QString logFileDefault;
    QString helpFile;

    bool clearProject();
    void setReport(QTextDocument& doc);
    int dragIndex, dropIndex;

private slots:
    void onAbout();
    void onPushConfig();

    void prepare();
    void onRunTst();
    void onPause();
    void onStop();
    //void onStopMonitor();
    void setSlot(QPushButton*, QPushButton*, QPushButton*);
    void unsetSlot(QPushButton*, QPushButton*, QPushButton*);
    void onDragged();
    void onDropped();
    void addDevice();
    void createTest();
    void openTest();

    bool onSavePrj();
    void onLoadPrj();

    void onHelp();
    bool onCreateRep();
    void onPrintRep();
    void printReport(QPrinter* prnt);
    void onMenuDevices(QPoint);
    void onMenuTests(QPoint);
    void onMenuOutput(QPoint);
    void outputClear();
    void actDevMode();

    void applyToAllTests(QString&,QString&,QString&,QString&);
};

#endif // MAINWINDOW_H
