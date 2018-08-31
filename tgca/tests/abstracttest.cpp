#include "abstracttest.h"
#include "memtest.h"
#include "regtest.h"
#include "echotest.h"
#include "bulbtest.h"
#include "trmsingletest.h"

int GlobalState::globalState = FREELY;

AbstractTest::AbstractTest(QWidget *parent) : QFrame(parent)
{
#ifdef debug_AT
    int nint = sizeof(int);
    int nshort = sizeof(short);
    qDebug() << "nint: " << QString::number(nint);
    qDebug() << "nshort: "<< QString::number(nshort);
#endif

    QAction *act;
    act = menu.addAction(tr("Настройки..."));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showSettings()));
    act = menu.addAction(tr("Сохранить")); act->setObjectName(tr("saveObj"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(save()));
//    act = menu.addAction(tr("Создать копию...")); act->setObjectName(tr("saveAsObj"));
//    connect(act, SIGNAL(triggered(bool)), this, SLOT(save(bool)));
    act = menu.addAction(tr("Убрать"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteLater()));
    layout = new QHBoxLayout(this);
    //layout->setMargin(0);
    setFixedHeight(44);
    name_enabled = new QCheckBox(tr("Тест"));
    //name_enabled->setLayoutDirection(Qt::RightToLeft);
    layout->addWidget(name_enabled);
    fileName = new QLabel(tr(""));
    fileName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(fileName);
    layout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

    startButton = new QPushButton(QIcon(":/pictogram/gtk-media-play-ltr_8717.png"), tr(""));
    layout->addWidget(startButton, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    //startButton->setFixedHeight(2*this->height()/3);
    forIcons.setHeight(startButton->height()/25);
    forIcons.setWidth(startButton->height()/25);
    startButton->setIconSize(forIcons);
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(firstStartTest()));
    pauseButton = new QPushButton(QIcon(":/pictogram/gtk-media-pause_2289.png"), tr(""));
    layout->addWidget(pauseButton, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    //pauseButton->setFixedHeight(2*this->height()/3);
    pauseButton->setIconSize(forIcons);
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseTest()));
    stopButton = new QPushButton(QIcon(":/pictogram/gtk-media-stop_9402.png"), tr(""));
    layout->addWidget(stopButton, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    //stopButton->setFixedHeight(2*this->height()/3);
    stopButton->setIconSize(forIcons);
    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stopTest()));

    status = new QLabel(tr(""));
    status->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(status);

    setAutoFillBackground(true);
    //setRunningState(AbstractTest::Stopped);
    setFrameStyle(QFrame::StyledPanel | QFrame::Plain);


    status->setPixmap((QPixmap(tr(":/pictogram/gtk-media-stop_9402.png"))).scaled(forIcons,Qt::KeepAspectRatio));
    runningState = Stopped;

    connect(this, SIGNAL(globalStart()), this, SLOT(firstStartTest()));

    setAcceptDrops(true);
}

AbstractTest::~AbstractTest()
{
    testThread.quit();
    testThread.wait();
    delete settings;
}

void AbstractTest::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        menu.exec(QCursor::pos());
    } else if (event->buttons() == Qt::LeftButton) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(tr("mimeDataForTamiasTests"));
        drag->setMimeData(mimeData);
        emit dragged();
        Qt::DropAction dropAction = drag->exec();
    }
}

void AbstractTest::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void AbstractTest::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain") &&
        (event->mimeData()->text() == tr("mimeDataForTamiasTests")))
        emit dropped();
    event->acceptProposedAction();
}

void AbstractTest::message(QString m)
{
    QDateTime local(QDateTime::currentDateTime());
    projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + name_enabled->text() + ". " + m);
}

void AbstractTest::testOutout(QString m)
{
    testsBrowser->append(m);
}

void AbstractTest::setConnections(Device *dev)
{
    connect(dev, SIGNAL(sigDelete(QString)), this, SLOT(deletingDevice(QString)));
//    connect(dev, SIGNAL(sigConnectedDevice()), this, SLOT(connectingSockDevice()));
//    connect(dev, SIGNAL(sigDisconnectedDevice()), this, SLOT(disconnectingSockDevice()));
//    connect(dev, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorDevice(QAbstractSocket::SocketError)));
}

void AbstractTest::setDisconnections(Device *dev)
{
#ifdef debug_AT
    qDebug() << "setDisconnections!";
#endif
    disconnect(dev, SIGNAL(sigDelete(QString)), this, SLOT(deletingDevice(QString)));
//    disconnect(dev, SIGNAL(sigConnectedDevice()), this, SLOT(connectingSockDevice()));
//    disconnect(dev, SIGNAL(sigDisconnectedDevice()), this, SLOT(disconnectingSockDevice()));
//    disconnect(dev, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorDevice(QAbstractSocket::SocketError)));
}

void AbstractTest::setValidState(AbstractTest::ValidState vs)
{
    QPalette palette;
    if (vs == AbstractTest::DeviceIsNotAvailable) {
        //setStyleSheet("QFrame { border: 3px solid red; background-color: lightGray ;}");  //qRgb(200,0,200)
        QBrush br(qRgb(255,75,75)); palette.setBrush(QPalette::Window, br); this->setPalette(palette);  // red
    } else if (vs == AbstractTest::ConnectionIsNotAvailable) {
        //setStyleSheet("QFrame { border: 3px solid yellow; background-color: lightGray;}");
        QBrush br(qRgb(255,255,100)); palette.setBrush(QPalette::Window, br); this->setPalette(palette);  // yellow
    } else if (vs == AbstractTest::ItIsOk) {
        //setStyleSheet("QFrame { border: 3px solid green; background-color: lightGray;}");
        QBrush br(qRgb(100,230,100)); palette.setBrush(QPalette::Window, br); this->setPalette(palette);  // green  // 170,255,130
    }
    //status->setStyleSheet("QLabel { border: 3px solid lightGray; background-color: lightGray;}");
    //fileName->setStyleSheet("QLabel { border: 3px solid lightGray; background-color: lightGray;}");
    validState = vs;
}

AbstractTest::ValidState AbstractTest::getValidState() const
{
    return validState;
}

AbstractTest::RunningState AbstractTest::getRunningState() const
{
    return runningState;
}

void AbstractTest::checkDeviceAvailability(int x)
{
    int counter=0;
    if (deviceList.count() == deviceLineEditList.count()) {
        for (int i=0; i<deviceLineEditList.count(); i++) {
            if (deviceLineEditList[i]->text() == deviceList[i]->getName()) {
                counter++;
            } else {
                break;
            }
        }
        if (counter == deviceLineEditList.count()) {
            return;
        } else {
            deletingDevice_part();
        }
    }
    deviceList.clear();
    Device* dev;
    for (int i=0; i<deviceLineEditList.count(); i++) {
        int j=0;
        for (; j<devices->count(); j++) {
            dev = (Device*)devices->itemAt(j)->widget();
            //qDebug() << deviceLineEditList[i]->text();
            //qDebug() << dev->getName();
            if (deviceLineEditList[i]->text() == dev->getName()) {
                deviceList.append(dev);
                break;
            }
        }
        if (j == devices->count()) {
            message(tr("Ошибка: устройство %1 не доступно - %2").arg(deviceLineEditList[i]->text()).arg(fileName->text()));
            setValidState(AbstractTest::DeviceIsNotAvailable);
            return;
        }
    }
    for (int i=0; i<deviceList.count(); i++)
        setConnections(deviceList[i]);
//    setValidState(AbstractTest::ConnectionIsNotAvailable);
//    for (int i=0; i<deviceList.count(); i++) {
//        if (deviceList[i]->rw_socket.state() != QAbstractSocket::ConnectedState) return;
//    }
    setValidState(AbstractTest::ItIsOk);
}

void AbstractTest::newDev(QString dn)
{
    for (int i=0; i<deviceLineEditList.count(); i++) {
        if (deviceLineEditList[i]->text() == dn) {
            checkDeviceAvailability(1111);
            return;
        }
    }
}

void AbstractTest::deletingDevice(QString dn)
{
    deletingDevice_part();
    checkDeviceAvailability(1111);
}

void AbstractTest::deletingDevice_part()
{
    if (sender() != NULL) {
        QString senderName(sender()->metaObject()->className());
#ifdef debug_AT
        qDebug() << "senderName" << senderName;
#endif
        if (senderName == "Device") {
            devices->removeWidget((Device*)sender());
        }
    }
    for (int i=0; i<deviceList.count(); i++)
        setDisconnections(deviceList[i]);
    deviceList.clear();
}

//void AbstractTest::connectingSockDevice()
//{
//    qDebug() << "connectingSockDevice";
//    for (int i=0; i<deviceList.count(); i++) {
//        if (deviceList[i]->rw_socket.state() != QAbstractSocket::ConnectedState) return;
//    }
//    setValidState(AbstractTest::ItIsOk);
//}

//void AbstractTest::disconnectingSockDevice()
//{
//    qDebug() << "disconnectingSockDevice";
//    setValidState(AbstractTest::ConnectionIsNotAvailable);
//}

//void AbstractTest::errorDevice(QAbstractSocket::SocketError err)
//{
//    qDebug() << "errorDevice slot in test";
//    setValidState(AbstractTest::ConnectionIsNotAvailable);
//}

void AbstractTest::setSettings(QVBoxLayout *b, QDialog *d, bool ched, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB)
{
    devices=b;  settings=d; projectBrowser = pB; testsBrowser = tB;
    settings->setWindowTitle(QObject::tr("Настройки"));
    name_enabled->setChecked(ched);
    name_enabled->setMinimumWidth(150);
    name_enabled->setText(tType);
    fileName->setText(fName);

    connect(settings,SIGNAL(finished(int)),this,SLOT(checkDeviceAvailability(int)));
}

QString AbstractTest::getName() const
{
    return fileName->text();
}

bool AbstractTest::isReady() const
{
    return name_enabled->isChecked();
}

void AbstractTest::showSettings()
{
    settings->exec();
}

void AbstractTest::save()
{
    QString saveSndrName = sender()->objectName();
    if (saveSndrName == "saveObj") {
        saveFileNameStr = fileName->text();
    } else if (saveSndrName == "saveAsObj") {
        saveFileNameStr = QFileDialog::getSaveFileName(this, QObject::tr("Введите имя файла"), QObject::tr(""));
        if (saveFileNameStr.isEmpty()) {
            return;
        }
    }
}

void AbstractTest::setRunningState(int rs)
{
    if (rs == AbstractTest::Running) {
        status->setPixmap((QPixmap(tr(":/pictogram/gtk-media-play-ltr_8717.png"))).scaled(forIcons, Qt::KeepAspectRatio));
        if (getRunningState() == Paused) {
            message(tr("Пауза снята - %1").arg(fileName->text()));
        } else {
            message(tr("Тест запущен - %1").arg(fileName->text()));
            setGlobalState(BUSY);
            emit setEmit(startButton, pauseButton, stopButton);
        }
    } else if (rs == AbstractTest::Paused) {
        status->setPixmap((QPixmap(tr(":/pictogram/gtk-media-pause_2289.png"))).scaled(forIcons, Qt::KeepAspectRatio));
        message(tr("Тест поставлен на паузу - %1").arg(fileName->text()));
    } else if (rs == AbstractTest::Stopped) {
        status->setPixmap((QPixmap(tr(":/pictogram/gtk-media-stop_9402.png"))).scaled(forIcons ,Qt::KeepAspectRatio));
        if (sender()->metaObject()->className() != tr("MainWindow")) {
            message(tr("Тест остановлен - %1").arg(fileName->text()));
            setGlobalState(FREELY);
            emit unsetEmit(startButton, pauseButton, stopButton);
        }
    } else if (rs == AbstractTest::Completed) {
        status->setPixmap((QPixmap(tr(":/pictogram/confirm_3843.png"))).scaled(forIcons ,Qt::KeepAspectRatio));
        message(tr("Тест закончен - %1").arg(fileName->text()));
        setGlobalState(FREELY);
        emit unsetEmit(startButton, pauseButton, stopButton);
    } else if (rs == AbstractTest::ErrorIsOccured) {
        status->setPixmap((QPixmap(tr(":/pictogram/cancel_8315.png"))).scaled(forIcons ,Qt::KeepAspectRatio));
        message(tr("Тест остановлен из-за ошибки - %1").arg(fileName->text()));
        setGlobalState(FREELY);
//        Device* dev;
//        foreach (dev, deviceList)
//            dev->rw_socket.abort();
        emit unsetEmit(startButton, pauseButton, stopButton);
    }
    runningState = (AbstractTest::RunningState)rs;
}

void AbstractTest::firstStartTest()
{
    if (getValidState() != ItIsOk) {
        message(tr("Ошибка: проблема с устройствами теста - %1").arg(fileName->text()));
        emit unsetEmit(startButton, pauseButton, stopButton);
        return;
    }
    if (getRunningState() == Running) {
        message(tr("Предупреждение: в данный момент запущен тест - %1").arg(fileName->text()));
        return;
    }
    if (getRunningState() == Paused) {
        objToThread->threadState = AbstractTest::Running;
        return;
    }
    if (getGlobalState() == FREELY) {
        objToThread->threadState = AbstractTest::Running; //???
        startTest();
        return;
    } else {
        message(tr("Предупреждение: в данный момент запущен некоторый тест"));
        return;
    }
}

void AbstractTest::pauseTest()
{
    if (getValidState() != ItIsOk) {
        message(tr("Ошибка: проблема с устройствами теста - %1").arg(fileName->text()));
        return;
    }
    if (getRunningState() != Running) {
        message(tr("Предупреждение: тест не запущен - %1").arg(fileName->text()));
        return;
    }
    objToThread->threadState = AbstractTest::Paused;
}

void AbstractTest::stopTest()
{
    if (getValidState() != ItIsOk) {
        message(tr("Ошибка: проблема с устройствами теста - %1").arg(fileName->text()));
        return;
    }
    if ((getRunningState() == Stopped) || (getRunningState() == Completed) || (getRunningState() == ErrorIsOccured)) {
        message(tr("Предупреждение: тест не запущен - %1").arg(fileName->text()));
        return;
    }
    objToThread->threadState = AbstractTest::Stopped;
}

AbstractTest *testLib::createTest(QVBoxLayout *devices, QTextBrowser *pB, QTextBrowser *tB)
{
    QStringList allTests;
    allTests << QObject::tr("Тест \"Эхо\"") << QObject::tr("Тест памяти") << QObject::tr("Тест регистров")
             << QObject::tr("Тест \"лампочек\"") << QObject::tr("Передача одного пакета");

    bool ok;
    QString testType = QInputDialog::getItem(0, QObject::tr("Создать тест"), QObject::tr("Тесты:"), allTests, 0, false, &ok);
    if (!(ok && !testType.isEmpty()))
        return NULL;
    QString newFileStr = QFileDialog::getSaveFileName(0, QObject::tr("Введите имя файла"), QObject::tr(""), QString(), 0, QFileDialog::DontConfirmOverwrite);
    if (newFileStr.isEmpty()) {
        return NULL;
    }
    QString defFileStr;

    if (testType == QObject::tr("Тест памяти")) {
        defFileStr = QObject::tr("../default/mem_test");
    } else if (testType == QObject::tr("Тест регистров")) {
        defFileStr = QObject::tr("../default/reg_test");
    } else if (testType == QObject::tr("Тест \"Эхо\"")) {
        defFileStr = QObject::tr("../default/echo_test");
    } else if (testType == QObject::tr("Тест \"лампочек\"")) {
        defFileStr = QObject::tr("../default/bulb_test");
    } else if (testType == QObject::tr("Передача одного пакета")) {
        defFileStr = QObject::tr("../default/trm_single_test");
    }

    if(!QFile::copy(defFileStr,newFileStr)) {
        QMessageBox::critical(0, QObject::tr("Создать тест"), QObject::tr("Файл уже существует"));
        return NULL;
    }

    return loadTest(newFileStr, devices, pB, tB);
}

AbstractTest *testLib::loadTest(QString settingsFileStr, QVBoxLayout *devices, QTextBrowser *pB, QTextBrowser *tB)
{
    QFile settingsFile(settingsFileStr);
    if (!settingsFile.open(QFile::ReadOnly))
        return NULL;
    QTextStream out(&settingsFile);
    QString testType = out.readLine();
    bool checked = out.readLine().toInt();
    settingsFile.close();
    QString uiFileStr;
    AbstractTest* test;
    if (testType == QObject::tr("Тест памяти")) {
        uiFileStr = QObject::tr("../default/settings_mem_test.ui");
        test = new MemTest(0);
    } else if (testType == QObject::tr("Тест регистров")) {
        uiFileStr = QObject::tr("../default/settings_reg_test.ui");
        test =  new RegTest(0);
    } else if (testType == QObject::tr("Тест \"Эхо\"")) {
        uiFileStr = QObject::tr("../default/settings_echo_test.ui");
        test = new EchoTest(0);
    } else if (testType == QObject::tr("Тест \"лампочек\"")) {
        uiFileStr = QObject::tr("../default/settings_bulb_test.ui");
        test = new BulbTest(0);
    } else if (testType == QObject::tr("Передача одного пакета")) {
        uiFileStr = QObject::tr("../default/settings_trm_single_test.ui");
        test = new TrmSingleTest(0);
    } else {
        qDebug() << "unknown test";
        return NULL;
    }
    QUiLoader loader;
    QFile uiFile(uiFileStr);
    if (!uiFile.open(QFile::ReadOnly))
        return NULL;
    QDialog* settingsForm = (QDialog*)loader.load(&uiFile);
    uiFile.close();

    test->setSettings(devices, settingsForm, checked, testType, settingsFileStr, pB, tB);
    return test;
}

void absObjToThread::setState(int rs)
{
    threadState = (AbstractTest::RunningState)rs;
}

int absObjToThread::pause_stop()
{
    if (threadState == AbstractTest::Paused) {
        emit resultReady((int)AbstractTest::Paused);
        while (threadState == AbstractTest::Paused) { qDebug() << "while paused"; }
        if (threadState == AbstractTest::Stopped) {
            emit resultReady((int)AbstractTest::Stopped);
            return -1;
        }
        if (threadState == AbstractTest::Running) {
            emit resultReady((int)AbstractTest::Running);
            return 0;
        }
    } else if (threadState == AbstractTest::Stopped) {
        emit resultReady((int)AbstractTest::Stopped);
        return -1;
    } else {
        return 0;
    }
}

int absObjToThread::readAll(QTcpSocket *socket, QByteArray& array, int size)
{
    array.clear();
    array.resize(size);

    int n = 0; int r = 0;
    while (1) {
        r = socket->read(array.data()+n, size-n);
        if (r!=0) {
#ifdef debug_AT
            qDebug() << "read: " << r;
            qDebug() << "n: " << n;
#endif
        }
        if (r == -1) {
            socket->abort();
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return -1;
        } else if (r==0) {
#ifdef debug_AT
            qDebug() << "-------------if (r==0)";
#endif
            int msec = 3000;
            //if (n != 0) msec = 1;
            if (!socket->waitForReadyRead(msec)) {
#ifdef debug_AT
                qDebug() << "socket->waitForReadyRead(5000) error!";
#endif
                socket->abort();
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                return -1;
            }
        } else {
            n+=r;
            if (n>=size) return 0;
        }
    }
}

int absObjToThread::writeAll(QTcpSocket *socket, QByteArray& array)
{
    char* temp = array.data();
    int size = array.size();
    while (1) {
        int n = socket->write(temp, size);
#ifdef debug_AT
        qDebug() << "wrote: " << n;
#endif
        if (n == -1) {
            socket->abort();
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return -1;
        } else  if (n < size) {
            size -=  n;
            temp = array.right(size).data();
            continue;
        } else if (n == size) {
            break;
        }
    }

    if (!socket->waitForBytesWritten(5000)) {
#ifdef debug_AT
        qDebug() << "socket->waitForBytesWritten(5000) error!";
#endif
        socket->abort();
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        return -1;
    } else {
        return 0;
    }
}

int absObjToThread::write_F1(QTcpSocket *tcpSocket, QByteArray &writeArray)
{
    int cmd = 1;    QByteArray answer;
    if (writeAll(tcpSocket, writeArray) == -1) return -1;
    if (readAll(tcpSocket, answer, 4) == -1) return -1;
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        tcpSocket->abort();
        return -1;
    }
    return 0;
}

int absObjToThread::write_F2(QTcpSocket *tcpSocket, QByteArray &writeArray)
{
    int cmd = 2;    QByteArray answer;
    if (writeAll(tcpSocket, writeArray) == -1) return -1;
    if (readAll(tcpSocket, answer, 4) == -1) return -1;
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        tcpSocket->abort();
        return -1;
    }
    return 0;
}

int absObjToThread::write_Echo(QTcpSocket *tcpSocket, QByteArray &writeArray)
{
    int cmd = 5;    QByteArray answer;
    if (writeAll(tcpSocket, writeArray) == -1) return -1;
    if (readAll(tcpSocket, answer, 4) == -1) return -1;
    if (*(int*)answer.data() != cmd) {
#ifdef debug_AT
        qDebug() << "(int*)answer.data() != cmd";
#endif
        tcpSocket->abort();
        return -1;
    }
    return 0;
}

int absObjToThread::read_F1(QTcpSocket *tcpSocket, QByteArray &writeArray, QByteArray &readArray)
{
    if (writeAll(tcpSocket, writeArray) == -1) return -1;
    if (readAll(tcpSocket, readArray, writeArray.size()-8) == -1) return -1;
    return 0;
}

int absObjToThread::read_F2(QTcpSocket *tcpSocket, QByteArray &writeArray, QByteArray &readArray)
{
    if (writeAll(tcpSocket, writeArray) == -1) return -1;
    int nr = *(int*)(writeArray.data()+12);
    if (readAll(tcpSocket, readArray, nr) == -1) return -1;
    return 0;
}

QByteArray absObjToThread::cmdHead(int cmd, int dsz)
{
    QByteArray head;
    head.append((char*)&cmd, 4);
    head.append((char*)&dsz, 4);
    return head;
}

void GlobalState::setGlobalState(int gs)
{
    globalState = gs;
}

int GlobalState::getGlobalState() const
{
    return globalState;
}
