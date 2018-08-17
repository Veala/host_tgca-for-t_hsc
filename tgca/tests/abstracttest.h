#ifndef ABSTRACTTEST_H
#define ABSTRACTTEST_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QComboBox>
#include <QSpinBox>
#include <QtUiTools>
#include <QFormBuilder>
#include <QtNetwork>
#include <QPushButton>
#include <QMessageBox>
#include <QDrag>
#include "../device.h"

#define FREELY 0
#define BUSY 1

class absObjToThread;
class GlobalState
{
private:
    static int globalState;
public:
    void setGlobalState(int);
    int getGlobalState() const;
};

class AbstractTest : public QFrame, public GlobalState
{
    Q_OBJECT

#define top_1       QFile settingsFile(fName); \
                    if (!settingsFile.open(QFile::ReadOnly)) \
                        return; \
                    QTextStream out(&settingsFile); \
                    out.readLine(); out.readLine();

#define top_2(str)          if (str.isEmpty()) \
                                return; \
                            QFile settingsFile(str); \
                            if(!settingsFile.open(QFile::WriteOnly)) \
                                return; \
                            QTextStream in(&settingsFile); \
                            in << name_enabled->text() << endl; \
                            in << name_enabled->isChecked() << endl;

public:
    explicit AbstractTest(QWidget *parent = 0);
    virtual ~AbstractTest();
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB);
    QString getName() const;
    bool isReady() const;

    enum ValidState {
        DeviceIsNotAvailable,
        ConnectionIsNotAvailable,
        ItIsOk
    };
    enum RunningState {
        Running,
        Paused,
        Stopped,
        Completed,
        ErrorIsOccured
    };

    void setValidState(ValidState);
    ValidState getValidState() const;
    RunningState getRunningState() const;

signals:
    void globalStart();
    void startTestTh();
    void setEmit(QPushButton*, QPushButton*, QPushButton*);
    void unsetEmit(QPushButton*, QPushButton*, QPushButton*);
    void dragged();
    void dropped();

protected:
    void mousePressEvent(QMouseEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    virtual void startTest() = 0;
    QThread testThread;
    absObjToThread *objToThread;
    QList<QLineEdit*> deviceLineEditList;
    QList<Device*> deviceList;
    QString saveFileNameStr;
    QCheckBox *name_enabled;
    QLabel *fileName, *status;
    QPushButton *startButton, *pauseButton, *stopButton;
    QDialog *settings;
    QVBoxLayout *devices;
    QTextBrowser *projectBrowser, *testsBrowser;
    void message(QString);
    void setConnections(Device*);
    void setDisconnections(Device*);

protected slots:
    void showSettings();
    virtual void save();
    void firstStartTest();
    void pauseTest();
    void stopTest();
    void newDev(QString);
    void checkDeviceAvailability(int);
    void deletingDevice(QString);
    void deletingDevice_part();
//    void connectingSockDevice();
//    void disconnectingSockDevice();
//    void errorDevice(QAbstractSocket::SocketError);
    void testOutout(QString);
    void setRunningState(int);
    virtual void done1(int n) { emit settingsClosed(1111); }
private:
    QMenu menu;
    QHBoxLayout *layout;
    ValidState validState;
    RunningState runningState;
    QSize forIcons;
signals:
    void settingsClosed(int);
};

class absObjToThread : public QObject
{
    Q_OBJECT
public:
    AbstractTest::RunningState threadState;
    int readAll(QTcpSocket*, QByteArray&, int);
    int writeAll(QTcpSocket*, QByteArray&);
public slots:
    virtual void doWork() = 0;
    void setState(int);
signals:
    void resultReady(int);
    void outputReady(QString);
protected:
    int pause_stop();
};

namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB);
AbstractTest *loadTest(QString file, QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB);
}
#endif // ABSTRACTTEST_H
