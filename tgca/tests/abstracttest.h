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
#include "../funclib.h"
#include "../device.h"
#include "../data_structs.h"
#include "../animatedspinnericon.h"

//#define debug_AT

#define FREE 0
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
                    out.readLine(); out.readLine(); out.readLine();

#define top_2(str)          if (str.isEmpty()) \
                                return; \
                            QFile settingsFile(str); \
                            if(!settingsFile.open(QFile::WriteOnly)) \
                                return; \
                            QTextStream in(&settingsFile); \
                            in << name_enabled->text() << endl; \
                            in << name_enabled->isChecked() << endl; \
                            in << mark->text() << endl; \
                            message(tr("Настройки теста сохранены (файл: %1, тест: %2)").arg(fileName->text()).arg(mark->text()));

public:
    explicit AbstractTest(QWidget *parent = 0);
    virtual ~AbstractTest();

    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
    QString getName() const;
    bool isReady() const;
    void setUserLevel(bool b);
    void setEnable(bool b) { name_enabled->setChecked(b); }
    bool getEnable() { return name_enabled->isChecked(); }

    void setBeginTest();
    static AbstractTest* getBeginTest();
    void setEndTest();
    static AbstractTest* getEndTest();
    void resetBeginEnd();

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
        ErrorIsOccured,
        Deleting
    };
private:
    void setValidState(ValidState);
    ValidState getValidState() const;
    RunningState getRunningState() const;
    CAnimatedSpinnerIcon* spinner;

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
    bool event(QEvent *e);
    virtual void startTest() = 0;
    QThread testThread;
    absObjToThread *objToThread;
    QList<QLineEdit*> deviceLineEditList;
    QList<Device*> deviceList;
    QMap<QString, QLabel*> statsMap;
    QString saveFileNameStr;
    QCheckBox *name_enabled;
    QLabel *fileName;
    QPushButton *startButton, *pauseButton, *stopButton;
    QDialog *settings;
    QWidget *stats;
    QVBoxLayout *devices;
    QTextBrowser *projectBrowser, *testsBrowser;
    QLineEdit* mark;
    static AbstractTest* yellowTest, *beginTest, *endTest;

    bool su;
    void message(QString);
    void setConnections(Device*);
    void setDisconnections(Device*);
    virtual void setEnabledSpecial(bool b);
    void disableStat();

protected slots:
    virtual void save();
    void markChanged(QString);
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
    virtual void statsTestOutput(QString, long);
    virtual void statsToZero();
    virtual void statsSave();
    void setRunningState(int);

    void deleteObject();
private slots:
    void actDevMode();

private:
    QMenu menu;
    QHBoxLayout *layout;
    ValidState validState;
    QLabel *statusIcon, *statusTxt, *begin_end_Icon;
    RunningState runningState;
    QSize forIcons;
    int forIconSize;

    bool isRunning();
};

class absObjToThread : public QObject//, public DeviceDriver
{
    Q_OBJECT

//    int readAll(QTcpSocket*, QByteArray&, int);
//    int writeAll(QTcpSocket*, QByteArray&);
public:
    explicit absObjToThread(QObject* parent = 0);
    AbstractTest::RunningState threadState;
//    int write_F1(QTcpSocket* tcpSocket, QByteArray& writeArray);
//    int write_F2(QTcpSocket* tcpSocket, QByteArray& writeArray);
//    int write_Echo(QTcpSocket* tcpSocket, QByteArray& writeArray);
//    int read_F1(QTcpSocket* tcpSocket, QByteArray& writeArray, QByteArray& readArray);
//    int read_F2(QTcpSocket* tcpSocket, QByteArray& writeArray, QByteArray& readArray);
//    QByteArray cmdHead(int cmd, int dsz);

public slots:
    virtual void doWork() = 0;
    void template_doWork();
    void setState(int);
signals:
    void resultReady(int);
    void outputReady(QString);
    void statsOutputReady(QString,long);
protected:
    int pause_stop();
};

namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB, bool su);
AbstractTest *loadTest(QString file, QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB);
}
#endif // ABSTRACTTEST_H
