#include "device.h"
#include "ui_device.h"

Device::Device(QWidget *parent, QString name, QTextBrowser *tB) :
    QFrame(parent),
    ui(new Ui::Device)
{
    ui->setupUi(this);
    setName(name);
    textBrowser = tB;
    QAction *act = menu.addAction(tr("Соединение"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showConnection(bool)));
    act = menu.addAction(tr("Конфигурация"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showConfiguration(bool)));
    act = menu.addAction(tr("Удалить"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteProc(bool)));

    connect(&connection, SIGNAL(connectTry(bool)), this, SLOT(connectTry(bool)));
    connect(&connection, SIGNAL(disconnectTry(bool)), this, SLOT(disconnectTry(bool)));
    connect(&rw_socket, SIGNAL(connected()), this, SLOT(doConnected()));
    connect(&rw_socket, SIGNAL(disconnected()), this, SLOT(doDisconnected()));
    connect(&rw_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));
    connect(&rw_socket, SIGNAL(readyRead()), this, SLOT(doReadyRead()));

//    background-color: rgb(164, 164, 220);
//    border-color: rgb(11, 4, 4);

    setAutoFillBackground(true);
    setConnectedState(disconned);
    setFrameStyle(QFrame::Box | QFrame::Plain);
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

void Device::setConnectedState(Device::ConnectedState cs)
{
    QPalette palette;
    if (cs == conned) {
        QBrush br(Qt::blue); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    } else if (cs == conning) {
        QBrush br(Qt::darkGray); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    } else if (cs == disconned) {
        QBrush br(Qt::gray); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
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
    textBrowser->append(ui->name->text() + local.toString(tr(" - dd.MM.yyyy hh:mm:ss\n")) + m);
}

void Device::deleteProc(bool)
{
    rw_socket.abort();
    emit sigDelete(getName());
    deleteLater();
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

void Device::disconnectTry(bool b)
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

void Device::doReadyRead()
{

}
