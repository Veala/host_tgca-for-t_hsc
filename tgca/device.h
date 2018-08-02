#ifndef DEVICE_H
#define DEVICE_H

#include <QWidget>
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

class Device : public QWidget
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
    QTcpSocket rw_socket;
    void setName(QString);
    QString getName() const;

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Device *ui;
    QMenu menu;
    QTextBrowser* textBrowser;
    void message(QString);

private slots:
    void deleteProc(bool);
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
