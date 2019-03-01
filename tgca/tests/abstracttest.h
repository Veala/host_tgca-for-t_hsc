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
    QString testType() const { return name_enabled->text(); }
    template<typename t>
    t* findField(QString& fieldName) { return (t*)(settings->findChild<t*>(fieldName)); }

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
    bool su;

signals:
    void globalStart();
    void startTestTh();
    void setEmit(QPushButton*, QPushButton*, QPushButton*);
    void unsetEmit(QPushButton*, QPushButton*, QPushButton*);
    void dragged();
    void dropped();
    void applyToAll(QString&,QString&,QString&,QString&);

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

    bool userMode() const { return !su; }
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

public:
    explicit absObjToThread(QObject* parent = 0);
    AbstractTest::RunningState threadState;

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

const QString testTypeMemory(QObject::tr("Тест памяти"));
const QString testTypeRegisters(QObject::tr("Тест регистров"));
const QString testTypeEcho(QObject::tr("Тест \"Эхо\""));
const QString testTypeBulbs(QObject::tr("Тест \"лампочек\""));
const QString testTypeMonitor(QObject::tr("Тест \"монитор\""));
const QString testTypeTrmSingle(QObject::tr("Передача одного пакета"));
const QString testTypeVariation(QObject::tr("Перебор параметров команды"));
const QString testTypeGroupVar(QObject::tr("Перебор параметров групповой команды"));
const QString testTypeLoadSPI(QObject::tr("Загрузка по SPI"));
const QString testTypeBuffers(QObject::tr("Проверка буферов"));
const QString testTypeNull(QObject::tr("Тест отладочный"));
const QString testTypeTrash(QObject::tr("Тест trash"));
const QString testTypeNoise(QObject::tr("Помехоустойчивость"));


namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB, bool su);
AbstractTest *loadTest(QString file, QVBoxLayout* devices, QTextBrowser *pB, QTextBrowser *tB);
}
#endif // ABSTRACTTEST_H
