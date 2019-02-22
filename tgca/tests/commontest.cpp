#include "commontest.h"
#include "../registers.h"
#include "../gendata.h"

////////////////////////////////////////////////.////////////////////////////
///   CommonTest - базовый класс для группы тестов с двумя устройствами   ///
/////////////////////////////////////////////////////////////////////////////

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

void CommonTest::assignDevices()
{
}

void CommonTest::connectThread()
{
    objToThread->moveToThread(&testThread);

    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
//    connect(objToThread, SIGNAL(resultReady(int)), objToThread, SLOT(testTerminate(int)));
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
     /*   if (exception == "socket")
            stdOutput(tr("Ошибка сокета"), tr("Socket error"));
        else
            stdOutput(tr("Неизвестная ошибка"), tr("Unknown error"));
     */
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
    QString ip = devBC->connection.getServerIP();
    ushort port = devBC->connection.getServerPORT().toUShort();
    tcpSocketBC.connectToHost(QHostAddress(ip), port);
    if (!tcpSocketBC.waitForConnected(5000))
    {
        stdOutput(tr("Нет соединения с КШ"), tr("BC: no connection"));
        if (pause_stop() == -1)
            return AbstractTest::Stopped;
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        return AbstractTest::ErrorIsOccured;
    }
    devBC->setSocket(&tcpSocketBC);
    return AbstractTest::Running;
}

AbstractTest::RunningState commonObjToThread::connectRT()
{
    QString ip = devRT->connection.getServerIP();
    ushort port = devRT->connection.getServerPORT().toUShort();
    tcpSocketRT.connectToHost(QHostAddress(ip), port);
    if (!tcpSocketRT.waitForConnected(5000)) {
        stdOutput(tr("Нет соединения с ОУ"), tr("RT: no connection"));
        if (pause_stop() == -1)
            return AbstractTest::Stopped;
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        return AbstractTest::ErrorIsOccured;
    }
    devRT->setSocket(&tcpSocketRT);
    return AbstractTest::Running;
}
/*
void commonObjToThread::testTerminate(int code)
{
    terminate(code);
}
*/

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
