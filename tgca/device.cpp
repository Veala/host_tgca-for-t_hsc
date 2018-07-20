#include "device.h"
#include "ui_device.h"

Device::Device(QWidget *parent, QString name) :
    QWidget(parent),
    ui(new Ui::Device)
{
    ui->setupUi(this);
    setName(name);
    QAction *act = menu.addAction(tr("Соединение"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showConnection(bool)));
    act = menu.addAction(tr("Конфигурация"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showConfiguration(bool)));

    connect(&connection, SIGNAL(connectTry(bool)), this, SLOT(connectTry(bool)));
    connect(&connection, SIGNAL(disconnectTry(bool)), this, SLOT(disconnectTry(bool)));
    connect(&rw_socket, SIGNAL(connected()), this, SLOT(doConnected()));
    connect(&rw_socket, SIGNAL(disconnected()), this, SLOT(doDisconnected()));
    connect(&rw_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));
    connect(&rw_socket, SIGNAL(readyRead()), this, SLOT(doReadyRead()));
}

Device::~Device()
{
    delete ui;
}

void Device::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        menu.exec(QCursor::pos());
    } else if (event->buttons() == Qt::LeftButton) {

    }
}

void Device::setName(QString name)
{
    ui->name->setText(name);
    connection.setName(name);
}

void Device::showConfiguration(bool)
{
    configuration.show();
}

void Device::showConnection(bool)
{
    connection.show();
}

void Device::connectTry(bool b)
{
    if (rw_socket.state() == QAbstractSocket::ConnectedState) {
        connection.doMessage(tr("Соединение установлено ранее"));
        return;
    }
    if (rw_socket.state() == QAbstractSocket::ConnectingState) {
        connection.doMessage(tr("Соединение устанавливается..."));
        return;
    }
    if (!rw_socket.bind(QHostAddress(connection.getHostIP())))
        return;
    rw_socket.connectToHost(QHostAddress(connection.getServerIP()), connection.getServerPORT().toUShort());
    if (!rw_socket.waitForConnected(5000))
        return;

    short cmd = 5;  short dsz = 4;
    char data[8];   char name[] = "Igor";
    strncpy(data, (char*)&cmd, 2);
    strncpy(&data[2], (char*)&dsz, 2);
    strncpy(&data[4], name, 4);
    rw_socket.write(data,8);
}

void Device::disconnectTry(bool b)
{
    if (rw_socket.state() == QAbstractSocket::UnconnectedState) {
        connection.doMessage(tr("Соединение разорвано ранее"));
        return;
    }
    rw_socket.disconnectFromHost();
    if (!(rw_socket.state() == QAbstractSocket::UnconnectedState ||
              rw_socket.waitForDisconnected(5000))) {
        connection.doError(rw_socket.errorString());
        return;
    }
}

void Device::doConnected()
{
    connection.doMessage(tr("Соединение установлено"));
    //На виджете добавить значек, что соединение есть.
}

void Device::doDisconnected()
{
    connection.doMessage(tr("Соединение разорвано"));
    //На виджете добавить значек, что соединения нет.
}

void Device::doError(QAbstractSocket::SocketError err)
{
    connection.doError(rw_socket.errorString());
    rw_socket.abort();
    emit error(err);
    //На виджете добавить значек, что соединения нет.
}

void Device::doReadyRead()
{

}
