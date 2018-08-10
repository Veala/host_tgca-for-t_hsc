#include "abstracttest.h"
#include "memtest.h"
#include "regtest.h"
#include "echotest.h"
#include "bulbtest.h"
#include "trmsingletest.h"

int GlobalState::globalState = FREELY;

AbstractTest::AbstractTest(QWidget *parent) : QFrame(parent)
{
    QAction *act;
//    act = menu.addAction(tr("Старт"));
//    connect(act, SIGNAL(triggered(bool)), this, SLOT(startTest(bool)));
    act = menu.addAction(tr("Настройки..."));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showSettings()));
    act = menu.addAction(tr("Сохранить")); act->setObjectName(tr("saveObj"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(save()));
//    act = menu.addAction(tr("Создать копию...")); act->setObjectName(tr("saveAsObj"));
//    connect(act, SIGNAL(triggered(bool)), this, SLOT(save(bool)));
    act = menu.addAction(tr("Убрать"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteLater()));
    layout = new QHBoxLayout(this);
    name_enabled = new QCheckBox(tr("Тест"));
    //name_enabled->setLayoutDirection(Qt::RightToLeft);
    layout->addWidget(name_enabled);
    fileName = new QLabel(tr(""));
    fileName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(fileName);
    layout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
    startButton = new QPushButton(QIcon(":/pictogram/gtk-media-play-ltr_8717.png"), tr(""));
    layout->addWidget(startButton);
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(firstStartTest()));
    pauseButton = new QPushButton(QIcon(":/pictogram/gtk-media-pause_2289.png"), tr(""));
    layout->addWidget(pauseButton);
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseTest()));
    stopButton = new QPushButton(QIcon(":/pictogram/gtk-media-stop_9402.png"), tr(""));
    layout->addWidget(stopButton);
    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stopTest()));

    status = new QLabel(tr(""));
    status->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(status);

    setAutoFillBackground(true);
    //setRunningState(AbstractTest::Stopped);
    setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

    status->setPixmap(QPixmap(tr(":/pictogram/gtk-media-stop_9402.png")));
    runningState = Stopped;

    connect(this, SIGNAL(globalStart()), this, SLOT(firstStartTest()));
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

    }
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
    connect(dev, SIGNAL(sigConnectedDevice()), this, SLOT(connectingSockDevice()));
    connect(dev, SIGNAL(sigDisconnectedDevice()), this, SLOT(disconnectingSockDevice()));
    connect(dev, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorDevice(QAbstractSocket::SocketError)));
}

void AbstractTest::setDisconnections(Device *dev)
{
    qDebug() << "setDissconnections!";
    disconnect(dev, SIGNAL(sigDelete(QString)), this, SLOT(deletingDevice(QString)));
    disconnect(dev, SIGNAL(sigConnectedDevice()), this, SLOT(connectingSockDevice()));
    disconnect(dev, SIGNAL(sigDisconnectedDevice()), this, SLOT(disconnectingSockDevice()));
    disconnect(dev, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorDevice(QAbstractSocket::SocketError)));
}

void AbstractTest::setValidState(AbstractTest::ValidState vs)
{
    QPalette palette;
    if (vs == AbstractTest::DeviceIsNotAvailable) {
        QBrush br(Qt::red); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    } else if (vs == AbstractTest::ConnectionIsNotAvailable) {
        QBrush br(Qt::yellow); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    } else if (vs == AbstractTest::ItIsOk) {
        QBrush br(Qt::green); palette.setBrush(QPalette::Window, br); this->setPalette(palette);
    }
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
    setValidState(AbstractTest::ConnectionIsNotAvailable);
    for (int i=0; i<deviceList.count(); i++) {
        if (deviceList[i]->rw_socket.state() != QAbstractSocket::ConnectedState) return;
    }
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
        qDebug() << "senderName" << senderName;
        if (senderName == "Device") {
            devices->removeWidget((Device*)sender());
        }
    }
    for (int i=0; i<deviceList.count(); i++)
        setDisconnections(deviceList[i]);
    deviceList.clear();
}

void AbstractTest::connectingSockDevice()
{
    qDebug() << "connectingSockDevice";
    for (int i=0; i<deviceList.count(); i++) {
        if (deviceList[i]->rw_socket.state() != QAbstractSocket::ConnectedState) return;
    }
    setValidState(AbstractTest::ItIsOk);
}

void AbstractTest::disconnectingSockDevice()
{
    qDebug() << "disconnectingSockDevice";
    setValidState(AbstractTest::ConnectionIsNotAvailable);
}

void AbstractTest::errorDevice(QAbstractSocket::SocketError err)
{
    qDebug() << "errorDevice slot in test";
    setValidState(AbstractTest::ConnectionIsNotAvailable);
}

void AbstractTest::setSettings(QVBoxLayout *b, QDialog *d, bool ched, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB)
{
    devices=b;  settings=d; projectBrowser = pB; testsBrowser = tB;
    settings->setWindowTitle(QObject::tr("Настройки"));
    name_enabled->setChecked(ched);
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
        status->setPixmap(QPixmap(tr(":/pictogram/gtk-media-play-ltr_8717.png")));
        if (getRunningState() == Paused) {
            message(tr("Пауза снята - %1").arg(fileName->text()));
        } else {
            message(tr("Тест запущен - %1").arg(fileName->text()));
            setGlobalState(BUSY);
            emit setEmit(startButton, pauseButton, stopButton);
        }
    } else if (rs == AbstractTest::Paused) {
        status->setPixmap(QPixmap(tr(":/pictogram/gtk-media-pause_2289.png")));
        message(tr("Тест поставлен на паузу - %1").arg(fileName->text()));
    } else if (rs == AbstractTest::Stopped) {
        status->setPixmap(QPixmap(tr(":/pictogram/gtk-media-stop_9402.png")));
        if (sender()->metaObject()->className() != tr("MainWindow")) {
            message(tr("Тест остановлен - %1").arg(fileName->text()));
            setGlobalState(FREELY);
            emit unsetEmit(startButton, pauseButton, stopButton);
        }
    } else if (rs == AbstractTest::Completed) {
        status->setPixmap(QPixmap(tr(":/pictogram/confirm_3843.png")));
        message(tr("Тест закончен - %1").arg(fileName->text()));
        setGlobalState(FREELY);
        emit unsetEmit(startButton, pauseButton, stopButton);
    } else if (rs == AbstractTest::ErrorIsOccured) {
        status->setPixmap(QPixmap(tr(":/pictogram/cancel_8315.png")));
        message(tr("Тест остановлен из-за ошибки - %1").arg(fileName->text()));
        setGlobalState(FREELY);
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

void GlobalState::setGlobalState(int gs)
{
    globalState = gs;
}

int GlobalState::getGlobalState() const
{
    return globalState;
}
