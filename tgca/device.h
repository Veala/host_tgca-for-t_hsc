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

namespace Ui {
class Device;
}

class Device : public QFrame
{
    Q_OBJECT

signals:
    void error(QAbstractSocket::SocketError err);
    //signals to tests

    void sigDelete(QString);
    void sigConnectedDevice();
    void sigDisconnectedDevice();

    void  doneWriteReg(int);
    void  doneReadReg(int,QByteArray);

public:
    explicit Device(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    ~Device();

    Connection connection;
    Configuration configuration;
    QTcpSocket rw_socket;
    void setName(QString);
    QString getName() const;

    enum ConnectedState {
        conned,
        conning,
        disconned
    };

protected:
    void mousePressEvent(QMouseEvent *event);

    void setConnectedState(ConnectedState);
    ConnectedState getConnectedState() const;

private:
    Ui::Device *ui;
    QMenu menu;
    QTextBrowser* projectBrowser;
    void message(QString);
    ConnectedState connectedState;

private slots:
    void showConfiguration();
    void showConnection();
    void connectTry();
    void configTry();
    void disconnectTry();
    void doConnected();
    void doDisconnected();
    void doError(QAbstractSocket::SocketError err);
    void doWriteReg(QByteArray);
    void doReadReg(QByteArray);
};

#endif // DEVICE_H
