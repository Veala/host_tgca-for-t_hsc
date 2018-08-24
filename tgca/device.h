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

public:
    explicit Device(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    ~Device();

    Connection connection;
    Configuration configuration;
    QTcpSocket socket;

    bool isMonitor();
    void setName(QString);
    QString getName() const;

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Device *ui;
    QMenu menu;
    QTextBrowser* projectBrowser;
    void message(QString);

private slots:
    void showConfiguration();
    void showConnection();
    void checkDevice();
    void doError(QAbstractSocket::SocketError err);
};

#endif // DEVICE_H
