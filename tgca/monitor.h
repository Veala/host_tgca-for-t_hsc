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

    void setDevice(Device *dev1, Device *dev2);
    int *writeBC, *writeRT, *readBC, *readRT;
    QByteArray *arr;
    bool *force_exit;

private:
    Ui::Monitor *ui;

    Device *device1, *device2;

private slots:
    void onChangeTab(int);
    bool onPushRead();
    bool onPushWrite();
    void onPushExit();
    bool onPushStore();
    bool onPushLoad();
    void onExpand1(int);
    void onExpand2(int);
    void onCheckSelect1();
    void onCheckSelect2();
    void onCellChanged1(int, int);
    void onCellChanged2(int, int);
    void onExit(int);
    void onSignalError(int);

    void forceExit();

public slots:
    void doneWriteBC();

signals:
    //void monitorClosed();
};

#endif // MONITOR_H
