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
    comboBoxAmpl = settings->findChild<QComboBox*>("comboBoxAmpl");
    comboBoxUnit = settings->findChild<QComboBox*>("comboBoxUnit");

    checkBoxBroad = settings->findChild<QCheckBox *>("checkBoxBroad");
    checkBoxCodec = settings->findChild<QCheckBox *>("checkBoxCodec");
    checkBoxEnaInt = settings->findChild<QCheckBox *>("checkBoxEnaInt");
    checkBoxUseInt = settings->findChild<QCheckBox *>("checkBoxUseInt");
    checkBoxOut = settings->findChild<QCheckBox *>("checkBoxOut");

    radioButtonLin = settings->findChild<QRadioButton *>("radioButtonLin");

    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    deviceLineEditList.append(lineEditDevBC);
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");
    deviceLineEditList.append(lineEditDevRT);

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
    comboBoxAmpl->setCurrentText(out.readLine());

    radioButtonLin->setChecked(out.readLine() != "Random");
    lineEditBegin->setText(out.readLine());
    lineEditStep->setText(out.readLine());
    lineEditNumStep->setText(out.readLine());
    comboBoxUnit->setCurrentText(out.readLine());

    lineEditTime->setText(out.readLine());
    checkBoxUseInt->setChecked(!out.readLine().isEmpty());
    checkBoxOut->setChecked(!out.readLine().isEmpty());

    labelLen->setText(QString(tr("Длина сообщения в байтах (0-%1)").arg(maxNumByte())));
    labelBegin->setText(radioButtonLin->isChecked() ? "Начальное значение" : "Зерно инициализации  ");
    labelStep->setText(radioButtonLin->isChecked() ? "Шаг" : "Минимум");
    labelNumStep->setText(radioButtonLin->isChecked() ? "Число шагов до сброса" : "Максимум");
    radioButtonRand->setChecked(!radioButtonLin->isChecked());

    lineEditDevBC->setText(out.readLine());
    lineEditDevRT->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

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

}

void TrmSingleTest::startTest()
{

}

void TrmSingleTest::onRadio()
{
    labelBegin->setText(radioButtonLin->isChecked() ? "Начальное значение" : "Зерно инициализации");
    labelStep->setText(radioButtonLin->isChecked() ? "Шаг" : "Минимум");
    labelNumStep->setText(radioButtonLin->isChecked() ? "Число шагов до сброса" : "Максимум");
}

void TrmSingleTest::onCodec()
{
    labelLen->setText(QString(tr("Длина сообщения в байтах (0-%1)").arg(maxNumByte())));
}

void TrmSingleTest::onManType()
{
    labelLen->setText(QString(tr("Длина сообщения в байтах (0-%1)").arg(maxNumByte())));
}

unsigned int TrmSingleTest::maxNumByte()
{
    return calcNumWordInSymbol(comboBoxManType->currentIndex(), checkBoxCodec->isChecked())*MAXNUMSYMBINPACK*4 - 4;
}
