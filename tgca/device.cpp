#include "device.h"
#include "ui_device.h"

Device::Device(QWidget *parent, QString name, QTextBrowser *tB) :
    QFrame(parent),
    ui(new Ui::Device)
{
    ui->setupUi(this);
    setName(name);
    projectBrowser = tB;
    QAction *act = menu.addAction(tr("Параметры tcp-ip"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showConnection()));
    act = menu.addAction(tr("Конфигурация"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showConfiguration()));
    act = menu.addAction(tr("Удалить"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteLater()));

//    connect(&connection, SIGNAL(connectTry(bool)), this, SLOT(connectTry()));
//    connect(&connection, SIGNAL(disconnectTry(bool)), this, SLOT(disconnectTry()));
//    connect(&rw_socket, SIGNAL(connected()), this, SLOT(doConnected()));
//    connect(&rw_socket, SIGNAL(disconnected()), this, SLOT(doDisconnected()));
//    connect(&rw_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));
    connect(&configuration, SIGNAL(doWriteReg(QByteArray)), this, SLOT(doWriteReg(QByteArray)));
    connect(&configuration, SIGNAL(doReadReg(QByteArray)), this, SLOT(doReadReg(QByteArray)));
    connect(this, SIGNAL(doneWriteReg(int)), &configuration, SLOT(doneWriteReg(int)));
    connect(this, SIGNAL(doneReadReg(int,QByteArray)), &configuration, SLOT(doneReadReg(int,QByteArray)));
    connect(this, SIGNAL(sigConnectedDevice()), &configuration, SLOT(enableReadWrite()));
    connect(this, SIGNAL(sigDisconnectedDevice()), &configuration, SLOT(blockReadWrite()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), &configuration, SLOT(blockReadWrite()));

//    background-color: rgb(164, 164, 220);
//    border-color: rgb(11, 4, 4);

    setAutoFillBackground(true);
    setConnectedState(conned);
    setFrameStyle(QFrame::Box | QFrame::Plain);
}

Device::~Device()
{
    //qDebug() << "~Device()" << getName();
    rw_socket.abort();
    emit sigDelete(getName());
    delete ui;
}

void Device::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        menu.exec(QCursor::pos());
    } else if (event->buttons() == Qt::LeftButton) {

    }
}

void Device::setConnectedState(Device::ConnectedState cs)
{
    QPalette palette;
    if (cs == conned) {
        QBrush br(Qt::blue); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
        ui->picture->setPixmap(QPixmap(tr(":/pictogram/connect_3497.png")));
    } else if (cs == conning) {
        QBrush br(Qt::yellow); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
        ui->picture->setPixmap(QPixmap(tr(":/pictogram/calculator_8158.png")));
    } else if (cs == disconned) {
        QBrush br(Qt::gray); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
        ui->picture->setPixmap(QPixmap(tr(":/pictogram/disconnect_9550.png")));
    }
    connectedState = cs;
}

Device::ConnectedState Device::getConnectedState() const
{
    return connectedState;
}

void Device::setName(QString name)
{
    ui->name->setText(name);
    connection.setName(name);
}

QString Device::getName() const
{
    return ui->name->text();
}

void Device::message(QString m)
{
    QDateTime local(QDateTime::currentDateTime());
    projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + ui->name->text() + ". " + m);
}

void Device::showConfiguration()
{
    configuration.show();
}

void Device::showConnection()
{
    connection.show();
}

void Device::connectTry()
{
    if (rw_socket.state() == QAbstractSocket::ConnectedState) {
        message(tr("Соединение установлено ранее"));
        return;
    }
    if (rw_socket.state() == QAbstractSocket::ConnectingState) {
        message(tr("Соединение устанавливается..."));
        setConnectedState(conning);
        return;
    }
    if (!rw_socket.bind(QHostAddress(connection.getHostIP())))
        return;
    rw_socket.connectToHost(QHostAddress(connection.getServerIP()), connection.getServerPORT().toUShort());
    if (!rw_socket.waitForConnected(5000))
        return;
}

void Device::configTry()
{
    return;
    if (rw_socket.state() == QAbstractSocket::ConnectedState) {
        configuration.onPushWrite();
    }
}

void Device::disconnectTry()
{
    if (rw_socket.state() == QAbstractSocket::UnconnectedState) {
        message(tr("Соединение разорвано ранее"));
        return;
    }
    rw_socket.disconnectFromHost();
    if (!(rw_socket.state() == QAbstractSocket::UnconnectedState ||
              rw_socket.waitForDisconnected(5000))) {
        message(rw_socket.errorString());
        return;
    }
}

void Device::doConnected()
{
    message(tr("Соединение установлено"));
    setConnectedState(conned);
    emit sigConnectedDevice();
    //На виджете добавить значек, что соединение есть.
}

void Device::doDisconnected()
{
    message(tr("Соединение разорвано"));
    setConnectedState(disconned);
    emit sigDisconnectedDevice();
    //На виджете добавить значек, что соединения нет.
}

void Device::doError(QAbstractSocket::SocketError err)
{
    message(tr("Ошибка: %1").arg(rw_socket.errorString()));
    rw_socket.abort();
    emit error(err);
    //На виджете добавить значек, что соединения нет.
}

/// Функция записи регистров устройства. Сигнал возвращает код ошибки.
void  Device::doWriteReg(QByteArray array)
{
    //return;

    int error_code = -1;
    if (rw_socket.state() == QAbstractSocket::ConnectedState) {
        /*char *pt = array.data();
        int sz = array.size();
        for (int i=0; i<sz; i+=4, pt+=4)
        {
            uint data;
            dev->rw_socket.read((char*)&data, 4);
            int i1 = */

        error_code = rw_socket.write(array);
        qDebug() << "wrote: " << error_code;
        if (error_code != -1)
        {
            if (rw_socket.waitForBytesWritten(5000)) {
                error_code = 0;
            }
            else
            {
                rw_socket.abort();
                error_code = -2;
            }
        }
    }
    qDebug() << "error_code = " << error_code;
    emit doneWriteReg(error_code);
}

/// Функция чтения регистров устройства в array. Сигнал возвращает код ошибки.
void  Device::doReadReg(QByteArray array)
{
    qDebug() << "slot doReadReg() is not implemented";
    emit doneReadReg(-1, array);
}
