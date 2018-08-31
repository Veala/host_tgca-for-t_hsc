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
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB);
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
    ~monitorObjToThread();
    int write_size_BC, write_size_RT, read_size_BC, read_size_RT;
    QByteArray array;
    bool force_exit;

    QTcpSocket tcpSocketBC, tcpSocketRT;
    Device *devBC, *devRT;

signals:
    void doneWriteBC();
};

#endif // MONITORTEST_H
