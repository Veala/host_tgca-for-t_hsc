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
    connect(act, SIGNAL(triggered(bool)), &connection, SLOT(show()));
    act = menu.addAction(tr("Конфигурация"));
    connect(act, SIGNAL(triggered(bool)), &configuration, SLOT(show()));
    act = menu.addAction(tr("Удалить"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteLater()));

    connect(&connection, SIGNAL(checkDevice(bool)), this, SLOT(checkDevice()));
    //connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));

    setAutoFillBackground(true);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    QPalette palette;
    QBrush br(Qt::blue); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    ui->picture->setPixmap(QPixmap(tr(":/pictogram/hardware_8356.png")));
}

Device::~Device()
{
    //sock->abort();
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

bool Device::isMonitor()
{
    return ui->monitor->isChecked();
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

void Device::setSocket(QTcpSocket *s)
{
    sock = s;
}

void Device::readAll(char* array, int size)
{
//    array.clear();
//    array.resize(size);

    int n = 0; int r = 0;
    while (1) {
        r = sock->read(array+n, size-n);
        if (r!=0) {
#ifdef debug_AT
            qDebug() << "read: " << r;
            qDebug() << "n: " << n;
#endif
        }
        if (r == -1) {
            sock->abort();
            //emit resultReady((int)AbstractTest::ErrorIsOccured);
            throw QString("socket");
        } else if (r==0) {
#ifdef debug_AT
            qDebug() << "-------------if (r==0)";
#endif
            int msec = 3000;
            //if (n != 0) msec = 1;
            if (!sock->waitForReadyRead(msec)) {
#ifdef debug_AT
                qDebug() << "sock->waitForReadyRead(5000) error!";
#endif
                sock->abort();
                //emit resultReady((int)AbstractTest::ErrorIsOccured);
                throw QString("socket");
            }
        } else {
            n+=r;
            if (n>=size) return;
        }
    }
}

//void Device::readAll(QByteArray& array, int size)
//{
//    array.clear();
//    array.resize(size);

//    int n = 0; int r = 0;
//    while (1) {
//        r = sock->read(array.data()+n, size-n);
//        if (r!=0) {
//#ifdef debug_AT
//            qDebug() << "read: " << r;
//            qDebug() << "n: " << n;
//#endif
//        }
//        if (r == -1) {
//            sock->abort();
//            //emit resultReady((int)AbstractTest::ErrorIsOccured);
//            throw QString("socket");
//        } else if (r==0) {
//#ifdef debug_AT
//            qDebug() << "-------------if (r==0)";
//#endif
//            int msec = 3000;
//            //if (n != 0) msec = 1;
//            if (!sock->waitForReadyRead(msec)) {
//#ifdef debug_AT
//                qDebug() << "sock->waitForReadyRead(5000) error!";
//#endif
//                sock->abort();
//                //emit resultReady((int)AbstractTest::ErrorIsOccured);
//                throw QString("socket");
//            }
//        } else {
//            n+=r;
//            if (n>=size) return;
//        }
//    }
//}

void Device::writeAll(char* array, int size)
{
    char* temp = array;
    while (1) {
        int n = sock->write(temp, size);
#ifdef debug_AT
        qDebug() << "wrote: " << n;
#endif
        if (n == -1) {
            sock->abort();
            //emit resultReady((int)AbstractTest::ErrorIsOccured);
            throw QString("socket");
        } else  if (n < size) {
            size -=  n;
            temp = array+n;
            continue;
        } else if (n == size) {
            break;
        }
    }

    if (!sock->waitForBytesWritten(5000)) {
#ifdef debug_AT
        qDebug() << "sock->waitForBytesWritten(5000) error!";
#endif
        sock->abort();
        //emit resultReady((int)AbstractTest::ErrorIsOccured);
        throw QString("socket");
    }
}

//void Device::writeAll(QByteArray& array)
//{
//    char* temp = array.data();
//    int size = array.size();
//    while (1) {
//        int n = sock->write(temp, size);
//#ifdef debug_AT
//        qDebug() << "wrote: " << n;
//#endif
//        if (n == -1) {
//            sock->abort();
//            //emit resultReady((int)AbstractTest::ErrorIsOccured);
//            throw QString("socket");
//        } else  if (n < size) {
//            size -=  n;
//            temp = array.right(size).data();
//            continue;
//        } else if (n == size) {
//            break;
//        }
//    }

//    if (!sock->waitForBytesWritten(5000)) {
//#ifdef debug_AT
//        qDebug() << "sock->waitForBytesWritten(5000) error!";
//#endif
//        sock->abort();
//        //emit resultReady((int)AbstractTest::ErrorIsOccured);
//        throw QString("socket");
//    }
//}

void Device::write_F1(char* writeArray, int size)
{
    int cmd = 1;    QByteArray answer; answer.resize(4);
    writeAll(cmdHead(1, size).append(writeArray, size).data(), size+8);
//    writeAll(cmdHead(1, size).data(), 8);
//    writeAll(writeArray, size);
    readAll(answer.data(), 4);
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket");
    }
}

void Device::write_F2(int startAddr, char *writeArray, int size)
{
    int cmd = 2;    QByteArray answer; answer.resize(4);
    writeAll(cmdHead(2, size+4).data(), 8);
    writeAll((char*)&startAddr, 4);
    writeAll(writeArray, size);
    readAll(answer.data(), 4);
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket");
    }
}

void Device::write_Echo(QString &text)
{
    QByteArray writeArray = cmdHead(5, text.size()+1);
    writeArray.append(text.toStdString().c_str(), text.size()+1);
    int cmd = 5;    QByteArray answer; answer.resize(4);
    writeAll(writeArray.data(), writeArray.size());
    readAll(answer.data(), 4);
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket");
    }
}

void Device::read_F1(char *writeArray, char *readArray, int wrsize)
{
    writeAll(cmdHead(3, wrsize).append(writeArray, wrsize).data(), wrsize+8);
    //writeAll(cmdHead(3, wrsize).data(), 8);
    //writeAll(writeArray, wrsize);
    readAll(readArray, wrsize);
}

void Device::read_F2(int startAddr, int count, char *readArray)
{
    writeAll(cmdHead(4, 8).data(), 8);
    writeAll((char*)&startAddr, 4);
    writeAll((char*)&count, 4);
    readAll(readArray, count);
}

void Device::writeReg(BaseReg *reg)
{
    write_F1((char*)reg, RegLen);
}

void Device::readReg(BaseReg *reg)
{
    read_F1((char*)reg, (char*)reg+BaseRegLen, BaseRegLen);
}

void Device::writeRegs(QVector<BaseReg *>& regs)
{
    QByteArray array;
    foreach (BaseReg* r, regs) {
        array.append((char*)r, RegLen);
    }
    write_F1(array.data(), array.size());
}

void Device::readRegs(QVector<BaseReg *>& regs)
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

void Device::writeDataToMem(char *writeArray, int size)
{
    if ()

}

void Device::message(QString m)
{
    QDateTime local(QDateTime::currentDateTime());
    projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + ui->name->text() + ". " + m);
}

void Device::checkDevice()
{
    if (!sock->bind(QHostAddress(connection.getHostIP()))) {
        message(sock->errorString());
        return;
    }
    sock->connectToHost(QHostAddress(connection.getServerIP()), connection.getServerPORT().toUShort());
    if (!sock->waitForConnected(5000)) {
        message(sock->errorString());
        return;
    }
    sock->disconnectFromHost();
    if (!(sock->state() == QAbstractSocket::UnconnectedState ||
              sock->waitForDisconnected(5000))) {
        message(sock->errorString());
        return;
    }
    message(tr("Устройство найдено и готово к работе."));
}

void Device::doError(QAbstractSocket::SocketError err)
{
    message(tr("Ошибка: %1").arg(sock->errorString()));
    sock->abort();
}
