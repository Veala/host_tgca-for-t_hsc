#include "commontest.h"
#include "../registers.h"
#include "../gendata.h"

////////////////////////////////////////////////.////////////////////////////
///   CommonTest - базовый класс для группы тестов с двумя устройствами   ///
/////////////////////////////////////////////////////////////////////////////

CommonTest::CommonTest(QWidget *parent) :
    AbstractTest(parent),
    comboBoxRTA(0),
    lineEditTime(0),
    lineEditPause(0),
    lineEditReservePause(0),
    lineEditOver(0),
    checkBoxEnaInt(0),
    checkBoxEnaAddr(0),
    checkBoxUseInt(0),
    labelUseInt(0),
    checkBoxOut(0),
    lineEditDevBC(0),
    lineEditDevRT(0),
    checkBoxDevBC(0),
    checkBoxDevRT(0)
{
    dataGen.setParentTest(this);
}

int CommonTest::updateDeviceList()
{
    if (checkBoxDevBC->isChecked())
        deviceLineEditList.append(lineEditDevBC);
    if (checkBoxDevRT->isChecked())
        deviceLineEditList.append(lineEditDevRT);
    return (checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0);

}

void CommonTest::connectStatisticSlots()
{
    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));
    connect(objToThread,SIGNAL(statsOutputReady(QString,long)), this, SLOT(statsTestOutput(QString,long)));
    statsToZero();
}

void CommonTest::statsAddlabel(QString str)
{
    statsMap.insert(str, stats->findChild<QLabel*>(str));
}

void CommonTest::statsTestOutput(QString str, long n)
{
    QMap<QString, QLabel*>::iterator it = statsMap.find(str);
    if (it != statsMap.end())
    {
        QLabel* l = it.value();
        qulonglong val = l->text().toULongLong() + n;
        l->setText(QString::number(val));
        if (str != "totalIter")
        {
            QFont f = l->font();
            if (val == 0)
            {
                f.setBold(false);
                l->setStyleSheet(QLatin1String("color: rgb(0, 170, 0);\n"
                                               "background-color: rgb(255, 255, 255);"));
            }
            else
            {
                f.setBold(true);
                l->setStyleSheet(QLatin1String("color: rgb(255, 0, 0);\n"
                                               "background-color: rgb(255, 255, 255);"));
            }
            l->setFont(f);
        }
    }
}

void CommonTest::statsToZero()
{
    foreach (QLabel* l, statsMap) {
        l->setText("0");
        if (l->objectName() != "totalIter")
        {
            l->setStyleSheet(QLatin1String("color: rgb(0, 170, 0);\n"
                                           "background-color: rgb(255, 255, 255);"));
            QFont f = l->font();
            f.setBold(false);
            l->setFont(f);
        }
    }
}

void CommonTest::addDevicesTolist()
{/*
    if (checkBoxDevBC->isChecked())
        deviceLineEditList.append(lineEditDevBC);
    if (checkBoxDevRT->isChecked())
        deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability((checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0));
    */
    checkDeviceAvailability(updateDeviceList());
}

void CommonTest::changeConnections()
{
    disconnect(settings,SIGNAL(finished(int)),this,SLOT(checkDeviceAvailability(int)));
    connect(settings,SIGNAL(finished(int)),this,SLOT(updateSettings()));
    connect(this,SIGNAL(settingsClosed(int)),this,SLOT(checkDeviceAvailability(int)));
}

void CommonTest::setTestConnections()
{
    if (comboBoxRTA) {
        comboBoxRTA->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(comboBoxRTA, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuRTA(QPoint)));
    }
//    lineEditDevBC->setContextMenuPolicy(Qt::CustomContextMenu);
//    QObject::connect(lineEditDevBC, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuDevBC(QPoint)));
//    lineEditDevRT->setContextMenuPolicy(Qt::CustomContextMenu);
//    QObject::connect(lineEditDevRT, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuDevRT(QPoint)));
    if (lineEditTime) {
        //lineEditTime->setInputMethodHints(Qt::ImhDigitsOnly);
        //lineEditTime->setInputMask("009");
        lineEditTime->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(lineEditTime, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuTime(QPoint)));
    }
    if (lineEditPause) {
        //lineEditPause->setInputMethodHints(Qt::ImhDigitsOnly);
        //lineEditPause->setInputMask("009");
        lineEditPause->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(lineEditPause, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuPause(QPoint)));
    }
    if (lineEditReservePause) {
        //lineEditReservePause->setInputMethodHints(Qt::ImhDigitsOnly);
        //lineEditReservePause->setInputMask("009");
        lineEditReservePause->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(lineEditReservePause, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuResPause(QPoint)));
    }
    if (lineEditOver) {
        //lineEditOver->setInputMethodHints(Qt::ImhDigitsOnly);
        //lineEditOver->setInputMask("009");
        lineEditOver->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(lineEditOver, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuOver(QPoint)));
    }
    if (checkBoxEnaInt) {
        checkBoxEnaInt->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(checkBoxEnaInt, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuEnaInt(QPoint)));
    }
    if (checkBoxUseInt) {
        checkBoxUseInt->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(checkBoxUseInt, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuUseInt(QPoint)));
    }
    if (checkBoxEnaAddr) {
        checkBoxEnaAddr->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(checkBoxEnaAddr, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuEnaAddr(QPoint)));
    }
    if (checkBoxOut) {
        checkBoxOut->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(checkBoxOut, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuOut(QPoint)));
    }
}

void CommonTest::connectThread()
{
    objToThread->moveToThread(&testThread);

    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    connect(objToThread, SIGNAL(resultReady(int)), objToThread, SLOT(terminate(int)));
}

void CommonTest::updateSettings()
{
    // Проверка устройств. Можно проверять только с признаком необходимости.
    deviceLineEditList.clear();
    int done = updateDeviceList();
    // qDebug() << "deviceLineEditList: " << deviceLineEditList.size() << " devices";
    emit settingsClosed(done);
    // Здесь можно проверить и другие настройки.
}

void CommonTest::codesToLabel(QString codes, QLineEdit* line)
{
    if (codes.isEmpty())
        codes = "1";
    QString str(codes.at(0));
    int i=1;
    for (; i<=MAX_COMMAND_CODE && i<codes.length(); i++)
    {
        QChar ch = codes.at(i);
        if (ch != '0' && ch != '1')
        {
            message(QObject::tr("Неправильный список кодов, прерван"));
            qDebug() << QObject::tr("Error reading codes from input, truncated");
            break;
        }
        str += QObject::tr(",%1").arg(ch);
    }
    for (; i<=MAX_COMMAND_CODE; i++)
    {
        str += QObject::tr(",0");
    }
    commmonTest_applyCodesToLabel(line, str);
}


void CommonTest::applyRTAddr() { applyCurrent(comboBoxRTA); }
void CommonTest::applyDevBC() { applyCurrent(lineEditDevBC); }
void CommonTest::applyDevRT() { applyCurrent(lineEditDevRT); }
void CommonTest::applyWaitTime() { applyCurrent(lineEditTime); }
void CommonTest::applyPause() { applyCurrent(lineEditPause); }
void CommonTest::applyResPause() { applyCurrent(lineEditReservePause); }
void CommonTest::applyEnaInt() { applyCurrent(checkBoxEnaInt); }
void CommonTest::applyUseInt() { applyCurrent(checkBoxUseInt); }
void CommonTest::applyEnaAddr() { applyCurrent(checkBoxEnaAddr); }
void CommonTest::applyOutputMode() { applyCurrent(checkBoxOut); }

void CommonTest::applyOverTime() { applyCurrent(lineEditOver);
                                 qDebug() << "This id different case!!!"; }

void CommonTest::applyCurrent(QWidget* wid)
{
    QString testType = name_enabled->text();
    QString widType = wid->metaObject()->className();
    QString name = wid->objectName();
    QString val;
    if (widType == "QLineEdit")
        val = ((QLineEdit*)wid)->text();
    else if (widType == "QComboBox")
        val = ((QComboBox*)wid)->currentText();
    else if (widType == "QCheckBox" && ((QCheckBox*)wid)->isChecked())
        val = "yes";

    emit applyToAll(testType, widType, name, val);
}

void CommonTest::onMenuRTA(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyRTAddr()));
    menu.exec(comboBoxRTA->mapToGlobal(point));
}

void CommonTest::onMenuDevBC(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyDevBC()));
    menu.exec(lineEditDevBC->mapToGlobal(point));
}

void CommonTest::onMenuDevRT(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyDevRT()));
    menu.exec(lineEditDevRT->mapToGlobal(point));
}

void CommonTest::onMenuTime(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyWaitTime()));
    menu.exec(lineEditTime->mapToGlobal(point));
}

void CommonTest::onMenuPause(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyPause()));
    menu.exec(lineEditPause->mapToGlobal(point));
}

void CommonTest::onMenuResPause(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyResPause()));
    menu.exec(lineEditReservePause->mapToGlobal(point));
}

void CommonTest::onMenuOver(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyOverTime()));
    menu.exec(lineEditOver->mapToGlobal(point));
}

void CommonTest::onMenuEnaInt(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyEnaInt()));
    menu.exec(checkBoxEnaInt->mapToGlobal(point));
}

void CommonTest::onMenuUseInt(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyUseInt()));
    menu.exec(checkBoxUseInt->mapToGlobal(point));
}

void CommonTest::onMenuEnaAddr(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyEnaAddr()));
    menu.exec(checkBoxEnaAddr->mapToGlobal(point));
}

void CommonTest::onMenuOut(QPoint point)
{
    QMenu menu;
    QAction *act = menu.addAction(tr("Применить ко всем тестам"));
    connect(act, SIGNAL(triggered()), this, SLOT(applyOutputMode()));
    menu.exec(checkBoxOut->mapToGlobal(point));
}

///////////////////////////////////////////////////////////////////////////////////////
///   commonObjToThread - базовый класс потока группы тестов с двумя устройствами   ///
///////////////////////////////////////////////////////////////////////////////////////

void commonObjToThread::doWork()
{
    try
    {
        emit resultReady((int)AbstractTest::Running);
        perform();
    }
    catch(const QString& exception)
    {
        stdOutput(tr("Ошибка: ") + exception, tr("Error: ") + exception);
        emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}

void commonObjToThread::stdOutput(QString message_rus, QString message_eng)
{
    if (outEnable)
        emit outputReady(message_rus);
    else
        qDebug() << message_eng;
}

AbstractTest::RunningState commonObjToThread::connectBC()
{
    try
    {
        devBC->tryToConnect();
        qDebug() << "BC connected";
        return AbstractTest::Running;
    }
    catch(const QString&)
    {
        qDebug() << "Connection failed";
        stdOutput(tr("Нет соединения с КШ"), tr("BC: no connection"));

        if (pause_stop() == -1)
            return AbstractTest::Stopped;

        emit resultReady((int)AbstractTest::ErrorIsOccured);
        return AbstractTest::ErrorIsOccured;
    }
}

AbstractTest::RunningState commonObjToThread::connectRT()
{
    try
    {
        devRT->tryToConnect();
        qDebug() << "RT connected";
        return AbstractTest::Running;
    }
    catch(const QString&)
    {
        qDebug() << "Connection failed";
        stdOutput(tr("Нет соединения с ОУ"), tr("RT: no connection"));
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        return AbstractTest::ErrorIsOccured;
    }
}

// Оконный режим
void commonObjToThread::switchWindow(int n)
{
    devRT->reg_aux_winmode.mode = n;
    devBC->reg_aux_winmode.mode = n;
    devRT->writeReg(&devRT->reg_aux_winmode);
    devBC->writeReg(&devBC->reg_aux_winmode);
}

bool commonObjToThread::isRunning()
{
    return threadState == AbstractTest::Running || threadState == AbstractTest::Paused;
}

void commonObjToThread::initStartBC()
{
    quint32 zero = 0;
    devBC->reg_hsc_creg.setData(zero);
    devBC->reg_hsc_creg.start_bc = 1;
}

void commonObjToThread::terminate(int )
{
    if (!isRunning())
    {
        if (devBC)
            devBC->tryToDisconnect();
        if (devRT)
            devRT->tryToDisconnect();
        this->destroyData();
    }
}

///////////////////////////////////////////////////
///   dataGeneration - класс генерации данных   ///
///////////////////////////////////////////////////

void dataGeneration::settings()
{
    radioButtonLin = parentTestSettings()->findChild<QRadioButton*>("radioButtonLin");
    lineEditBegin = parentTestSettings()->findChild<QLineEdit*>("lineEditBegin");
    lineEditStep = parentTestSettings()->findChild<QLineEdit*>("lineEditStep");
    lineEditNumStep = parentTestSettings()->findChild<QLineEdit*>("lineEditNumStep");
    lineEditBufLen = parentTestSettings()->findChild<QLineEdit*>("lineEditBufLen");
    labelBegin = parentTestSettings()->findChild<QLabel*>("labelBegin");
    labelStep = parentTestSettings()->findChild<QLabel*>("labelStep");
    labelNumStep = parentTestSettings()->findChild<QLabel*>("labelNumStep");
    radioButtonRand = parentTestSettings()->findChild<QRadioButton*>("radioButtonRand");
    comboBoxUnit = parentTestSettings()->findChild<QComboBox*>("comboBoxUnit");
}

QDialog* dataGeneration::parentTestSettings()
{
    return ((CommonTest*)test)->getSettings();
}

QString dataGeneration::genType() const { return (radioButtonRand->isChecked() ? "Random" :  "Linear"); }
QString dataGeneration::begin() const { return lineEditBegin->text(); }
QString dataGeneration::step() const { return lineEditStep->text(); }
QString dataGeneration::unit() const { return comboBoxUnit->currentText(); }
QString dataGeneration::dataLen() const { return lineEditBufLen->text(); }

QString dataGeneration::numStep() const
{
    QString str = lineEditNumStep->text();
    return ((str.isEmpty() || str.toInt(0,16) == 0) ? "" : str);
}

void dataGeneration::init (QString genDataType, QString genDataBegin, QString genDataStep,
                           QString genDataNumStep, QString genDataUnit, QString genDataLen)
{
    radioButtonLin->setChecked(genDataType != "Random");
    radioButtonRand->setChecked(!radioButtonLin->isChecked());
    lineEditBegin->setText(genDataBegin);
    lineEditStep->setText(genDataStep);
    lineEditNumStep->setText(genDataNumStep);
    comboBoxUnit->setCurrentText(genDataUnit);
    if (lineEditBufLen != 0 && !genDataLen.isEmpty())
        lineEditBufLen->setText(genDataLen);
    onRadioData();
    connect(radioButtonLin, SIGNAL(toggled(bool)), this, SLOT(onRadioData()));
}

void dataGeneration::onRadioData()
{
    labelBegin->setText(radioButtonLin->isChecked() ? tr("Начальное значение (hex)") : tr("Зерно инициализации"));
    labelStep->setText(radioButtonLin->isChecked() ? tr("Приращение (со знаком)") : tr("Минимум (hex)"));
    labelNumStep->setText(radioButtonLin->isChecked() ? tr("Число шагов до сброса") : tr("Максимум (hex)"));
    if (!radioButtonLin->isChecked() && lineEditNumStep->text().isEmpty())
        lineEditNumStep->setText("0");
}

void dataGeneration::enable(bool b)
{
    radioButtonLin->setEnabled(b);
    radioButtonRand->setEnabled(b);
    lineEditBegin->setEnabled(b);
    lineEditStep->setEnabled(b);
    lineEditNumStep->setEnabled(b);
    parentTestSettings()->findChild<QLabel*>("labelHeaderData")->setEnabled(b);
    labelBegin->setEnabled(b);
    labelStep->setEnabled(b);
    labelNumStep->setEnabled(b);
    //comboBoxUnit->setEnabled(b);
    if (lineEditBufLen)
    {
        lineEditBufLen->setEnabled(b);
        parentTestSettings()->findChild<QLabel*>("labelBufLen")->setEnabled(b);
    }
}

long dataGeneration::getDataLen() const
{
    long len = 0;
    if (lineEditBufLen)
    {
        len = lineEditBufLen->text().toLong(0, 10);
        if (len < 0)
            len = 0;
    }
    return len;
}

void *dataGeneration::createData(long numb, int numcopy)
{
    if (radioButtonLin->isChecked())
        return createRegularData(numb, lineEditBegin->text().toInt(0, 16), lineEditStep->text().toInt(0, 10),
                                    lineEditNumStep->text().isEmpty() ? 0 : lineEditNumStep->text().toInt(0, 10),
                                    comboBoxUnit->currentText().toInt(0,10), numcopy);
    else
        return createRandomData(numb, lineEditBegin->text().toInt(0, 16), lineEditStep->text().toInt(0, 16),
                                    lineEditNumStep->text().toInt(0, 16), comboBoxUnit->currentText().toInt(0,10), numcopy);
}


///////////////////////////////////////////////////////////////////////////////////////
///   общие функции для тестов класса CommonTest, не обращающиеся к членам класса   ///
///////////////////////////////////////////////////////////////////////////////////////

void commmonTest_applyCodesToLabel(QLineEdit* line, QString str)
{
    line->setText(str);
    line->setToolTip(str);
    line->setCursorPosition(0);
}

QString commmonTest_labelToCodes(QLineEdit* line)
{
    QString codes;
    QString str = line->text();
    for (int i=0; i<=MAX_COMMAND_CODE; i++)
        codes.append(str.at(i*2));
    return codes;
}
