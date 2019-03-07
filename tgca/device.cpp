#include "device.h"
#include "ui_device.h"
//#define debug_AT
//#define QT_NO_DEBUG_OUTPUT

//QMutex mutex; //сделать в классе

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
    //qDebug() << "Device(): after toSocketDriver.start();";
    emit tcpInit();
    //qDebug() << "Device(): after emit tcpInit();";
}

Device::~Device()
{
    //---------------------------------когда тесты работают - добавить корректное удаление
    //sock->abort();
    //emit stop
    socketDriver.mutex->unlock();
    toSocketDriver.quit();
    toSocketDriver.wait();
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
    toSocketDriver.setObjectName(name);
}

QString Device::getName() const
{
    return ui->name->text();
}

void Device::tryToConnect()
{
    //qDebug() << "tryToConnect(): begin";
    socketDriver.mutex->unlock();
    //qDebug() << "tryToConnect(): after mutex.unlock()";
    //qDebug() << "tryToConnect(): before emit tcpConnect();";
    emit tcpConnect(connection.getServerIP(), connection.getServerPORT().toUShort());
    //qDebug() << "tryToConnect(): before mutex.lock(); after emit tcpConnect();";
    socketDriver.mutex->lock();
    //qDebug() << "tryToConnect(): after mutex.lock();";
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("connection");
}

void Device::tryToDisconnect()
{
//    qDebug() << "tryToDisconnect(): begin";
//    mutex.unlock();
//    qDebug() << "tryToDisconnect(): after mutex.unlock()";
//    qDebug() << "tryToDisconnect(): before emit tcpDisconnect();";
    emit tcpDisconnect();
//    qDebug() << "tryToDisconnect(): before mutex.lock(); after emit tcpDisconnect();";
//    mutex.lock();
//    qDebug() << "tryToDisconnect(): after mutex.lock();";
}

void Device::setSocket(QTcpSocket *)
{

}

/*
void Device::readAll(char* array, int size)
{
    int n = 0; int r = 0;
    while (1) {
        r = sock->read(array+n, size-n);
        if (r!=0) {
#ifdef debug_AT
            //qDebug() << "read: " << r;
            //qDebug() << "n: " << n;
#endif
        }
        if (r == -1) {
            sock->abort();
            //emit resultReady((int)AbstractTest::ErrorIsOccured);
            throw QString("socket read");
        } else if (r==0) {
#ifdef debug_AT
            //qDebug() << "-------------if (r==0)";
#endif
            int msec = 10000;
            //if (n != 0) msec = 1;
            if (!sock->waitForReadyRead(msec)) {
#ifdef debug_AT
                //qDebug() << "sock->waitForReadyRead(5000) error!";
#endif
                sock->abort();
                //emit resultReady((int)AbstractTest::ErrorIsOccured);
                throw QString("socket waitForReadyRead %1").arg(msec);
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
        //qDebug() << "wrote: " << n;
#endif
        if (n == -1) {
            sock->abort();
            //emit resultReady((int)AbstractTest::ErrorIsOccured);
            throw QString("socket write");
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
        //qDebug() << "sock->waitForBytesWritten(5000) error!";
#endif
        sock->abort();
        //emit resultReady((int)AbstractTest::ErrorIsOccured);
        throw QString("socket waitForBytesWritten");
    }
}

*/

/*

void Device::getLittleAnswer(int cmdCode)
{
    readAll(littleAnswer.data(), 4);
    if (*(int*)littleAnswer.data() != cmdCode) {
#ifdef debug_AT
        //qDebug() << "(int*)littleAnswer.data() != cmd";
#endif
        sock->abort();
        throw QString("socket getLittleAnswer");
    }
}

*/

void Device::write_F1(char* writeArray, int size)
{
    //qDebug() << "write_F1(): begin";
    socketDriver.mutex->unlock();
    //qDebug() << "write_F1(): after mutex.unlock()";
    socketDriver.allData.format = ExchangeFormat::write_f1;
    socketDriver.allData.size = size;
    socketDriver.allData.writeArray = writeArray;
    //qDebug() << "write_F1(): before emit tcpExchange();";
    emit tcpExchange();
    //qDebug() << "write_F1(): before mutex.lock(); after emit tcpExchange();";
    socketDriver.mutex->lock();
    //qDebug() << "write_F1(): after mutex.lock();";
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void Device::write_F2(int startAddr, char *writeArray, int size)
{
    //qDebug() << "write_F2(): begin";
    socketDriver.mutex->unlock();
    //qDebug() << "write_F2(): after mutex.unlock()";
    socketDriver.allData.format = ExchangeFormat::write_f2;
    socketDriver.allData.size = size;
    socketDriver.allData.writeArray = writeArray;
    socketDriver.allData.startAddr = startAddr;
    //qDebug() << "write_F2(): before emit tcpExchange();";
    emit tcpExchange();
    //qDebug() << "write_F2(): before mutex.lock(); after emit tcpExchange();";
    socketDriver.mutex->lock();
    //qDebug() << "write_F2(): after mutex.lock();";
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void Device::write_Echo(QString &text)
{
    //qDebug() << "write_echo(): begin";
    socketDriver.mutex->unlock();
    //qDebug() << "write_echo(): after mutex.unlock()";
    socketDriver.allData.format = ExchangeFormat::write_echo;
    socketDriver.allData.text = text;
    socketDriver.allData.size = text.size()+1;
    //qDebug() << "write_echo(): before emit tcpExchange();";
    emit tcpExchange();
    //qDebug() << "write_echo(): before mutex.lock(); after emit tcpExchange();";
    socketDriver.mutex->lock();
    //qDebug() << "write_echo(): after mutex.lock();";
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void Device::read_F1(char *writeArray, char *readArray, int wrsize)
{
    //qDebug() << "read_F1(): begin";
    socketDriver.mutex->unlock();
    //qDebug() << "read_F1(): after mutex.unlock()";
    socketDriver.allData.format = ExchangeFormat::read_f1;
    socketDriver.allData.writeArray = writeArray;
    socketDriver.allData.readArray = readArray;
    socketDriver.allData.size = wrsize;
    //qDebug() << "read_F1(): before emit tcpExchange();";
    emit tcpExchange();
    //qDebug() << "read_F1(): before mutex.lock(); after emit tcpExchange();";
    socketDriver.mutex->lock();
    //qDebug() << "read_F1(): after mutex.lock();";
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void Device::read_F2(int startAddr, int count, char *readArray)
{
    //qDebug() << "read_F2(): begin";
    socketDriver.mutex->unlock();
    //qDebug() << "read_F2(): after mutex.unlock()";
    socketDriver.allData.format = ExchangeFormat::read_f2;
    socketDriver.allData.startAddr = startAddr;
    socketDriver.allData.count = count;
    socketDriver.allData.size = 8; //sizeof(count) + sizeof(startAddr)
    socketDriver.allData.readArray = readArray;
    //qDebug() << "read_F2(): before emit tcpExchange();";
    emit tcpExchange();
    //qDebug() << "read_F2(): before mutex.lock(); after emit tcpExchange();";
    socketDriver.mutex->lock();
    //qDebug() << "read_F2(): after mutex.lock();";
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
}

void Device::cpyOnHard(int fromAddr, int count, int toAddr)
{
    //qDebug() << "cpyOnHard(): begin";
    socketDriver.mutex->unlock();
    //qDebug() << "cpyOnHard(): after mutex.unlock()";
    socketDriver.allData.format = ExchangeFormat::cpy_on_hard;
    socketDriver.allData.fromAddr = fromAddr;
    socketDriver.allData.count = count;
    socketDriver.allData.toAddr = toAddr;
    socketDriver.allData.size = 12; //sizeof(fromAddr) + sizeof(count) + sizeof(toAddr)
    //qDebug() << "cpyOnHard(): before emit tcpExchange();";
    emit tcpExchange();
    //qDebug() << "cpyOnHard(): before mutex.lock(); after emit tcpExchange();";
    socketDriver.mutex->lock();
    //qDebug() << "cpyOnHard(): after mutex.lock();";
    if (socketDriver.getSocketState() != QAbstractSocket::ConnectedState)
        throw QString("socket");
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

/*
void Device::sendDataToRT(quint32 addrRT, char *writeArray, int size)
{
    commandWord.rtaddr = addrRT;
    commandWord.t_r = 0;
    int CL = sizeof(commandWord)/2;
    int SL = symbolLength();                                        //длина одного символа в байтах
    int DL = size + CL;                                             //длина всех данных с командным словом в байтах
    int Ns = DL/SL + (DL%SL != 0 ? 1 : 0);                          //общее кол-во символов
    int Np = Ns/maxNumSymbols + (Ns%maxNumSymbols != 0 ? 1 : 0);    //общее кол-во пакетов
    int d = Ns*SL-DL;                                               //кол-во нулей в байтах
    char oneNull = 0b00000000;  QByteArray nulls(d, oneNull);
    int Nsp;                                                        //кол-во символов в пакете
    int curAddr;
    int remainNs = Ns-(Np-1)*maxNumSymbols;                         //кол-во символов в последнем пакете
    int lastSLwithoutNulls = SL-d;                                  //кол-во байт данных в последнем символе без нулей
    char* curArrayPointer = writeArray;

    for (int i=1; i<=Np; i++) {
        readReg(reg_hsc_status);
        if (reg_hsc_status.tx_num_buf == 0)         curAddr = ADDR_MEM2P_BUF_TX_0;
        else if (reg_hsc_status.tx_num_buf == 1)    curAddr = ADDR_MEM2P_BUF_TX_1;
        commandWord.addr = curAddr;
        if (Np!=i) Nsp = maxNumSymbols; else Nsp = remainNs;
        commandWord.num_of_symbols=Nsp;
        for (int j=1; j<=Nsp; j++, curAddr+=ADDR_MEM2P_DELTA) {
            if (j==1 && j!=Nsp) {
                write_F1((char*)&commandWord, 2*CL);
                write_F2(curAddr+CL, curArrayPointer, SL-CL);
                curArrayPointer+=SL-CL;
            } else if (j==1 && j==Nsp) {
                write_F1((char*)&commandWord, 2*CL);
                write_F2(curAddr+CL, curArrayPointer, lastSLwithoutNulls-CL);
                write_F2(curAddr+sizeof(commandWord)/2, , lastSLwithoutNulls-sizeof(commandWord)/2);
            } else if () {

            }
        }
    }
}

void Device::sendDataToRT(quint32 addrRT, char *writeArray, int size)
{
    commandWord.rtaddr = addrRT;
    commandWord.t_r = 0;
    int nextSize = size + sizeof(quint32);                          //плюс в начало к данным ихний размер, для понимания оконечником того, сколько нулей в конце(следующий за cmdHead протокол)
    char* curArrayPointer = writeArray;
    int CL = sizeof(commandWord)/2;                                 //длина командного слова в байтах
    int SL = symbolLength();                                        //длина одного символа в байтах
    int DL = nextSize + CL;                                         //длина всех данных с командным словом в байтах без нулей
    int Ns = DL/SL + (DL%SL != 0 ? 1 : 0);                          //общее кол-во символов
    int Np = Ns/maxNumSymbols + (Ns%maxNumSymbols != 0 ? 1 : 0);    //общее кол-во пакетов
    int d = Ns*SL-DL;                                               //кол-во нулей в байтах
    int Nsp = maxNumSymbols;                                        //кол-во символов в пакете
    int Nb = Nsp*SL;                                                //кол-во байт в пакете без нулей и лишних байт протокола cmd, но с командным словом и размером данных
    int remainNsp = Ns-(Np-1)*maxNumSymbols;                        //кол-во символов в последнем пакете
    //int lastSLwithoutNulls = SL-d;                                  //кол-во байт данных в последнем символе без нулей
    reg_hsc_creg.com_res=0;
    reg_hsc_creg.rx_res=0;
    reg_hsc_creg.tx_res=0;
    reg_hsc_creg.start_bc=1;

    QByteArray first;
    int sizeSize = 0;
    readReg(&reg_hsc_status);
    for (int i=1; i<=Np; i++) {
        if (reg_hsc_status.tx_num_buf == 0)         commandWord.addr = ADDR_MEM2P_BUF_TX_0;
        else if (reg_hsc_status.tx_num_buf == 1)    commandWord.addr = ADDR_MEM2P_BUF_TX_1;
        if (i==Np) {
            Nsp = remainNsp;
            Nb=(Nsp-1)*SL + SL-d;
        }
        commandWord.num_of_symbols=Nsp;
        if (i==1)   sizeSize = sizeof(quint32); else    sizeSize = 0;
        first.clear();
        first = cmdHead(6,Nb+CL).append((char*)&commandWord, 2*CL).append((char*)&size,sizeSize);
        writeAll(first.data(), first.size());
        writeAll(curArrayPointer, Nb-CL-sizeSize);
        getLittleAnswer(6);
        writeReg(&reg_hsc_creg);

        while (1) {
            readReg(&reg_hsc_status);
            if (reg_hsc_status.rt_bc_int != 1) {
                //добавить условие с каким то максимальным временем ожидания или скольими то итерациями, по которому бросать ошибку
                continue;
            } else if (reg_hsc_status.rt_bc_int == 1) {
                if (reg_hsc_status.rs_err == 1 || reg_hsc_status.no_aw_err == 1 || reg_hsc_status.yes_aw_gr_err == 1) {
                    if (reg_hsc_status.rs_err == 1) emit hsc_message("rs_err");
                    if (reg_hsc_status.no_aw_err == 1) emit hsc_message("no_aw_err");
                    if (reg_hsc_status.yes_aw_gr_err == 1) emit hsc_message("yes_aw_gr_err");
                    i--;
                }
                curArrayPointer-=Nb-CL;
                break;
            }
        }



        curArrayPointer+=Nb-CL;
    }
}
*/

/*

void Device::sendDataToRT(quint32 addrRT, char *writeArray, int size)
{
    commandWord.rtaddr = addrRT;
    commandWord.t_r = 0;
    int nextSize = size + sizeof(quint32);                          //плюс в начало к данным ихний размер, для понимания оконечником того, сколько нулей в конце(следующий за cmdHead протокол)
    char* curArrayPointer = writeArray;
    int CL = sizeof(commandWord)/2;                                 //длина командного слова в байтах
    int SL = symbolLength();                                        //длина одного символа в байтах
    int DL = nextSize + CL;                                         //длина всех данных с командным словом в байтах без нулей
    int Ns = DL/SL + (DL%SL != 0 ? 1 : 0);                          //общее кол-во символов
    int Np = Ns/maxNumSymbols + (Ns%maxNumSymbols != 0 ? 1 : 0);    //общее кол-во пакетов
    int d = Ns*SL-DL;                                               //кол-во нулей в байтах
    int Nsp = maxNumSymbols;                                        //кол-во символов в пакете
    int Nb = Nsp*SL;                                                //кол-во байт в пакете без нулей и лишних байт протокола cmd, но с командным словом и размером данных
    int remainNsp = Ns-(Np-1)*maxNumSymbols;                        //кол-во символов в последнем пакете
    //int lastSLwithoutNulls = SL-d;                                  //кол-во байт данных в последнем символе без нулей
    reg_hsc_creg.com_res=0;
    reg_hsc_creg.rx_res=0;
    reg_hsc_creg.tx_res=0;
    reg_hsc_creg.start_bc=1;

    QByteArray first;
    int sizeSize = 0;
    readReg(&reg_hsc_status);
    for (int i=1; i<=Np; i++) {
        if (reg_hsc_status.tx_num_buf == 0)         commandWord.addr = ADDR_MEM2P_BUF_TX_0;
        else if (reg_hsc_status.tx_num_buf == 1)    commandWord.addr = ADDR_MEM2P_BUF_TX_1;
        if (i==Np) {
            Nsp = remainNsp;
            Nb=(Nsp-1)*SL + SL-d;
        }
        commandWord.num_of_symbols=Nsp;
        if (i==1)   sizeSize = sizeof(quint32); else    sizeSize = 0;
        first.clear();
        first = cmdHead(6,Nb+CL).append((char*)&commandWord, 2*CL).append((char*)&size,sizeSize);
        writeAll(first.data(), first.size());
        writeAll(curArrayPointer, Nb-CL-sizeSize);
        getLittleAnswer(6);
        writeReg(&reg_hsc_creg);

        while (1) {
            readReg(&reg_hsc_status);
            if (reg_hsc_status.rt_bc_int != 1) {
                //добавить условие с каким то максимальным временем ожидания или скольими то итерациями, по которому бросать ошибку
                continue;
            } else if (reg_hsc_status.rt_bc_int == 1) {
                if (reg_hsc_status.rs_err == 1 || reg_hsc_status.no_aw_err == 1 || reg_hsc_status.yes_aw_gr_err == 1) {
                    if (reg_hsc_status.rs_err == 1) emit hsc_message("rs_err");
                    if (reg_hsc_status.no_aw_err == 1) emit hsc_message("no_aw_err");
                    if (reg_hsc_status.yes_aw_gr_err == 1) emit hsc_message("yes_aw_gr_err");
                    i--;
                }
                curArrayPointer-=Nb-CL;
                break;
            }
        }



        curArrayPointer+=Nb-CL;
    }
}


void Device::writePath(quint32 addr2pmem, char *writeArray, int size)
{

}

*/

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

void Device::doError(QAbstractSocket::SocketError err)
{
    //message(tr("Ошибка: %1").arg());
}

SocketDriver::~SocketDriver() {
    if (socket->state() != QAbstractSocket::UnconnectedState)
        socket->abort();
    timer->stop();
    delete mutex;
}

void SocketDriver::init()
{
    mutex = new QMutex();
    mutex->lock();
    socket = new QTcpSocket(this);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(3000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTOSlot()));
    connect(socket, SIGNAL(connected()), this, SLOT(connectedSlot()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWrittenSlot(qint64)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSlot(QAbstractSocket::SocketError)));
    //qDebug() << "SocketDriver::init(): the end;";
}

void SocketDriver::stop()
{
    timer->stop();
    mutex->unlock();
    socket->abort();
}

void SocketDriver::connectedSlot()
{
    timer->stop();
    mutex->unlock();
    qDebug() << "connectedSlot() OK";
}

void SocketDriver::disconnectedSlot()
{
//    timer->stop();
//    mutex.unlock();
    qDebug() << "disconnectedSlot() OK";
}

void SocketDriver::readyReadSlot()
{
    qint64 bytesAvailable = socket->bytesAvailable();
    //qDebug() << "In readyReadSlot(); bytesAvailable(): " << bytesAvailable;
    qint64 bytesRead = socket->read(allData.tempArray, bytesAvailable);
    //qDebug() << "In readyReadSlot(); real socket->read() bytes: " << bytesRead;

    allData.allDataToRead-=bytesRead;
    allData.tempArray+=bytesRead;

    if (allData.allDataToRead != 0) {
        if (bytesRead != bytesAvailable)
            qDebug() << "Warning: return from readyReadSlot() with (bytesRead != bytesAvailable)!";
        //qDebug() << "In readyReadSlot(); allData.allDataToRead: " << allData.allDataToRead;
        return;
    }

    timer->stop();
    if (allData.format == write_f1 || allData.format == write_f2 || allData.format == write_echo || allData.format == cpy_on_hard) {
        if (allData.littleAnswer != allData.format) {
            qDebug() << "littleAnswer != allData.format";
            socket->abort();
        }
    }
    mutex->unlock();
}

void SocketDriver::bytesWrittenSlot(qint64 bytes)
{
    //qDebug() << "From bytesWrittenSlot(qint64 bytes); bytes: " << bytes;
    //qDebug() << "From bytesWrittenSlot(qint64 bytes); bytesToWrite(): " << socket->bytesToWrite();
//    allData.allDataToWrite-=bytes;
//    int pos = head_and_Data.size()-allData.allDataToWrite;
//    //qDebug() << "In bytesWrittenSlot(qint64 bytes); bytesToWrite(): " << socket->bytesToWrite();
//    if (allData.allDataToWrite)
//        socket->write(head_and_Data.data() + pos, allData.allDataToWrite);
}

void SocketDriver::errorSlot(QAbstractSocket::SocketError error)
{
    qDebug() << "SocketDriver::errorSlot(): error: " << error;
    qDebug() << QString("SocketDriver::errorSlot(): errorString: %1").arg(socket->errorString());
}

void SocketDriver::timerTOSlot()
{
    qDebug() << "SocketDriver::timerTOSlot()";
    socket->abort();
    mutex->unlock();
}

void SocketDriver::tryToConnect(QString ip, ushort port)
{
    socket->setProxy(QNetworkProxy::NoProxy);
    timer->start();
    socket->connectToHost(QHostAddress(ip), port);
    //qDebug() << "SocketDriver::tryToConnect(): before mutex.lock();";
    mutex->lock();
    //qDebug() << "SocketDriver::tryToConnect(): after mutex.lock();";
}

void SocketDriver::tryToDisconnect()
{
    socket->abort();
//    timer->start();
//    socket->disconnectFromHost();
//    //qDebug() << "SocketDriver::tryToDisconnect(): before mutex.lock();";
//    mutex.lock();
//    //qDebug() << "SocketDriver::tryToDisconnect(): after mutex.lock();";
}

void SocketDriver::tryToExchange()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
        return;
    timer->start();
    head_and_Data.clear();
    allData.littleAnswer = 0;
    switch (allData.format) {
    case write_f1:
        head_and_Data = cmdHead(allData.format, allData.size).append(allData.writeArray, allData.size);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    case write_f2:
        head_and_Data = cmdHead(allData.format, allData.size + 4).append((char*)&allData.startAddr, 4).append(allData.writeArray, allData.size);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    case write_echo:
        head_and_Data = cmdHead(allData.format, allData.size).append(allData.text.toStdString().c_str(), allData.size);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    case read_f1:
        head_and_Data = cmdHead(allData.format, allData.size).append(allData.writeArray, allData.size);
        allData.allDataToRead = allData.size;
        allData.tempArray = allData.readArray;
        break;
    case read_f2:
        head_and_Data = cmdHead(allData.format, allData.size).append((char*)&allData.startAddr, 4).append((char*)&allData.count, 4);
        allData.allDataToRead = allData.count;
        allData.tempArray = allData.readArray;
        break;
    case cpy_on_hard:
        head_and_Data = cmdHead(allData.format, allData.size).append((char*)&allData.fromAddr, 4).append((char*)&allData.count, 4).append((char*)&allData.toAddr, 4);
        allData.allDataToRead = 4;
        allData.tempArray = (char*)&allData.littleAnswer;
        break;
    default:
        qDebug() << "Error on SocketDriver thread: unknown format from tryToExchange()!";
        return;
        //break;
    }
    allData.allDataToWrite = head_and_Data.size();
    allData.curPointer = 0;
    //qDebug() << "Before socket->write() in tryExchange()!";
    socket->write(head_and_Data.data(), allData.allDataToWrite);
    //qDebug() << "After socket->write() in tryExchange()!";
    mutex->lock();
}
