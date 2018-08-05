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
#include "../device.h"

#define FREELY 0
#define BUSY 1


class AbstractTest : public QFrame
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

signals:

public:
    explicit AbstractTest(QWidget *parent = 0);
    virtual ~AbstractTest();
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);
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

    int getGlobalState() const;

protected:
    void mousePressEvent(QMouseEvent *);
    virtual void startTest() = 0;
    bool pause, stop;
    QList<QLineEdit*> deviceLineEditList;
    QList<Device*> deviceList;
    QString saveFileNameStr;
    QCheckBox *name_enabled;
    QLabel *fileName;
    QPushButton *startButton, *pauseButton, *stopButton;
    QLabel *status;
    QDialog *settings;
    QVBoxLayout *devices;
    QTextBrowser *projectBrowser;
    void message(QString);
    void setConnections(Device*);
    void setDisconnections(Device*);

    void setValidState(ValidState);
    ValidState getValidState() const;

    void setRunningState(RunningState);
    RunningState getRunningState() const;

    void setGlobalState(int);

protected slots:
    void showSettings(bool);
    void deleteProc(bool);
    virtual void save(bool);
    void startTest(bool);
    void pauseTest(bool);
    void stopTest(bool);
    void newDev(QString);
    void checkDeviceAvailability(int);
    void deletingDevice(QString);
    void deletingDevice_part();
    void connectingSockDevice();
    void disconnectingSockDevice();
    void errorDevice(QAbstractSocket::SocketError);

private:
    QMenu menu;
    QHBoxLayout *layout;
    ValidState validState;
    RunningState runningState;
    static int globalState;
};

class MemTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit MemTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);
protected slots:
    virtual void save(bool);
protected:
    virtual void startTest();
private:
    QComboBox *mode, *inversion, *oput;
    QLineEdit *startAddr, *endAddr, *startData, *deviceEdit;
    QSpinBox *incAddr, *incData, *iteration;
};

class RegTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit RegTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);
protected slots:
    virtual void save(bool);
protected:
    virtual void startTest();
};

class EchoTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit EchoTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);
protected slots:
    virtual void save(bool);
protected:
    virtual void startTest();
private:
    QLineEdit *echo, *deviceEdit;
};

namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices, QTextBrowser *tB);
AbstractTest *loadTest(QString file, QVBoxLayout* devices, QTextBrowser *tB);
}
#endif // ABSTRACTTEST_H
