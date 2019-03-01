#ifndef MONITORTEST_H
#define MONITORTEST_H

#include "commontest.h"
#include "../monitor.h"

class MonitorTest : public CommonTest
{
    Q_OBJECT
public:
    explicit MonitorTest(QWidget *parent = 0) : CommonTest(parent) { }
    //~MonitorTest() { qDebug()<< "MonitorTest deleted"; }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    void save();
protected:
    void startTest();
    //void setEnabledSpecial(bool) {}

private slots:
    void printFin();
};

class monitorObjToThread : public commonObjToThread
{
    Q_OBJECT

    void prepTerminate();
public slots:
    void doWork();
public:
    monitorObjToThread();
    ~monitorObjToThread() {}
    Monitor::MonitorSignal signalFromMonitor;
    QByteArray writeArray;
    bool force_exit;

signals:
    void doneWriteBC();
    void doneWriteRT();
    void doneReadBC(QByteArray);
    void doneReadRT(QByteArray);
    void connected(int);
    void terminated();

protected:
    void perform() {}
protected slots:
    void terminate(int) {}
};

#endif // MONITORTEST_H
