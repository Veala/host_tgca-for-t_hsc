#include "trmsingletest.h"
#include "../registers.h"

void TrmSingleTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,pB,tB);

    lineEditLen = settings->findChild<QLineEdit *>("lineEditLen");
    lineEditBegin = settings->findChild<QLineEdit *>("lineEditBegin");
    lineEditStep = settings->findChild<QLineEdit *>("lineEditStep");
    lineEditNumStep = settings->findChild<QLineEdit *>("lineEditNumStep");
    lineEditTime = settings->findChild<QLineEdit *>("lineEditTime");

    comboBoxRTA = settings->findChild<QComboBox*>("comboBoxRTA");
    comboBoxCode = settings->findChild<QComboBox*>("comboBoxCode");
    comboBoxManType = settings->findChild<QComboBox*>("comboBoxManType");
    comboBoxAmplBC = settings->findChild<QComboBox*>("comboBoxAmplBC");
    comboBoxAmplRT = settings->findChild<QComboBox*>("comboBoxAmplRT");
    comboBoxUnit = settings->findChild<QComboBox*>("comboBoxUnit");

    checkBoxBroad = settings->findChild<QCheckBox *>("checkBoxBroad");
    checkBoxCodec = settings->findChild<QCheckBox *>("checkBoxCodec");
    checkBoxEnaInt = settings->findChild<QCheckBox *>("checkBoxEnaInt");
    checkBoxEnaAddr = settings->findChild<QCheckBox *>("checkBoxEnaAddr");
    checkBoxUseInt = settings->findChild<QCheckBox *>("checkBoxUseInt");
    checkBoxOut = settings->findChild<QCheckBox *>("checkBoxOut");
    checkBoxDevBC = settings->findChild<QCheckBox *>("checkBoxDevBC");
    checkBoxDevRT = settings->findChild<QCheckBox *>("checkBoxDevRT");

    radioButtonLin = settings->findChild<QRadioButton *>("radioButtonLin");

    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");

    labelLen = settings->findChild<QLabel *>("labelLen");
    labelBegin = settings->findChild<QLabel *>("labelBegin");
    labelStep = settings->findChild<QLabel *>("labelStep");
    labelNumStep = settings->findChild<QLabel *>("labelNumStep");
    radioButtonRand = settings->findChild<QRadioButton *>("radioButtonRand");
top_1
    lineEditLen->setText(out.readLine());
    comboBoxRTA->setCurrentText(out.readLine());
    comboBoxCode->setCurrentText(out.readLine());
    checkBoxBroad->setChecked(!out.readLine().isEmpty());

    comboBoxManType->setCurrentText(out.readLine());
    checkBoxCodec->setChecked(!out.readLine().isEmpty());
    checkBoxEnaInt->setChecked(!out.readLine().isEmpty());
    checkBoxEnaAddr->setChecked(!out.readLine().isEmpty());
    comboBoxAmplBC->setCurrentText(out.readLine());
    comboBoxAmplRT->setCurrentText(out.readLine());

    radioButtonLin->setChecked(out.readLine() != "Random");
    lineEditBegin->setText(out.readLine());
    lineEditStep->setText(out.readLine());
    lineEditNumStep->setText(out.readLine());
    comboBoxUnit->setCurrentText(out.readLine());

    lineEditTime->setText(out.readLine());
    checkBoxUseInt->setChecked(!out.readLine().isEmpty());
    checkBoxOut->setChecked(!out.readLine().isEmpty());

    recalc();
    labelBegin->setText(radioButtonLin->isChecked() ? "Начальное значение" : "Зерно инициализации  ");
    labelStep->setText(radioButtonLin->isChecked() ? "Шаг" : "Минимум");
    labelNumStep->setText(radioButtonLin->isChecked() ? "Число шагов до сброса" : "Максимум");
    radioButtonRand->setChecked(!radioButtonLin->isChecked());

    lineEditDevBC->setText(out.readLine());
    checkBoxDevBC->setChecked(!out.readLine().isEmpty());
    lineEditDevRT->setText(out.readLine());
    checkBoxDevRT->setChecked(!out.readLine().isEmpty());
    settingsFile.close();

    if (checkBoxDevBC->isChecked())
        deviceLineEditList.append(lineEditDevBC);
    if (checkBoxDevRT->isChecked())
        deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability((checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0));

    connect(comboBoxManType, SIGNAL(activated(int)), this, SLOT(onManType()));
    connect(checkBoxCodec, SIGNAL(clicked(bool)), this, SLOT(onCodec()));
    connect(radioButtonLin, SIGNAL(toggled(bool)), this, SLOT(onRadio()));

    objToThread = new trmSingleObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));

    testThread.start();
}

void TrmSingleTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << lineEditLen->text() << endl;
    in << comboBoxRTA->currentText() << endl;
    in << comboBoxCode->currentText() << endl;
    in << (checkBoxBroad->isChecked() ? "1" : "") << endl;

    in << comboBoxManType->currentText() << endl;
    in << (checkBoxCodec->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaAddr->isChecked() ? "1" : "") << endl;
    in << comboBoxAmplBC->currentText() << endl;
    in << comboBoxAmplRT->currentText() << endl;

    in << (radioButtonLin->isChecked() ? "Linear" : "Random") << endl;
    in << lineEditBegin->text() << endl;
    in << lineEditStep->text() << endl;
    QString str = lineEditNumStep->text();
    in << ((str.isEmpty() || str.toInt(0,10) == 0) ? "" : str) << endl;
    in << comboBoxUnit->currentText() << endl;

    in << lineEditTime->text() << endl;
    in << (checkBoxUseInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxOut->isChecked() ? "1" : "") << endl;

    in << lineEditDevBC->text() << endl;
    in << (checkBoxDevBC->isChecked() ? "1" : "") << endl;
    in << lineEditDevRT->text() << endl;
    in << (checkBoxDevRT->isChecked() ? "1" : "") << endl;

    settingsFile.close();
}

void TrmSingleTest::onRadio()
{
    labelBegin->setText(radioButtonLin->isChecked() ? "Начальное значение" : "Зерно инициализации");
    labelStep->setText(radioButtonLin->isChecked() ? "Шаг" : "Минимум");
    labelNumStep->setText(radioButtonLin->isChecked() ? "Число шагов до сброса" : "Максимум");
}

void TrmSingleTest::onCodec()
{
    recalc();
}

void TrmSingleTest::onManType()
{
    recalc();
}

unsigned int TrmSingleTest::maxNumByte()
{
    return calcNumWordInSymbol(comboBoxManType->currentIndex(), checkBoxCodec->isChecked())*MAXNUMSYMBINPACK*4 - 4;
}

void TrmSingleTest::recalc()
{
    mnb = maxNumByte();
    labelLen->setText(QString(tr("Длина сообщения в байтах (0-%1)").arg(mnb)));
    if (lineEditLen->text() != "max" && lineEditLen->text().toInt() > mnb)
        lineEditLen->setText("max");
}

void TrmSingleTest::done1(int)
{
    // Проверка устройств. Можно проверять только с признаком необходимости.
    int done = (checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0);
    emit settingsClosed(done);
    // Здесь можно проверить и другие настройки.
}

void TrmSingleTest::startTest()
{
    trmSingleObjToThread* curThread = (trmSingleObjToThread*)objToThread;
    if (deviceList.count() != 2)
    {
        message(tr("Two devices are required"));
        return;
    }
    curThread->devBC = deviceList.at(0);
    curThread->socketDescBC = curThread->devBC->rw_socket.socketDescriptor();
    if (curThread->socketDescBC == -1) {
        message(tr("Socket descriptor error"));
        return;
    }
    curThread->devRT = deviceList.at(1);
    curThread->socketDescBC = curThread->devBC->rw_socket.socketDescriptor();
    if (curThread->socketDescBC == -1) {
        message(tr("Socket descriptor error"));
        return;
    }

    int cfgBC = curThread->devRT->configuration.getConfigReg();
          //  config(curThread->devRT, )

/* 1
    curThread->setPause(lineEditInterval);
    curThread->setIter(lineEditIterate);
*/
    /* 2

    uint command, cfgBC, cfgRT, amplBC, amplRT, time, rta;
    bool broad, useInt, output;
    char* data;
    long inCycle;
    QString mode;


 */
    /* 3

    in << lineEditLen->text() << endl;
    in << comboBoxRTA->currentText() << endl;
    in << comboBoxCode->currentText() << endl;
    in << (checkBoxBroad->isChecked() ? "1" : "") << endl;

    in << comboBoxManType->currentText() << endl;
    in << (checkBoxCodec->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaAddr->isChecked() ? "1" : "") << endl;
    in << comboBoxAmplBC->currentText() << endl;
    in << comboBoxAmplRT->currentText() << endl;

    in << (radioButtonLin->isChecked() ? "Linear" : "Random") << endl;
    in << lineEditBegin->text() << endl;
    in << lineEditStep->text() << endl;
    QString str = lineEditNumStep->text();
    in << ((str.isEmpty() || str.toInt(0,10) == 0) ? "" : str) << endl;
    in << comboBoxUnit->currentText() << endl;

    in << lineEditTime->text() << endl;
    in << (checkBoxUseInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxOut->isChecked() ? "1" : "") << endl;

      */

    emit startTestTh();
}

void trmSingleObjToThread::doWork()
{
    if (tcpSocketBC.state() != QAbstractSocket::ConnectedState ||
        !tcpSocketBC.setSocketDescriptor(socketDescBC) ||
        tcpSocketBC.state() != QAbstractSocket::ConnectedState ||
        !tcpSocketBC.setSocketDescriptor(socketDescRT))
    {
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
    }

    emit resultReady((int)AbstractTest::Running);
    qDebug() <<  "Transmit test started";

}
