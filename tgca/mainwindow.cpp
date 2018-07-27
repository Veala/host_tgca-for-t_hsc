#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "device.h"
#include "abstracttest.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
//#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    prjLoaded(false),
    tstLoaded(false),
    inRun(false),
    inPause(false),
    su(false),
    logFile("log.txt")
{
    ui->setupUi(this);
    //setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint);

    act_devMode = new QAction(tr("Включить секретный режим"), this);
    act_devMode->setObjectName(QStringLiteral("act_devMode"));
    addAction(act_devMode);
    act_devMode->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+D", 0));
    connect(act_devMode, SIGNAL(triggered()), this, SLOT(actDevMode()));

    ui->actConfiguration->setVisible(false);
    ui->actConfiguration->setEnabled(false);

    QSettings project("project.txt", QSettings::IniFormat);
    if (project.status() == QSettings::NoError && project.allKeys().size() >= 2 &&
        project.value("Common/autoload").toString() == "1")
            prjLoaded = loadProject(project);

    if (prjLoaded)
            ui->actSavePrj->setEnabled(true);
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

static bool setupConnection(Device *device, QString ip, QString port, QString hostIp, QString hodtPort)
{
    qDebug() << "Функция setupConnection() не реализована !!!";
    ///  LLL
    return false;
}

static bool configurateDevice(Device *device)
{
    qDebug() << "Функция configurateDevice() не реализована !!!";
    /// Здесь надо установить соединение и потом записать регистры LLL
    return false;
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
            Device *device = new Device(0, name);
            ui->devices->addWidget(device);
            if (/*setupConnection(device, settings.value("IP").toString(), settings.value("port").toString(),
                            settings.value("host IP").toString(), settings.value("host port").toString()) &&*/
            device->configuration.initFrom(settings.value("configuration").toString()) &&
            settings.value("autoload").toString() == "1")
                configurateDevice(device);
        }
    }
    settings.endArray();

    sze = settings.beginReadArray("Tests");
    for (int i = 0; i < sze; ++i) {
        settings.setArrayIndex(i);

        QString name = settings.value("test").toString();
        if (!name.isEmpty())
        {
            AbstractTest *at = testLib::loadTest(name, ui->devices);
            if (at)
            {
                ui->tests->addWidget(at);
                at->setEnabled(settings.value("enabled").toString() != "0");
                tstLoaded = true;
                ui->actRun->setEnabled(true);
            }
        }
    }
    settings.endArray();
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
    ui->devices->addWidget(new Device(0, name));

    if (tstLoaded)
        ui->actRun->setEnabled(true);
}

void MainWindow::loadTest()
{
    QString uiFile = QFileDialog::getOpenFileName(0, tr("Open File"), tr(""), tr("ui file (*.ui)"));
    if (uiFile.isEmpty()) return;
    AbstractTest* test = testLib::loadTest(uiFile, ui->devices);
    ui->tests->addWidget(test);
}

void MainWindow::createTest()
{
    AbstractTest* test = testLib::createTest(ui->devices);
    ui->tests->addWidget(test);
}

void MainWindow::actDevMode()
{
    if (!su)
    {
        su = true;
        ui->actConfiguration->setVisible(true);
        ui->actConfiguration->setEnabled(true);
        qDebug() << "Developer mode";
        devConf = new Configuration();
    }
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

void MainWindow::onLoadPrj()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл проекта"), "/home/", tr("Файлы проекта (*.txt)\nВсе файлы (*.*)"));

    if (!fileName.isEmpty())
    {
        clearProject();
        QSettings project(fileName, QSettings::IniFormat);
        prjLoaded = (project.status() == QSettings::NoError && project.allKeys().size() >= 2 && loadProject(project));
    }
}

/////////////////////////////////////////////////
///    НЕ РЕАЛИЗОВАНЫ:

void MainWindow::onRunTst()
{
    qDebug() << "onRunTst";

    ui->actPrintRep->setEnabled(true);
    ui->actCreateRep->setEnabled(true);
    ui->actStop->setEnabled(true);
    ui->actPause->setEnabled(true);
    ui->actRun->setEnabled(false);

    inRun = true;
    inPause = false;
}

void MainWindow::onStop()
{
    qDebug() << "onStop";

    ui->actStop->setEnabled(false);
    ui->actPause->setEnabled(false);
    ui->actRun->setEnabled(true);

    inRun = false;
    inPause = false;
}

void MainWindow::onPause()
{
    qDebug() << "onPause";

    ui->actStop->setEnabled(inPause);
    inPause = !inPause;
}

void MainWindow::onSavePrj()
{
    qDebug() << "onSavePrj";
}

void MainWindow::clearProject()
{
}

void MainWindow::onHelp()
{
    qDebug() << "onHelp";
}

void MainWindow::onPrintRep()
{
    qDebug() << "onPrintRep";
}

void MainWindow::onCreateRep()
{
    qDebug() << "onCreateRep";
}
