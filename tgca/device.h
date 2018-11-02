#ifndef DEVICE_H
#define DEVICE_H

#include <QFrame>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTextBrowser>
#include "connection.h"
#include "configuration.h"
#include "funclib.h"

namespace Ui {
class Device;
}

class Device : public QFrame//, public DeviceDriver
{
    Q_OBJECT

signals:
    void error(QAbstractSocket::SocketError err);

    //signals to tests
    void sigDelete(QString);
    void sigConnectedDevice();
    void sigDisconnectedDevice();

public:
    explicit Device(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    ~Device();

    Connection connection;
    Configuration configuration;

    bool isMonitor();
    void setName(QString);
    QString getName() const;
    void setSocket(QTcpSocket*);

    int write_F1(QByteArray& writeArray);
    int write_F2(QByteArray& writeArray);
    int write_Echo(QByteArray& writeArray);
    int read_F1(QByteArray& writeArray, QByteArray& readArray);
    int read_F2(QByteArray& writeArray, QByteArray& readArray);
    int readReg(int addr, int *val);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Device *ui;
    QMenu menu;
    QTextBrowser* projectBrowser;
    QTcpSocket* sock;
    void message(QString);
    int readAll(QByteArray&, int);
    int writeAll(QByteArray&);


private slots:
    void showConfiguration();
    void showConnection();
    void checkDevice();
    void doError(QAbstractSocket::SocketError err);
};

#endif // DEVICE_H
