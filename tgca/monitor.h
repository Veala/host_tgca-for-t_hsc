#ifndef MONITOR_H
#define MONITOR_H

#include <QDialog>
#include "device.h"

namespace Ui {
class Monitor;
}

class Monitor : public QDialog
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

    enum MonitorSignal
    {
        wait = 0,
        writeBC,
        writeRT,
        readBC,
        readRT
    };

    void setDevices(Device *dev1, Device *dev2);
    MonitorSignal *signalToMonitorTest;
    QByteArray *arr;
    bool *force_exit;

private:
    Ui::Monitor *ui;

    Device *device1, *device2;
    bool connection[2];

    QByteArray getRegistersToWrite();
    QByteArray getRegistersToRead();
    bool isSecondPage();
    void doneRead(CTableEditCol *tab, QByteArray readArray);
    bool initFromFile(QString, CTableEditCol *tab);
    bool initDefault(CONFIG_DEFAULT_TYPE key, word16_t param, CTableEditCol *tab);

private slots:
    void onChangeTab(int);
    bool onPushRead();
    bool onPushWrite();
    void onPushExit();
    bool onPushStore();
    void onPushLoad();
    void onExpand1(int);
    void onExpand2(int);
    void onCheckSelect1();
    void onCheckSelect2();
    void onCellChanged1(int, int);
    void onCellChanged2(int, int);
    void onSignalError(int);
    void onConnect(int);

    void forceExit();
    bool initFrom(QString, int*);

    void doneWriteBC();
    void doneWriteRT();
    void doneReadBC(QByteArray);
    void doneReadRT(QByteArray);

signals:
    //void monitorClosed();
};

#endif // MONITOR_H
