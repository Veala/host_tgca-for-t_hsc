#include "abstracttest.h"
#include "memtest.h"
#include "regtest.h"
#include "echotest.h"
#include "bulbtest.h"
#include "trmsingletest.h"
#include "varcommtest.h"
#include "monitortest.h"
#include "spipart.h"
#include "nulltest.h"
#include "ramtest.h"
#include "trashtest.h"
#include "noisetest.h"
#include "varbroadtest.h"
#include "alientest.h"
#include "invalidcommtest.h"
#include "lsctest.h"
//#include "lscmessagetest.h"

#include "../picts.h"

AbstractTest* AbstractTest::yellowTest = NULL;
AbstractTest* AbstractTest::beginTest = NULL;
AbstractTest* AbstractTest::endTest = NULL;

static void setStatusText(QLabel *wid, QString str, bool bold, QString color) //Qt::GlobalColor color
{
    QFont f = wid->font();
    f.setBold(bold);
    f.setPixelSize(12); //(bold ? 12 : 7);
    wid->setFont(f);
    wid->setStyleSheet(color);
    wid->setText(str);
}

AbstractTest::AbstractTest(QWidget *parent):
    QFrame(parent),
    su(false),
    runningState(Stopped),
    spinner(0)
{
#ifdef debug_AT
    int nint = sizeof(int);
    int nshort = sizeof(short);
    qDebug() << "nint: " << QString::number(nint);
    qDebug() << "nshort: "<< QString::number(nshort);
#endif

    QAction *act;
    act = menu.addAction(tr("Настройки...")); act->setObjectName("settings");
    act = menu.addAction(tr("Статистика")); act->setObjectName("stats");
    act = menu.addAction(tr("Сохранить")); act->setObjectName(tr("saveObj"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(save()));
    act = menu.addAction(tr("Сохранить как...")); act->setObjectName(tr("saveAsObj"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(save()));
    act = menu.addAction(tr("Убрать")); act->setObjectName(tr("delete"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteObject()));
    layout = new QHBoxLayout(this);

    QHBoxLayout *fileLayout = new QHBoxLayout();

    //layout->setMargin(0);
    setFixedHeight(44);
    name_enabled = new QCheckBox(tr("Тест"));
    //name_enabled->setLayoutDirection(Qt::RightToLeft);
    layout->addWidget(name_enabled);
    fileLayout->addWidget(new QLabel("Файл: "));
    fileName = new QLabel(tr(""));
    fileName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    fileLayout->addWidget(fileName);
    //fileLayout->addSpacing(100);
    fileLayout->addStretch();
    //fileLayout->setContentsMargins(0,0,0,0);
//    layout->addWidget(fileName);


    QHBoxLayout *markingLayout = new QHBoxLayout();
    //QSplitter *markingLayout = new QSplitter();
    markingLayout->addWidget(new QLabel("Имя: "));
    mark = new QLineEdit("");
    //mark->setFixedWidth(200);
    mark->setMinimumWidth(450);
    markingLayout->addWidget(mark);
    //markingLayout->addWidget(new QSpacerItem(0,0));
    markingLayout->addStretch();
    //markingLayout->setStretchFactor(1, 50);
    //markingLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(fileLayout);
    //vbox->addWidget(markingLayout);
    vbox->addLayout(markingLayout);
    //vbox->setContentsMargins(0,0,0,0);

    layout->addLayout(vbox);
    layout->setContentsMargins(layout->contentsMargins().left(),0,layout->contentsMargins().right(),0);

    layout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

    begin_end_Icon = new QLabel(tr(""));
    begin_end_Icon->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    layout->addWidget(begin_end_Icon);

    startButton = new QPushButton(QIcon(strPictTestButtonStart.c_str()), tr(""));
    layout->addWidget(startButton, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    //startButton->setFixedHeight(2*this->height()/3);
    forIconSize = startButton->height()/25;
    forIcons.setHeight(forIconSize);
    forIcons.setWidth(forIconSize);
    forIconSize = forIconSize * 3 / 2;
    startButton->setIconSize(forIcons);
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(firstStartTest()));
    pauseButton = new QPushButton(QIcon(strPictTestButtonPause.c_str()), tr(""));
    layout->addWidget(pauseButton, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    //pauseButton->setFixedHeight(2*this->height()/3);
    pauseButton->setIconSize(forIcons);
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseTest()));
    stopButton = new QPushButton(QIcon(strPictTestButtonStop.c_str()), tr(""));
    layout->addWidget(stopButton, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    //stopButton->setFixedHeight(2*this->height()/3);
    stopButton->setIconSize(forIcons);
    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stopTest()));

    begin_end_Icon->setMinimumWidth(forIconSize * 2);
    statusIcon = new QLabel(tr(""));
    statusIcon->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusIcon->setMinimumWidth(forIconSize * 2);

    statusIcon->setPixmap((QPixmap(tr(strPictTestStatusNotStarted.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));

    layout->addWidget(statusIcon);

    statusTxt = new QLabel(tr(""));
    //statusTxt->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusTxt->setMinimumWidth(90);
    layout->addWidget(statusTxt);
    setStatusText(statusTxt, tr("Загружен"), false, "color: rgb(127, 127, 127)"); // grey

    setAutoFillBackground(true);
    setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

    connect(this, SIGNAL(globalStart()), this, SLOT(firstStartTest()));

    setAcceptDrops(true);
}

AbstractTest::~AbstractTest()
{
    qDebug() << "~AbstractTest() start";

    stopTest();
    testThread.quit();
    //testThread.terminate();
    testThread.wait();

    deleteSpinner(spinner);

    if (yellowTest == this) yellowTest = NULL;
    if (beginTest == this) beginTest = NULL;
    if (endTest == this) endTest = NULL;
    delete settings;
    delete stats;
    message(tr("Тест удален (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
    qDebug() << "~AbstractTest() end";
}

void AbstractTest::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
//        if (yellowTest != NULL && yellowTest != this)
//            yellowTest->setValidState(yellowTest->validState);
        QAction* tempAct = menu.exec(QCursor::pos());
        if (tempAct == 0) return;
        else if (tempAct->objectName() == "delete") {
            if (yellowTest == this) yellowTest = NULL;
            if (beginTest == this) beginTest = NULL;
            if (endTest == this) endTest = NULL;
            return;
        }
        QPalette palette;
        if (getValidState() == ItIsOk) {
            if (yellowTest != NULL)
                yellowTest->setValidState(yellowTest->validState);
            yellowTest = this;
            QBrush br(qRgb(90,210,120)); palette.setBrush(QPalette::Window, br); this->setPalette(palette);  // yellow   99,229,138
            setFrameShadow(QFrame::Sunken);
        }
    } else if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(tr("mimeDataForTamiasTests"));
        drag->setMimeData(mimeData);
        emit dragged();
        Qt::DropAction dropAction = drag->exec();
    } else if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ShiftModifier) {
        setBeginTest();
    } else if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
        setEndTest();
    } else if (event->buttons() == Qt::LeftButton && event->modifiers() == (Qt::ShiftModifier + Qt::ControlModifier)) {
        resetBeginEnd();
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

bool AbstractTest::event(QEvent *e)
{
    if (e->type() == QEvent::Hide)  stats->hide();
    return QFrame::event(e);
}

void AbstractTest::message(QString m, QString browser)
{
    QDateTime local(QDateTime::currentDateTime());
    if (browser == "project")
        projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + name_enabled->text() + ". " + m);
    else if(browser == "tests")
        testsBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + m);
}

void AbstractTest::testOutout(QString m)
{
    testsBrowser->append(m);
}

void AbstractTest::statsTestOutput(QString str, long n)
{
    QMap<QString, QLabel*>::iterator it = statsMap.find(str);
    if (it != statsMap.end())
    {
        QLabel* l = it.value();
        l->setText(QString::number(l->text().toULongLong()+n));
    }
}

void AbstractTest::statsToZero()
{
    foreach (QLabel* l, statsMap) {
        l->setText("0");
    }
}

void AbstractTest::statsSave()
{
    QString saveStatsFileNameStr = QFileDialog::getSaveFileName(this, QObject::tr("Введите имя файла"), QObject::tr(""));
    if (!saveStatsFileNameStr.isEmpty()) {
        QFile f(saveStatsFileNameStr);
        f.open(QIODevice::WriteOnly);
        foreach (QLabel* l, statsMap) {
            QByteArray str;
            str.append(l->objectName());
            str.append(" : ");
            str.append(l->text());
            str.append('\n');
            f.write(str);
        }
        f.close();
    }
}

void AbstractTest::setConnections(Device *dev)
{
    connect(dev, SIGNAL(sigDelete(QString)), this, SLOT(deletingDevice(QString)), Qt::DirectConnection);
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
        //setStyleSheet("QFrame { border: 3px solid red; background-color: lightGray ;}");  // qRgb(200,0,200)
        QBrush br(qRgb(255,75,75)); palette.setBrush(QPalette::Window, br);                 // red
    } else if (vs == AbstractTest::ConnectionIsNotAvailable) {
        //setStyleSheet("QFrame { border: 3px solid yellow; background-color: lightGray;}");
        QBrush br(qRgb(255,255,100)); palette.setBrush(QPalette::Window, br);               // yellow
    } else if (vs == AbstractTest::ItIsOk) {
        //setStyleSheet("QFrame { border: 3px solid green; background-color: lightGray;}");
        QBrush br(qRgb(110, 255, 160)); palette.setBrush(QPalette::Window, br);             // green  // 170,255,130  100,230,100
    }
    else
    {
        validState = vs;
        return;
    }
    //status->setStyleSheet("QLabel { border: 3px solid lightGray; background-color: lightGray;}");
    //fileName->setStyleSheet("QLabel { border: 3px solid lightGray; background-color: lightGray;}");
    setPalette(palette);
    setFrameShadow(QFrame::Raised);
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
            setValidState(AbstractTest::ItIsOk);                            //Для галочек, в дальнейшем убрать!!!
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
            message(tr("Ошибка: устройство %1 не доступно (файл: %2, имя: %3)").arg(deviceLineEditList[i]->text()).arg(fileName->text()).arg(mark->text()));
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
        if (senderName == "BaseDevice") {
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

void AbstractTest::setSettings(QVBoxLayout *b, QDialog *d, bool ched, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    devices=b;  settings=d; projectBrowser = pB; testsBrowser = tB; stats = d2;

    name_enabled->setChecked(ched);
    name_enabled->setMinimumWidth(15);  // 150
    name_enabled->setMaximumWidth(15);
    name_enabled->setText(tType);
    QFileInfo info(fName);
    if (info.path() != "../default")
        fileName->setText(fName);
    mark->setText(markStr);

    if (!markStr.isEmpty())
    {
        settings->setWindowTitle("Настройки(" + mark->text() + ")");
        stats->setWindowTitle("Статистика(" + mark->text() + ")");
    }

    connect((QAction*)menu.findChild<QAction*>("settings"), SIGNAL(triggered(bool)), settings, SLOT(exec()));
    connect((QAction*)menu.findChild<QAction*>("stats"), SIGNAL(triggered(bool)), stats, SLOT(show()));
    QShortcut* sc = new QShortcut(QKeySequence("Esc"), stats);
    connect(sc,SIGNAL(activated()),stats, SLOT(hide()));

//    if (this->parentWidget() == NULL) qDebug() << "objname parent NULL";
//    else qDebug() << this->parentWidget()->objectName() << "objname parent";
    stats->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    settings->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    connect(this,SIGNAL(destroyed(QObject*)),stats,SLOT(hide()));
    connect(mark,SIGNAL(textChanged(QString)),this,SLOT(markChanged(QString)));
    connect(settings,SIGNAL(finished(int)),this,SLOT(checkDeviceAvailability(int)));
    message(tr("Тест добавлен (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
}

QString AbstractTest::getName() const
{
    return fileName->text();
}

bool AbstractTest::isReady() const
{
    return name_enabled->isChecked();
}

void AbstractTest::setUserLevel(bool b)
{
   su = b;
   if (!b)
       setEnabledSpecial(false);
}

void AbstractTest::setBeginTest()
{
    resetBeginEnd();
    if (beginTest != NULL)
        beginTest->begin_end_Icon->setPixmap(QPixmap());
    beginTest = this;
    beginTest->begin_end_Icon->setPixmap((QPixmap(strPictTestMarkerStart.c_str())).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
}

AbstractTest *AbstractTest::getBeginTest()
{
    return beginTest;
}

void AbstractTest::setEndTest()
{
    resetBeginEnd();
    if (endTest != NULL)
        endTest->begin_end_Icon->setPixmap(QPixmap());
    endTest = this;
    endTest->begin_end_Icon->setPixmap((QPixmap(strPictTestMarkerEnd.c_str())).scaled(forIconSize*2, forIconSize*2, Qt::KeepAspectRatio));
}

AbstractTest *AbstractTest::getEndTest()
{
    return endTest;
}

void AbstractTest::resetBeginEnd()
{
    if (this == beginTest) {
        beginTest = NULL;
        begin_end_Icon->setPixmap(QPixmap());
    } else if (this == endTest) {
        endTest = NULL;
        begin_end_Icon->setPixmap(QPixmap());
    }
}

void AbstractTest::actDevMode()
{
    if (!su)
    {
        su = true;
        setEnabledSpecial(true);
    }
}

void AbstractTest::setEnabledSpecial(bool b)
{
    QList<QWidget*> list = settings->findChildren<QWidget*>(QRegExp());
    int sz = list.size();
    for (int i=0; i<sz; i++)
    {
        QWidget* wid = list.at(i);
        QString type_name = wid->metaObject()->className();
        if (type_name != "QLabel" && type_name != "QCheckBox" && type_name != "QLineEdit" && type_name != "QComboBox" && type_name != "QSpinBox" && type_name != "QRadioButton"
                && type_name != "QTableWidget")
            continue;
        wid->setEnabled(b);
    }
}

void AbstractTest::disableStat()
{
    ((QAction*)menu.findChild<QAction*>("stats"))->setEnabled(false);
}

void AbstractTest::save()
{
    QString saveSndrName = sender()->objectName();
    if ((fileName->text().isEmpty() == true) || (saveSndrName == "saveAsObj")) {
        saveFileNameStr = QFileDialog::getSaveFileName(this, QObject::tr("Введите имя файла"), QObject::tr(""));
        if (saveFileNameStr.isEmpty()) {
            return;
        } else {
            fileName->setText(saveFileNameStr);
        }
    } else if (saveSndrName == "saveObj") {
        saveFileNameStr = fileName->text();
    }/* else if (saveSndrName == "saveAsObj") {
        saveFileNameStr = QFileDialog::getSaveFileName(this, QObject::tr("Введите имя файла"), QObject::tr(""));
        if (saveFileNameStr.isEmpty()) {
            return;
        }
    }*/
}

void AbstractTest::markChanged(QString newStr)
{
    stats->setWindowTitle("Статистика(" + newStr + ")");
    settings->setWindowTitle("Настройки(" + newStr + ")");
}

void AbstractTest::setRunningState(int rs)
{
    deleteSpinner(spinner);
    spinner = 0;

    switch (rs)
    {
    case AbstractTest::Running:
        spinner = insertSpinner(statusIcon);
        //statusIcon->setPixmap((QPixmap(tr(strPictTestStatusRunning.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
        statusIcon->setPixmap(QPixmap(""));
        setStatusText(statusTxt, tr("Выполняется"), false, "color: rgb(0, 0, 0)"); // Qt::black)
        if (getRunningState() == Paused) {
            message(tr("Пауза снята с теста \"%1\"").arg(mark->text()), "tests");
        } else {
            message(QObject::tr("Тест \"%1\" запущен").arg(mark->text()), "tests");
            setGlobalState(BUSY);
            emit setEmit(startButton, pauseButton, stopButton);
        }
        break;
    case  AbstractTest::Paused:
        statusIcon->setPixmap((QPixmap(tr(strPictTestStatusInPause.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
        setStatusText(statusTxt, tr("П а у з а"), false, "color: rgb(0, 0, 0)"); // Qt::black)
        message(QObject::tr("Тест \"%1\" поставлен на паузу").arg(mark->text()), "tests");
        break;
    case AbstractTest::Stopped:
        if (sender()->metaObject()->className() != tr("MainWindow")) {
            statusIcon->setPixmap((QPixmap(tr(strPictTestStatusInterrupted.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
            setStatusText(statusTxt, tr("Остановлен"), false, "color: rgb(0, 0, 0)"); // Qt::black)
            message(QObject::tr("Тест \"%1\" остановлен пользователем").arg(mark->text()), "tests");
            setGlobalState(FREE);
            emit unsetEmit(startButton, pauseButton, stopButton);
            if (runningState == AbstractTest::Deleting)
                deleteLater();
        }
        else
        {
           // statusIcon->setPixmap((QPixmap(tr(strPictTestStatusInterrupted.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
           // setStatusText(statusTxt, tr("Пропущен"), false, "color: rgb(0, 0, 0)"); // Qt::black)
        }
        break;
    case AbstractTest::Completed:
        statusIcon->setPixmap((QPixmap(tr(strPictTestStatusFinishedOk.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
        setStatusText(statusTxt, tr("Успех"), true, "color: rgb(0, 0, 0)"); // Qt::blue color: rgb(0, 0, 255)
        message(QObject::tr("Тест \"%1\" закончен").arg(mark->text()), "tests");
        setGlobalState(FREE);
        emit unsetEmit(startButton, pauseButton, stopButton);
        break;
    case AbstractTest::ErrorIsOccured:
        statusIcon->setPixmap((QPixmap(tr(strPictTestStatusFinishedErr.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
        setStatusText(statusTxt, tr("Ошибка"), true, "color: rgb(255, 0, 0)"); // Qt::red)
        message(QObject::tr("Тест \"%1\" остановлен из-за ошибки").arg(mark->text()), "tests");
        setGlobalState(FREE);
//        Device* dev;
//        foreach (dev, deviceList)
//            dev->rw_socket.abort();
        emit unsetEmit(startButton, pauseButton, stopButton);
        break;
//    case AbstractTest::Deleting:
//        break;
    default:
        statusIcon->setPixmap((QPixmap(tr(strPictTestStatusFinishedErr.c_str()))).scaled(forIconSize, forIconSize, Qt::KeepAspectRatio));
        setStatusText(statusTxt, tr("Ошибка"), true, "color: rgb(255, 0, 0)"); // Qt::red)
        message(tr("Внутренняя ошибка: неизвестное состояние теста"), "tests");
        message(QObject::tr("Тест \"%1\" остановлен из-за ошибки").arg(mark->text()), "tests");
        setGlobalState(FREE);
//        Device* dev;
//        foreach (dev, deviceList)
//            dev->rw_socket.abort();
        emit unsetEmit(startButton, pauseButton, stopButton);
        break;
    }
    runningState = (AbstractTest::RunningState)rs;
}

void AbstractTest::deleteObject()
{
    if (isRunning())
        stopTest();
    else
        deleteLater();
    runningState = AbstractTest::Deleting;
}

void AbstractTest::firstStartTest()
{
    try
    {
        if (getValidState() != ItIsOk) {
            message(tr("Ошибка: проблема с устройствами теста (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
            emit unsetEmit(startButton, pauseButton, stopButton);
            return;
        }
        if (getRunningState() == Running) {
            message(tr("Предупреждение: в данный момент запущен тест (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
            return;
        }
        if (getRunningState() == Paused) {
            objToThread->threadState = AbstractTest::Running;
            return;
        }
        if (getGlobalState() == FREE) {
            objToThread->threadState = AbstractTest::Running; //???
            startTest();
            return;
        } else {
            message(tr("Предупреждение: в данный момент запущен некоторый тест"));
            return;
        }
    }
    catch(const QString& exception)
    {
        message(tr("Ошибка запуска теста: ") + exception);
        objToThread->threadState = runningState = AbstractTest::ErrorIsOccured;
    }
}

bool AbstractTest::isRunning()
{
    return objToThread->threadState == AbstractTest::Running || objToThread->threadState == AbstractTest::Paused;
}

void AbstractTest::pauseTest()
{
    if (getValidState() != ItIsOk) {
        message(tr("Ошибка: проблема с устройствами теста (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
        return;
    }
    if (getRunningState() == Paused) {
        message(tr("Предупреждение: тест на паузе (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
        return;
    } else if (getRunningState() != Running) {
        message(tr("Предупреждение: тест не запущен (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
        return;
    }
    objToThread->threadState = AbstractTest::Paused;
}

void AbstractTest::stopTest()
{
    if (getValidState() != ItIsOk) {
        message(tr("Ошибка: проблема с устройствами теста (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
        return;
    }
    if ((getRunningState() == Stopped) || (getRunningState() == Completed) || (getRunningState() == ErrorIsOccured)) {   ///  || (getRunningState() == Deleting)  ???
    //if (!isRunning()) {
        message(tr("Предупреждение: тест не запущен (файл: %1, имя: %2)").arg(fileName->text()).arg(mark->text()));
        return;
    }
    objToThread->threadState = AbstractTest::Stopped;
}

AbstractTest *testLib::createTest(QVBoxLayout *devices, QTextBrowser *pB, QTextBrowser *tB, bool su)
{
    QStringList allTests;
    allTests << testTypeEcho << testTypeMemory << testTypeRegisters
             << testTypeBulbs << testTypeMonitor << testTypeTrmSingle
             << testTypeLoadSPI << testTypeVariation << testTypeBuffers
             << testTypeTrash << testTypeNoise << testTypeGroupVar
             << testTypeAlien << testTypeInvalid << testTypeLSC << testTypeLSCMes;
    if (su)
        allTests << testTypeNull;

    bool ok;
    QString testType = QInputDialog::getItem(0, QObject::tr("Создать тест"), QObject::tr("Тесты:"), allTests, 0, false, &ok);
    if (!(ok && !testType.isEmpty()))
        return NULL;
//    QString newFileStr = QFileDialog::getSaveFileName(0, QObject::tr("Введите имя файла"), QObject::tr(""), QString(), 0, QFileDialog::DontConfirmOverwrite);
//    if (newFileStr.isEmpty()) {
//        return NULL;
//    }
    QString defFileStr;

    if (testType == testTypeMemory) {
        defFileStr = QObject::tr("../default/mem_test");
    } else if (testType == testTypeRegisters) {
        defFileStr = QObject::tr("../default/reg_test");
    } else if (testType == testTypeEcho) {
        defFileStr = QObject::tr("../default/echo_test");
    } else if (testType == testTypeBulbs) {
        defFileStr = QObject::tr("../default/bulb_test");
    } else if (testType == testTypeMonitor) {
        defFileStr = QObject::tr("../default/monitor_test");
    } else if (testType == testTypeTrmSingle) {
        defFileStr = QObject::tr("../default/trm_single_test");
    } else if (testType == testTypeVariation) {
        defFileStr = QObject::tr("../default/var_comm_test");
    } else if (testType == testTypeGroupVar) {
        defFileStr = QObject::tr("../default/var_broad_test");
    } else if (testType == testTypeLoadSPI) {
        defFileStr = QObject::tr("../default/spi_part");
    } else if (testType == testTypeBuffers) {
        defFileStr = QObject::tr("../default/ram_test");
    } else if (testType == testTypeNull) {
        defFileStr = QObject::tr("../default/null_test");
    } else if (testType == testTypeTrash) {
        defFileStr = QObject::tr("../default/trash_test");
    } else if (testType == testTypeAlien) {
        defFileStr = QObject::tr("../default/alien_test");
    } else if (testType == testTypeInvalid) {
        defFileStr = QObject::tr("../default/invalid_test");
    } else if (testType == testTypeNoise) {
        defFileStr = QObject::tr("../default/noise_test");
    } else if (testType == testTypeLSC) {
        defFileStr = QObject::tr("../default/lsc_test");
    } else if (testType == testTypeLSCMes) {
        defFileStr = QObject::tr("../default/lsc_messages_test");
    }

//    if(!QFile::copy(defFileStr,newFileStr)) {
//        QMessageBox::critical(0, QObject::tr("Создать тест"), QObject::tr("Файл уже существует"));
//        return NULL;
//    }

//    return loadTest(newFileStr, devices, pB, tB);
    return loadTest(defFileStr, devices, pB, tB);
}

static QAction* findActionByName(QList<QAction*> list, QString name)
{
    for (int i=0; i<list.size(); i++)
    {
        QAction* pAct = list[i];
        if (pAct->objectName() == name)
            return pAct;
    }
    return 0;
}

void AbstractTest::connectActions()
{
    settings->addAction(findActionByName(actions(), tr("act_devMode1")));
    settings->addAction(findActionByName(actions(), tr("act_devMode2")));
    stats->addAction(findActionByName(actions(), tr("act_devMode1")));
    stats->addAction(findActionByName(actions(), tr("act_devMode2")));
}

AbstractTest *testLib::loadTest(QString settingsFileStr, QVBoxLayout *devices, QTextBrowser *pB, QTextBrowser *tB)
{
    QFile settingsFile(settingsFileStr);
    if (!settingsFile.open(QFile::ReadOnly))
        return NULL;
    QTextStream out(&settingsFile);
    QString testType = out.readLine();
    bool checked = out.readLine().toInt();
    QString markStr = out.readLine();
    settingsFile.close();
    QString uiFileStr;
    QString uiFileStr_stats;
    AbstractTest* test;
    if (testType == testTypeMemory) {
        uiFileStr = QObject::tr("../default/settings_mem_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_mem_test.ui");
        test = new MemTest(0);
    }else if (testType == testTypeBuffers) {
        uiFileStr = QObject::tr("../default/settings_ram_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_ram_test.ui");
        test = new RamTest(0);
    } else if (testType == testTypeRegisters) {
        uiFileStr = QObject::tr("../default/settings_reg_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_reg_test.ui");
        test =  new RegTest(0);
    } else if (testType == testTypeEcho) {
        uiFileStr = QObject::tr("../default/settings_echo_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_echo_test.ui");
        test = new EchoTest(0);
    } else if (testType == testTypeBulbs) {
        uiFileStr = QObject::tr("../default/settings_bulb_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_bulb_test.ui");
        test = new BulbTest(0);
    } else if (testType == testTypeMonitor) {
        uiFileStr = QObject::tr("../default/settings_monitor_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_monitor_test.ui");
        test = new MonitorTest(0);
    } else if (testType == testTypeTrmSingle) {
        uiFileStr = QObject::tr("../default/settings_trm_single_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_trm_single_test.ui");
        test = new TrmSingleTest(0);
    } else if (testType == testTypeVariation) {
        uiFileStr = QObject::tr("../default/settings_var_comm_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_var_comm_test.ui");
        test = new VarCommandTest(0);
    } else if (testType == testTypeLoadSPI) {
        uiFileStr = QObject::tr("../default/settings_spi_part.ui");
        uiFileStr_stats = QObject::tr("../default/stats_spi_part.ui");
        test = new SpiPart(0);
    } else if (testType == testTypeNull) {
        uiFileStr = QObject::tr("../default/settings_null_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_null_test.ui");
        test = new NullTest(0);
    } else if (testType == testTypeTrash) {
        uiFileStr = QObject::tr("../default/settings_trash_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_trash_test.ui");
        test = new TrashTest(0);
    } else if (testType == testTypeAlien) {
        uiFileStr = QObject::tr("../default/settings_alien_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_alien_test.ui");
        test = new AlienTest(0);
    } else if (testType == testTypeInvalid) {
        uiFileStr = QObject::tr("../default/settings_invalid_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_invalid_test.ui");
        test = new InvalidCommTest(0);
    } else if (testType == testTypeNoise) {
        uiFileStr = QObject::tr("../default/settings_noise_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_noise_test.ui");
        test = new NoiseTest(0);
    } else if (testType == testTypeGroupVar) {
        uiFileStr = QObject::tr("../default/settings_var_broad_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_var_broad_test.ui");
        test = new VarBroadTest(0);
    } else if (testType == testTypeLSC) {
        uiFileStr = QObject::tr("../default/settings_lsc_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_lsc_test.ui");
        test = new LscTest(0);
    } else if (testType == testTypeLSCMes) {
        uiFileStr = QObject::tr("../default/settings_lsc_message_test.ui");
        uiFileStr_stats = QObject::tr("../default/stats_lsc_message_test.ui");
        //test = new LscMessageTest(0);
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

    uiFile.setFileName(uiFileStr_stats);
    if (!uiFile.open(QFile::ReadOnly))
        return NULL;
    QWidget* statsForm = (QWidget*)loader.load(&uiFile);
    uiFile.close();

    test->setSettings(devices, settingsForm, checked, testType, settingsFileStr, markStr, pB, tB, statsForm);

    return test;
}

absObjToThread::absObjToThread(QObject *parent) : QObject(parent)
{
    threadState = AbstractTest::Stopped;
    connect(this, SIGNAL(resultReady(int)), SLOT(setState(int)));
}

void absObjToThread::template_doWork()
{

}

void absObjToThread::setState(int rs)
{
    threadState = (AbstractTest::RunningState)rs;
}

int absObjToThread::pause_stop()
{
    if (threadState == AbstractTest::Paused) {
        emit resultReady((int)AbstractTest::Paused);
        while (threadState == AbstractTest::Paused) { this->thread()->msleep(50);/*qDebug() << "while paused";*/  }
        if (threadState == AbstractTest::Running) {
            emit resultReady((int)AbstractTest::Running);
            return 0;
        }
        else {
        //if (threadState == AbstractTest::Stopped) {
            //emit resultReady((int)AbstractTest::Stopped);
            return -1;
        }
    } else if (threadState == AbstractTest::Stopped) {
        //emit resultReady((int)AbstractTest::Stopped);
        return -1;
    } else {
        return 0;
    }
}
