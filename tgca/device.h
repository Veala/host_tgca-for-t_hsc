#ifndef DEVICE_H
#define DEVICE_H

#include <QFrame>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTextBrowser>
#include <QVector>
#include "connection.h"
#include "configuration.h"
#include "funclib.h"
#include "data_structs.h"

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

    bool isMonitor();
    void setName(QString);
    QString getName() const;
    void setSocket(QTcpSocket*);

    void write_F1(QByteArray& writeArray);
    void write_F2(QByteArray& writeArray);
    void write_Echo(QString& text);
    void read_F1(QByteArray& writeArray, QByteArray& readArray);
    void read_F2(QByteArray& writeArray, QByteArray& readArray);
    void writeReg(BaseReg* reg);
    void readReg(BaseReg* reg);
    void writeRegs(QVector<BaseReg*>& regs);
    void readRegs(QVector<BaseReg*>& regs);
    //int readReg(int addr, int *val);
protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Device *ui;
    QMenu menu;
    QTextBrowser* projectBrowser;
    QTcpSocket* sock;
    void message(QString);
    void readAll(QByteArray&, int);
    void writeAll(QByteArray&);


private slots:
    void showConfiguration();
    void showConnection();
    void checkDevice();
    void doError(QAbstractSocket::SocketError err);
};

#endif // DEVICE_H
