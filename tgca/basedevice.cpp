#include "basedevice.h"
#include "ui_basedevice.h"

BaseDevice::BaseDevice(QWidget *parent, QString name, QTextBrowser *tB) :
    QFrame(parent),
    ui(new Ui::BaseDevice)
{
    ui->setupUi(this);
    setName(name);
    projectBrowser = tB;
    QAction *act = menu.addAction(tr("Параметры tcp-ip"));
    connect(act, SIGNAL(triggered(bool)), &connection, SLOT(show()));
    act = menu.addAction(tr("Сохранить tcp-ip"));
    connect(act, SIGNAL(triggered(bool)), &connection, SLOT(save()));
    act = menu.addAction(tr("Конфигурация"));
    connect(act, SIGNAL(triggered(bool)), &configuration, SLOT(show()));
    act = menu.addAction(tr("Удалить"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(tryToDelete()));

    connect(&connection, SIGNAL(checkDevice(bool)), this, SLOT(checkDevice()));
    //connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));

    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    QPalette palette;
    QBrush br(Qt::blue); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    //ui->picture->setPixmap(QPixmap(tr(":/pictogram/hardware_8356.png")));
    ui->monitor->setEnabled(false);
    ui->monitor->setVisible(false);

    connect(this, SIGNAL(tcpInit()), &socketDriver, SLOT(init()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(stopAll()), &socketDriver, SLOT(stop()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(tcpConnect(QString,ushort)), &socketDriver, SLOT(tryToConnect(QString,ushort)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(tcpDisconnect()), &socketDriver, SLOT(tryToDisconnect()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(tcpExchange()), &socketDriver, SLOT(tryToExchange()), Qt::BlockingQueuedConnection);
    socketDriver.moveToThread(&toSocketDriver);
    toSocketDriver.setObjectName(name);
    socketDriver.setObjectName(name);
    toSocketDriver.start();
    emit tcpInit();
    socketDriver.mutex->lock();
    message(tr("Устройство добавлено"));
}

BaseDevice::~BaseDevice()
{
    //---------------------------------когда тесты работают - добавить корректное удаление девайса
    //sock->abort();
    qDebug() << "~BaseDevice() start";
    emit stopAll();
    socketDriver.mutex->unlock();
    toSocketDriver.quit();
    //toSocketDriver.terminate();
    toSocketDriver.wait();
    emit sigDelete(getName());
    qDebug() << "Device " << getName() << " deleted.";
    message(tr("Устройство удалено"));
    delete ui;
    qDebug() << "~BaseDevice() end";
}

void BaseDevice::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        menu.exec(QCursor::pos());
    } else if (event->buttons() == Qt::LeftButton) {

    }
}

bool BaseDevice::isMonitor()
{
    return ui->monitor->isChecked();
}

void BaseDevice::setName(QString name)
{
    ui->name->setText(name);
    connection.setName(name);
    toSocketDriver.setObjectName(name);
}

QString BaseDevice::getName() const
{
    return ui->name->text();
}

void BaseDevice::tryToConnect()
{
    socketDriver.mutex->unlock();
    emit tcpConnect(connection.getServerIP(), connection.getServerPORT().toUShort());
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("connection");
}

void BaseDevice::tryToDisconnect()
{
    emit tcpDisconnect();
}

void BaseDevice::write_F1(char* writeArray, int size)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::write_f1;
    socketDriver.allData.size = size;
    socketDriver.allData.writeArray = writeArray;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseDevice::write_F2(int startAddr, char *writeArray, int size)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::write_f2;
    socketDriver.allData.size = size;
    socketDriver.allData.writeArray = writeArray;
    socketDriver.allData.startAddr = startAddr;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseDevice::write_Echo(QString &text)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::write_echo;
    socketDriver.allData.text = text;
    socketDriver.allData.size = text.size()+1;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseDevice::read_F1(char *writeArray, char *readArray, int wrsize)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::read_f1;
    socketDriver.allData.writeArray = writeArray;
    socketDriver.allData.readArray = readArray;
    socketDriver.allData.size = wrsize;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseDevice::read_F2(int startAddr, int count, char *readArray)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::read_f2;
    socketDriver.allData.startAddr = startAddr;
    socketDriver.allData.count = count;
    socketDriver.allData.size = 8; //sizeof(count) + sizeof(startAddr)
    socketDriver.allData.readArray = readArray;
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseDevice::cpyOnHard(int fromAddr, int count, int toAddr)
{
    socketDriver.mutex->unlock();
    socketDriver.allData.format = ExchangeFormat::cpy_on_hard;
    socketDriver.allData.fromAddr = fromAddr;
    socketDriver.allData.count = count;
    socketDriver.allData.toAddr = toAddr;
    socketDriver.allData.size = 12; //sizeof(fromAddr) + sizeof(count) + sizeof(toAddr)
    emit tcpExchange();
    socketDriver.mutex->lock();
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void BaseDevice::writeReg(BaseReg *reg)
{
    write_F1((char*)reg, RegLen);
}

void BaseDevice::readReg(BaseReg *reg)
{
    read_F1((char*)reg, (char*)reg+BaseRegLen, BaseRegLen);
}

void BaseDevice::writeRegs(QVector<BaseReg *>& regs)
{
    QByteArray array;
    foreach (BaseReg* r, regs) {
        array.append((char*)r, RegLen);
    }
    write_F1(array.data(), array.size());
}

void BaseDevice::readRegs(QVector<BaseReg *>& regs)
{
    QByteArray array;
    QByteArray answer; answer.resize(regs.size()*BaseRegLen);
    foreach (BaseReg* r, regs) {
        array.append((char*)r, BaseRegLen);
    }
    read_F1(array.data(), answer.data(), regs.size()*BaseRegLen);
    for (int i=0; i<regs.size(); i++) {
        *((quint32*)regs[i]+1) = *((quint32*)answer.data()+i);
    }
}

void BaseDevice::message(QString m)
{
    QDateTime local(QDateTime::currentDateTime());
    projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + ui->name->text() + ". " + m);
}

void BaseDevice::checkDevice()
{
    QTcpSocket sock1;
    if (!sock1.bind(QHostAddress(connection.getHostIP()))) {
        message(sock1.errorString());
        return;
    }
    sock1.connectToHost(QHostAddress(connection.getServerIP()), connection.getServerPORT().toUShort());
    if (!sock1.waitForConnected(5000)) {
        message(QString(tr("Не удалось установить соединение: ")) + sock1.errorString());
        return;
    }
    sock1.disconnectFromHost();
    if (!(sock1.state() == QAbstractSocket::UnconnectedState ||
              sock1.waitForDisconnected(5000))) {
        message(tr("Устройство найдено."));
        message(QString(tr("Ошибка: ")) + sock1.errorString());
        return;
    }
    message(tr("Устройство найдено и готово к работе."));
}

void BaseDevice::doError(QAbstractSocket::SocketError err)
{
    //message(tr("Ошибка: %1").arg());
}

void BaseDevice::tryToDelete()
{
    if (getGlobalState() == FREE) {
        deleteLater();
    }
    else {
        message(tr("Ошибка: устройство не может быть удалено при работающих тестах."));
    }
}
