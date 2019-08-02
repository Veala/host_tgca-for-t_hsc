#ifndef LSCGATEWAY_H
#define LSCGATEWAY_H

#include <QThread>

class Device;

class LSCGatewayThread : public QThread
{
    Q_OBJECT
    Device* dev;
public:
    explicit LSCGatewayThread(QObject *parent = 0) : QThread(parent) { }
    void setDevice(Device* d)  { dev = d; }
    QString state;
protected:
    virtual void run() Q_DECL_OVERRIDE;
};

#endif // LSCGATEWAY_H
