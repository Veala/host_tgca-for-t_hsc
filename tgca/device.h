#ifndef DEVICE_H
#define DEVICE_H

#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "connection.h"
#include "configuration.h"

namespace Ui {
class Device;
}

class Device : public QWidget
{
    Q_OBJECT

signals:
    void error(QAbstractSocket::SocketError err);

public:
    explicit Device(QWidget *parent = 0, QString name = "default");
    ~Device();

    Connection connection;
    Configuration configuration;
    QTcpSocket rw_socket;

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Device *ui;
    QMenu menu;
    void setName(QString);

private slots:
    void showConfiguration(bool);
    void showConnection(bool);
    void connectTry(bool);
    void disconnectTry(bool);
    void doConnected();
    void doDisconnected();
    void doError(QAbstractSocket::SocketError err);
    void doReadyRead();
};

#endif // DEVICE_H