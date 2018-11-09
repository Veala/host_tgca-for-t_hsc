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

void Device::readAll(QByteArray& array, int size)
{
    array.clear();
    array.resize(size);

    int n = 0; int r = 0;
    while (1) {
        r = sock->read(array.data()+n, size-n);
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

void Device::writeAll(QByteArray& array)
{
    char* temp = array.data();
    int size = array.size();
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
            temp = array.right(size).data();
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

void Device::write_F1(QByteArray &writeArray)
{
    int cmd = 1;    QByteArray answer;
    writeAll(writeArray);
    readAll(answer, 4);
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket");
    }
}

void Device::write_F2(QByteArray &writeArray)
{
    int cmd = 2;    QByteArray answer;
    writeAll(writeArray);
    readAll(answer, 4);
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
    int cmd = 5;    QByteArray answer;
    writeAll(writeArray);
    readAll(answer, 4);
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket");
    }
}

void Device::read_F1(QByteArray &writeArray, QByteArray &readArray)
{
    writeAll(writeArray);
    readAll(readArray, writeArray.size()-8);
}

void Device::writeReg(BaseReg *reg)
{
    int len = sizeof(BaseReg);
    QByteArray array = cmdHead(1, 2*len);
    array.append((char*)reg, 2*len);
    write_F1(array);
}

void Device::readReg(BaseReg *reg)
{
    int len = sizeof(BaseReg);
    QByteArray array = cmdHead(3, len);
    QByteArray answer;
    array.append((char*)reg, len);
    read_F1(array, answer);
    *((quint32*)reg+1) = *(quint32*)answer.data();
}

void Device::writeRegs(QVector<BaseReg *>& regs)
{
    int len = sizeof(BaseReg);
    QByteArray array = cmdHead(1, regs.size()*2*len);
    foreach (BaseReg* r, regs) {
        array.append((char*)r, 2*len);
    }
    write_F1(array);
}

void Device::readRegs(QVector<BaseReg *>& regs)
{
    int len = sizeof(BaseReg);
    QByteArray array = cmdHead(3, regs.size()*len);
    QByteArray answer;
    foreach (BaseReg* r, regs) {
        array.append((char*)r, len);
    }
    read_F1(array, answer);
    for (int i=0; i<regs.size(); i++) {
        *((quint32*)regs[i]+1) = *((quint32*)answer.data()+i);
    }
}

void Device::read_F2(QByteArray &writeArray, QByteArray &readArray)
{
    writeAll(writeArray);
    int nr = *(int*)(writeArray.data()+12);
    readAll(readArray, nr);
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

//int Device::readReg(int addr, int *val)
//{
//    QByteArray answer;
//    QByteArray readArray = cmdHead(3, 4);
//    readArray.append((char*)&addr, 4);
//    if (read_F1(readArray, answer) == -1)
//        return -1;

//    *val = *(int*)(answer.data());
//    return 0;
//}
