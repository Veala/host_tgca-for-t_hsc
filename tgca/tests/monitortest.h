#ifndef MONITORTEST_H
#define MONITORTEST_H

#include "abstracttest.h"
#include "../monitor.h"

class MonitorTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit MonitorTest(QWidget *parent = 0) : AbstractTest(parent) { }
    //~MonitorTest() { qDebug()<< "MonitorTest deleted"; }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    void save();
protected:
    void startTest();

private:
    QLineEdit *lineEditDevBC;
    QLineEdit *lineEditDevRT;
    QCheckBox *checkBoxDevBC;
    QCheckBox *checkBoxDevRT;
    void updateDeviceList();
private slots:
    void updateSettings();
    void printFin();
signals:
    void settingsClosed(int);
};

class monitorObjToThread : public absObjToThread
{
    Q_OBJECT

    void terminate();
public slots:
    virtual void doWork();
public:
    monitorObjToThread();
    ~monitorObjToThread() {}
    Monitor::MonitorSignal signalFromMonitor;
    QByteArray writeArray;
    bool force_exit;

    QTcpSocket tcpSocketBC, tcpSocketRT;
    Device *devBC, *devRT;

signals:
    void doneWriteBC();
    void doneWriteRT();
    void doneReadBC(QByteArray);
    void doneReadRT(QByteArray);
    void connected(int);
    void terminated();
};

#endif // MONITORTEST_H
