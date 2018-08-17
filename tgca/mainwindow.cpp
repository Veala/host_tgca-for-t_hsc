#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrintDialog>

static bool configurateDevice(Device *device); // не реализована !!!

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    prjLoaded(false),
    tstLoaded(false),
    su(false),
    logFile("log.txt"),
    logFileDefault("log.txt")
{
    ui->setupUi(this);

    connect(ui->actRun, SIGNAL(triggered(bool)), this, SLOT(onRunTst()));
    connect(ui->actPause, SIGNAL(triggered(bool)), this, SLOT(onPause()));
    connect(ui->actStop, SIGNAL(triggered(bool)), this, SLOT(onStop()));
    //connect(ui->actStop, SIGNAL(triggered(bool)), this, SLOT(onStopMonitor()));

    ui->outputMenu->addAction(ui->dockPro->toggleViewAction());
    ui->outputMenu->addAction(ui->dockTest->toggleViewAction());

    //setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint);

    act_devMode = new QAction(tr("Включить секретный режим"), this);
    act_devMode->setObjectName(QStringLiteral("act_devMode"));
    addAction(act_devMode);
    act_devMode->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+D", 0));
    connect(act_devMode, SIGNAL(triggered()), this, SLOT(actDevMode()));

    ui->actConfiguration->setVisible(false);
    ui->actConfiguration->setEnabled(false);

    QSettings project("../tgca/project.ini", QSettings::IniFormat);
    if (project.status() == QSettings::NoError && project.allKeys().size() >= 2 &&
        project.value("Common/autoload").toString() == "1")
            prjLoaded = loadProject(project);

    if (prjLoaded)
            ui->actSavePrj->setEnabled(true);

    treeState = stopped;
}

MainWindow::~MainWindow()
{
    if (su)
        delete devConf;
    delete ui;
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("Справка"), tr("Шифр: \"Обработка 30\"\nВерсия: 1.0\n"));
}

bool MainWindow::clearProject()
{
    bool bRet = true;
    if (su)
    {
        su = false;
        ui->actConfiguration->setVisible(false);
        ui->actConfiguration->setEnabled(false);
        if (devConf)
            delete devConf;
        devConf = 0;
    }
    logFile = logFileDefault;

    int sz = ui->tests->count();
    for (int i=sz-1; i>=0; i--)
    {
        AbstractTest *test = (AbstractTest*)ui->tests->itemAt(i)->widget();
        /// закрыть тест, выгрузить из tests
        delete test;
    };

    sz = ui->devices->count();
    for (int i=sz-1; i>=0; i--)
    {
        Device *dev = (Device*)ui->devices->itemAt(i)->widget();
        /// Здесь надо разорвать все соединения и удалить устройство
        delete dev;
    };
    qDebug() << "Done";
    return bRet;
}

static bool setupConnection(Device *device, QString ip, QString port, QString hostIp, QString hostPort)
{
    device->connection.setServerIP(ip);
    device->connection.setServerPORT(port);
    device->connection.setHostIP(hostIp);
    device->connection.setHostPORT(hostPort);
    return !ip.isEmpty() && !port.isEmpty() && !hostIp.isEmpty();
}

bool MainWindow::loadProject(QSettings& settings)
{
    if (settings.value("Common/user").toString() == "su")
        actDevMode();
    QString log = settings.value("Common/output").toString();
    if (!log.isEmpty())
        logFile = log;

    int sze = settings.beginReadArray("Devices");
    for (int i = 0; i < sze; ++i) {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        if (!name.isEmpty())
        {
            Device *device = new Device(this, name, ui->projectBrowser);
            ui->devices->addWidget(device);
            bool bConn = setupConnection(device, settings.value("IP").toString(), settings.value("port").toString(),
                                        settings.value("hostIP").toString(), settings.value("hostPort").toString());
            bool bConf = device->configuration.initFrom(settings.value("configuration").toString(), 0);
            if (bConn && bConf && settings.value("autoload").toString() == "1")
            {
                connect(this, SIGNAL(connectTry()), device, SLOT(connectTry()));
                connect(this, SIGNAL(autoConfigurate()), device, SLOT(configTry()));
                emit connectTry();
                //emit autoConfigurate();
                disconnect(this, SIGNAL(connectTry()), device, SLOT(connectTry()));
                disconnect(this, SIGNAL(autoConfigurate()), device, SLOT(configTry()));

            }
        }
    }
    settings.endArray();

    sze = settings.beginReadArray("Tests");
    for (int i = 0; i < sze; ++i) {
        settings.setArrayIndex(i);

        QString name = settings.value("test").toString();
        if (!name.isEmpty())
        {
            AbstractTest *at = testLib::loadTest(name, ui->devices, ui->projectBrowser, ui->testBrowser);
            if (at)
            {
                ui->tests->addWidget(at);
                at->setEnabled(settings.value("enabled").toString() != "0");
                tstLoaded = true;
                //at->setParent(this);
                connect(this, SIGNAL(newDev(QString)), at, SLOT(newDev(QString)));
                connect(this, SIGNAL(setTestStateIcon(int)), at, SLOT(setRunningState(int)));
                connect(at, SIGNAL(setEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(setSlot(QPushButton*,QPushButton*,QPushButton*)));
                connect(at, SIGNAL(unsetEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(unsetSlot(QPushButton*,QPushButton*,QPushButton*)));
                connect(at, SIGNAL(dragged()), this, SLOT(onDragged()));
                connect(at, SIGNAL(dropped()), this, SLOT(onDropped()));
                ui->actRun->setEnabled(true);
            }
        }
    }
    settings.endArray();
    return true;
}

void MainWindow::onLoadPrj()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл проекта"), tr(""));

    if (!fileName.isEmpty())
    {
        if (clearProject())
        {
            QSettings project(fileName, QSettings::IniFormat);
            prjLoaded = (project.status() == QSettings::NoError && project.allKeys().size() >= 2 && loadProject(project));
        }
    }
}

bool MainWindow::onSavePrj()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить в файл"), "");

    if (fileName == "")
        return false;

    QSettings ini(fileName, QSettings::IniFormat);

    ini.clear();

    ini.setValue("Common/output", logFile);
    ini.setValue("Common/autoload", "0");
    ini.setValue("Common/output", su ? "su" : "");

    ini.beginWriteArray("Devices");
    for (int j=0; j<ui->devices->count(); j++)
    {
        Device *dev = (Device*)ui->devices->itemAt(j)->widget();

        ini.setArrayIndex(j);
        ini.setValue("name", dev->getName());
        ini.setValue("IP", dev->connection.getServerIP());
        ini.setValue("port", dev->connection.getServerPORT());
        ini.setValue("hostIP", dev->connection.getHostIP());
        ini.setValue("hostPort", dev->connection.getHostPORT());
        ini.setValue("configuration", dev->configuration.getName());
        ini.setValue("autoload", "0");
    }
    ini.endArray();


    ini.beginWriteArray("Tests");
    for (int i=0; i<ui->tests->count(); i++)
    {
        AbstractTest * test = (AbstractTest*)ui->tests->itemAt(i)->widget();
        ini.setArrayIndex(i);
        ini.setValue("test", test->getName());
        ini.setValue("enabled", "1");
    }
    ini.endArray();
    return true;
}

void MainWindow::onPushConfig()
{
    devConf->show();
}

void MainWindow::addDevice()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Добавить устройство"), tr("Имя устройства:"), QLineEdit::Normal, "", &ok);
    if (!ok) return;
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Добавить устройство"), tr("Не задано имя устройства"));
        return;
    }
    Device* dev;
    for (int i=0; i<ui->devices->count(); i++) {
        dev = (Device*)ui->devices->itemAt(i)->widget();
        if (dev->getName() == name) {
            QMessageBox::information(this, tr("Добавить устройство"), tr("Ошибка: устройство %1 уже существует").arg(name));
            return;
        }
    }
    ui->devices->addWidget(new Device(this, name, ui->projectBrowser));
    ui->projectBrowser->append(tr("Устройство %1 добавлено").arg(name));
    emit newDev(name);

    if (tstLoaded)
        ui->actRun->setEnabled(true);
}

void MainWindow::loadTest()
{
    QString txtFile = QFileDialog::getOpenFileName(0, tr("Открыть файл параметров теста"), tr(""));
    if (txtFile.isEmpty()) return;
    AbstractTest* test = testLib::loadTest(txtFile, ui->devices, ui->projectBrowser, ui->testBrowser);
    if(test == NULL) return;
    test->setParent(this);
    connect(this, SIGNAL(newDev(QString)), test, SLOT(newDev(QString)));
    connect(this, SIGNAL(setTestStateIcon(int)), test, SLOT(setRunningState(int)));
    connect(test, SIGNAL(setEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(setSlot(QPushButton*,QPushButton*,QPushButton*)));
    connect(test, SIGNAL(unsetEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(unsetSlot(QPushButton*,QPushButton*,QPushButton*)));
    connect(test, SIGNAL(dragged()), this, SLOT(onDragged()));
    connect(test, SIGNAL(dropped()), this, SLOT(onDropped()));
    ui->tests->addWidget(test);
}

void MainWindow::createTest()
{
    AbstractTest* test = testLib::createTest(ui->devices, ui->projectBrowser, ui->testBrowser);
    if(test == NULL) return;
    test->setParent(this);
    connect(this, SIGNAL(newDev(QString)), test, SLOT(newDev(QString)));
    connect(this, SIGNAL(setTestStateIcon(int)), test, SLOT(setRunningState(int)));
    connect(test, SIGNAL(setEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(setSlot(QPushButton*,QPushButton*,QPushButton*)));
    connect(test, SIGNAL(unsetEmit(QPushButton*,QPushButton*,QPushButton*)), this, SLOT(unsetSlot(QPushButton*,QPushButton*,QPushButton*)));
    connect(test, SIGNAL(dragged()), this, SLOT(onDragged()));
    connect(test, SIGNAL(dropped()), this, SLOT(onDropped()));
    ui->tests->addWidget(test);
}

void MainWindow::actDevMode()
{
    if (!su)
    {
        su = true;
        ui->actConfiguration->setVisible(true);
        ui->actConfiguration->setEnabled(true);
        devConf = new Configuration();
        devConf->blockReadWrite();
    }
}

void MainWindow::onDragged()
{
    dragIndex = ui->tests->indexOf((AbstractTest*)sender());
}

void MainWindow::onDropped()
{
    dropIndex = ui->tests->indexOf((AbstractTest*)sender());
    if (dragIndex == dropIndex) return;
    QLayoutItem* tempTest = ui->tests->takeAt(dragIndex);
    ui->tests->insertItem(dropIndex, tempTest);
}

void MainWindow::onMenuDevices(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Добавить устройство"));
    connect(act, SIGNAL(triggered()), this, SLOT(addDevice()));

    menu.exec(ui->labeDevicesTitle->mapToGlobal(point));
}

void MainWindow::onMenuTests(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Добавить новый тест"));
    connect(act, SIGNAL(triggered()), this, SLOT(createTest()));
    act = menu.addAction(tr("Добавить существующий тест"));
    connect(act, SIGNAL(triggered()), this, SLOT(loadTest()));

    menu.exec(ui->labeDevicesTitle->mapToGlobal(point));
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Escape)
        QMainWindow::keyPressEvent(e);
}

void MainWindow::setTreeState(MainWindow::TreeState s)
{
    if (s == running) {
        if (curIndex == 0) message(tr("Тесты запущены"));
    } else if (s == stopped) {
        curIndex = 0;
        message(tr("Тесты остановлены"));
    } else if (s == finished) {
        curIndex = 0;
        message(tr("Тесты завершены"));
    } else if (s == next) {
        curIndex++;
    } else if (s == bigStop) {
        if (treeState!=running) {
            setTreeState(smallStop);
            return;
        }
    } else if (s == smallStop) {

    }
    treeState = s;
}

MainWindow::TreeState MainWindow::getTreeState() const
{
    return treeState;
}

void MainWindow::message(QString m)
{
    QDateTime local(QDateTime::currentDateTime());
    ui->projectBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss - ")) + m);
}

void MainWindow::run(int index)
{
    if (index == ui->tests->count()) {
        setTreeState(finished);
        return;
    }
    AbstractTest* test = (AbstractTest* )ui->tests->itemAt(index)->widget();
    if (test->isReady()) {
        emit test->globalStart();
    } else {
        setTreeState(next);
        onRunTst();
    }
}

void MainWindow::onRunTst()
{
    if (getGlobalState() == BUSY) return;
    if (getTreeState() == running) {
        return;
    } else {
        if (curIndex == 0)
            emit setTestStateIcon((int)AbstractTest::Stopped);
        setTreeState(running);
        run(curIndex);
    }
}

void MainWindow::onPause()
{
    if (getGlobalState() == BUSY) return;
    if (getTreeState() != running)
        message(tr("Тесты не запущены"));
}

void MainWindow::onStop()
{
    if (getGlobalState() == BUSY) {
        setTreeState(bigStop);
        return;
    }
    if (getTreeState() != running)
        message(tr("Тесты не запущены"));
}

void MainWindow::setSlot(QPushButton *start, QPushButton *pause, QPushButton *stop)
{
    qDebug() << "setSlots";
    connect(ui->actRun, SIGNAL(triggered(bool)), start, SIGNAL(clicked(bool)));
    connect(ui->actPause, SIGNAL(triggered(bool)), pause, SIGNAL(clicked(bool)));
    connect(ui->actStop, SIGNAL(triggered(bool)), stop, SIGNAL(clicked(bool)));
}

void MainWindow::unsetSlot(QPushButton *start, QPushButton *pause, QPushButton *stop)
{
    qDebug() << "unsetSlots";
    disconnect(ui->actRun, SIGNAL(triggered(bool)), start, SIGNAL(clicked(bool)));
    disconnect(ui->actPause, SIGNAL(triggered(bool)), pause, SIGNAL(clicked(bool)));
    disconnect(ui->actStop, SIGNAL(triggered(bool)), stop, SIGNAL(clicked(bool)));
    if (getTreeState() == running) {
        setTreeState(next);
        onRunTst();
    } else if (getTreeState() == bigStop) {
        setTreeState(stopped);
    } else if (getTreeState() == smallStop) {
        treeState = stopped;
    }
}

void MainWindow::setReport(QTextDocument& doc)
{
    QString header1 = QString("<html>\n<p><br><br><br><br><big><b><center>ОКНО ПРОЕКТА<br></center></b><br><br><br>");
    QString page1 = ui->projectBrowser->toHtml();
    QString header2 = QString("</p>\n<br style = 'page-break-after: always;'><html>\n<body>\n<p><br><br><br><br><big><b><center>ОКНО ТЕСТОВ<br></center></b><br><br><br>");
    QString page2 = ui->testBrowser->toHtml();
    doc.setHtml(header1+page1+header2+page2);

}

void MainWindow::printReport(QPrinter* prnt)
{
    QTextDocument document;
    setReport(document);
    document.print(prnt);
}

void MainWindow::onPrintRep()
{
    QPrinter prnt;
    QPrintDialog* dialog = new QPrintDialog(&prnt, this);
    connect(dialog, SIGNAL(accepted(QPrinter*)), this, SLOT(printReport(QPrinter*)));
    dialog->open();
    disconnect(dialog, SIGNAL(accepted(QPrinter*)), this, SLOT(printReport(QPrinter*)));
}

bool MainWindow::onCreateRep()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить в файл"), "", tr("Файлы pdf (*.pdf)\nВсе файлы (*.*)"));

    if (fileName == "")
        return false;

    QPrinter prnt;
    prnt.setPageSize(QPrinter::A4);
    prnt.setOutputFormat(QPrinter::PdfFormat);
    prnt.setOutputFileName(fileName);
    QTextDocument document;
    setReport(document);
    document.print(&prnt);

    return true;
}

static bool configurateDevice(Device *device)
{
    qDebug() << QObject::tr("Функция configurateDevice() не реализована !!!");
    /// Здесь надо установить соединение и потом записать регистры LLL
    return false;
}

void MainWindow::onHelp()
{
    qDebug() << "onHelp";
    qDebug() << QObject::tr("Функция onHelp() не реализована !!!");
}

