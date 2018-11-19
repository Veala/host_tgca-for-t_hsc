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

    littleAnswer.resize(4);

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

void Device::write_F1(char* writeArray, int size)
{
    int cmd = 1;
    writeAll(cmdHead(1, size).append(writeArray, size).data(), size+8);
//    writeAll(cmdHead(1, size).data(), 8);
//    writeAll(writeArray, size);
    readAll(littleAnswer.data(), 4);
    if (*(int*)littleAnswer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)littleAnswer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket");
    }
}

void Device::write_F2(int startAddr, char *writeArray, int size)
{
    int cmd = 2;
    writeAll(cmdHead(2, size+4).data(), 8);
    writeAll((char*)&startAddr, 4);
    writeAll(writeArray, size);
    readAll(littleAnswer.data(), 4);
    if (*(int*)littleAnswer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)littleAnswer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket");
    }
}

void Device::write_Echo(QString &text)
{
    QByteArray writeArray = cmdHead(5, text.size()+1);
    writeArray.append(text.toStdString().c_str(), text.size()+1);
    int cmd = 5;
    writeAll(writeArray.data(), writeArray.size());
    readAll(littleAnswer.data(), 4);
    if (*(int*)littleAnswer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)littleAnswer.data() != cmd";
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

void Device::sendDataToRT(quint32 addrRT, char *writeArray, int size)
{
    commandWord.rtaddr = addrRT;
    commandWord.t_r = 0;
    int SL = symbolLength();
    int DL = size + sizeof(commandWord)/2;
    int Ns = DL/SL + (DL%SL != 0 ? 1 : 0);
    int Np = Ns/maxNumSymbols + (Ns%maxNumSymbols != 0 ? 1 : 0);
    for (int i=1; i<=Np; i++) {
        readReg(reg_hsc_status);
        int curAddr;
        if (reg_hsc_status.tx_num_buf == 0)
            curAddr = ADDR_MEM2P_BUF_TX_0;
        else if (reg_hsc_status.tx_num_buf == 1)
            curAddr = ADDR_MEM2P_BUF_TX_1;
        commandWord.addr = curAddr;

        int realSymbolLength;

        if (i==0) {
            curAddr+=sizeof(commandWord)/2;

        } else if ()

        write_F1();
    }
}

int Device::symbolLength()
{
    if (reg_hsc_cfg.ena_codec == 0 && reg_hsc_cfg.type_man == VAL_reg_hsc_cfg_type_man_QPSK)    return 112; //28*4=112
    if (reg_hsc_cfg.ena_codec == 0 && reg_hsc_cfg.type_man == VAL_reg_hsc_cfg_type_man_QAM16)   return 224; //56*4=224
    if (reg_hsc_cfg.ena_codec == 0 && reg_hsc_cfg.type_man == VAL_reg_hsc_cfg_type_man_QAM64)   return 336; //84*4=336
    if (reg_hsc_cfg.ena_codec == 1 && reg_hsc_cfg.type_man == VAL_reg_hsc_cfg_type_man_QPSK)    return 96;  //24*4=96
    if (reg_hsc_cfg.ena_codec == 1 && reg_hsc_cfg.type_man == VAL_reg_hsc_cfg_type_man_QAM16)   return 176; //44*4=176
    if (reg_hsc_cfg.ena_codec == 1 && reg_hsc_cfg.type_man == VAL_reg_hsc_cfg_type_man_QAM64)   return 224; //56*4=224
    throw QString("codec_or_type_man_incorrect");
}

void Device::writeDataToMem(char *writeArray, int size)
{
    int SL = symbolLength();
    int DL = size + sizeof(commandWord)/2;
    int Ns = DL/SL + (DL%SL != 0 ? 1 : 0);
    int Np = Ns/maxNumSymbols + (Ns%maxNumSymbols != 0 ? 1 : 0);
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
