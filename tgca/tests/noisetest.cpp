
#include "noisetest.h"

void NoiseTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    defineFields();
    load(fName);

    deviceLineEditList.append(lineEditDevBC);
    deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability(3);

    objToThread = new noiseObjToThread();

    connectThread();

    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("totalBit", stats->findChild<QLabel*>("totalBit"));
    statsMap.insert("errPack", stats->findChild<QLabel*>("errPack"));
    statsMap.insert("errBit", stats->findChild<QLabel*>("errBit"));
    connectStats();

    testThread.start();
}

void NoiseTest::defineFields()
{
    // тип команды
    comboBoxTestType = settings->findChild<QComboBox*>("comboBoxTestType");
    // длина пакета
    lineEditNumS = settings->findChild<QLineEdit*>("lineEditNumS");
    // условие выхода
    comboBoxExitRule = settings->findChild<QComboBox*>("comboBoxExitRule");
    // зацикливание
    radioButtonSingle = settings->findChild<QRadioButton*>("radioButtonSingle");
    radioButtonUnlimited = settings->findChild<QRadioButton*>("radioButtonUnlimited");
    radioButtonEnter = settings->findChild<QRadioButton*>("radioButtonEnter");
    spinBoxCycle = settings->findChild<QSpinBox*>("spinBoxCycle");
    // вывод
    checkBoxParamView = settings->findChild<QCheckBox*>("checkBoxParamView");
    labelParamView = settings->findChild<QLabel*>("labelParamView");
    checkBoxOut = settings->findChild<QCheckBox*>("checkBoxOut");
    comboBoxShow = settings->findChild<QComboBox*>("comboBoxShow");
    // скорость
    comboBoxSpeed = settings->findChild<QComboBox*>("comboBoxSpeed");
    lineEditOver = settings->findChild<QLineEdit*>("lineEditOver");
    labelOver = settings->findChild<QLabel*>("labelOver");
    // генерация данных
    dataGen.settings();
    // регистры
    checkBoxInit = settings->findChild<QCheckBox*>("checkBoxInit");
    checkBoxCodec = settings->findChild<QCheckBox*>("checkBoxCodec");
    checkBoxEnaInt = settings->findChild<QCheckBox*>("checkBoxEnaInt");
    checkBoxEnaAddr = settings->findChild<QCheckBox*>("checkBoxEnaAddr");
    checkBoxRTALoad = settings->findChild<QCheckBox*>("checkBoxRTALoad");
    comboBoxRTA = settings->findChild<QComboBox*>("comboBoxRTA");
    comboBoxManType = settings->findChild<QComboBox*>("comboBoxManType");
    checkBoxConfRegLoad = settings->findChild<QCheckBox*>("checkBoxConfRegLoad");
    // проверки
    comboBoxWrongCfgReg = settings->findChild<QComboBox*>("comboBoxWrongCfgReg");
    comboBoxErrRegComp = settings->findChild<QComboBox*>("comboBoxErrRegComp");
    comboBoxErrStatusBC = settings->findChild<QComboBox*>("comboBoxErrStatusBC");
    comboBoxErrStatusRT = settings->findChild<QComboBox*>("comboBoxErrStatusRT");
    comboBoxBCIntErr = settings->findChild<QComboBox*>("comboBoxBCIntErr");
    comboBoxCheckRTA = settings->findChild<QComboBox*>("comboBoxCheckRTA");
    // устройства
    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");
    // паузы
    lineEditWinModePause = settings->findChild<QLineEdit*>("lineEditWinModePause");
    lineEditTime = settings->findChild<QLineEdit*>("lineEditTime");
    lineEditPause = settings->findChild<QLineEdit*>("lineEditPause");
    // прерывание
    checkBoxUseInt = settings->findChild<QCheckBox*>("checkBoxUseInt");
    labelUseInt = settings->findChild<QLabel*>("labelUseInt");
}

void NoiseTest::statsToZero()
{
    foreach (QLabel* l, statsMap) {
        l->setText("0");
        if (l->objectName() != "totalIter" && l->objectName() != "totalBit")
        {
            l->setStyleSheet(QLatin1String("color: rgb(0, 170, 0);\n"
                                           "background-color: rgb(255, 255, 255);"));
            QFont f = l->font();
            f.setBold(false);
            l->setFont(f);
        }
    }
}

void NoiseTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)

    // основные
    in << comboBoxTestType->currentIndex() << endl;  // тип команды
    in << lineEditNumS->text() << endl;              // длина пакета
    in << comboBoxExitRule->currentIndex() << endl;  // условие выхода
    // зацикливание
    in << spinBoxCycle->text() << endl;
    int cycleMode = radioButtonEnter->isChecked() ? 0 : (radioButtonSingle->isChecked() ? 1 : 2);
    in << cycleMode << endl;
    // вывод
    in << (checkBoxOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxParamView->isChecked() ? "1" : "") << endl;
    in << comboBoxShow->currentIndex() << endl;
    // скорость
    in << comboBoxSpeed->currentIndex() << endl;
    in << lineEditOver->text() << endl;
    // генерация данных
    in << dataGen.genType() << endl;
    in << dataGen.begin() << endl;
    in << dataGen.step() << endl;
    in << dataGen.numStep() << endl;
    in << dataGen.unit() << endl;
    in << dataGen.dataLen() << endl;
    // регистры
    in << (checkBoxInit->isChecked() ? "1" : "") << endl;
    in << (checkBoxCodec->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaAddr->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTALoad->isChecked() ? "1" : "") << endl;
    in << comboBoxManType->currentText() << endl;
    in << comboBoxRTA->currentText() << endl;
    in << (checkBoxConfRegLoad->isChecked() ? "1" : "") << endl;
    // проверки
    in << comboBoxWrongCfgReg->currentIndex() << endl;
    in << comboBoxErrRegComp->currentIndex() << endl;
    in << comboBoxErrStatusBC->currentIndex() << endl;
    in << comboBoxErrStatusRT->currentIndex() << endl;
    in << comboBoxBCIntErr->currentIndex() << endl;
    in << comboBoxCheckRTA->currentIndex() << endl;
    // устройства
    in << lineEditDevBC->text() << endl;
    in << lineEditDevRT->text() << endl;
    // паузы
    in << lineEditWinModePause->text() << endl;
    in << lineEditTime->text() << endl;
    in << lineEditPause->text() << endl;
    // прерывание
    in << (checkBoxUseInt->isChecked() ? "1" : "") << endl;
}

void NoiseTest::load(QString fName)
{
top_1
    // основные
    comboBoxTestType->setCurrentIndex(out.readLine().toInt());  // тип команды
    lineEditNumS->setText(out.readLine());                      // длина пакета
    comboBoxExitRule->setCurrentIndex(out.readLine().toInt());  // условие выхода
    // зацикливание
    spinBoxCycle->setValue(out.readLine().toInt());
    int cycleMode = out.readLine().toInt();
    if (cycleMode == 0)
        radioButtonEnter->setChecked(true);
    else if (cycleMode == 1)
        radioButtonSingle->setChecked(true);
    else
        radioButtonUnlimited->setChecked(true);
    // вывод
    checkBoxOut->setChecked(!out.readLine().isEmpty());
    checkBoxParamView->setChecked(!out.readLine().isEmpty());
    comboBoxShow->setCurrentIndex(out.readLine().toInt());
    // скорость
    comboBoxSpeed->setCurrentIndex(out.readLine().toInt());
    lineEditOver->setText(out.readLine());
    // генерация данных
    QString genDataType = out.readLine();
    QString genDataBegin = out.readLine();
    QString genDataStep = out.readLine();
    QString genDataNumStep = out.readLine();
    QString genDataUnit = out.readLine();
    QString genDataLen = out.readLine();
    dataGen.init(genDataType, genDataBegin, genDataStep, genDataNumStep, genDataUnit, genDataLen);
    // регистры
    checkBoxInit->setChecked(!out.readLine().isEmpty());
    checkBoxCodec->setChecked(!out.readLine().isEmpty());
    checkBoxEnaInt->setChecked(!out.readLine().isEmpty());
    checkBoxEnaAddr->setChecked(!out.readLine().isEmpty());
    checkBoxRTALoad->setChecked(!out.readLine().isEmpty());
    comboBoxManType->setCurrentText(out.readLine());
    comboBoxRTA->setCurrentText(out.readLine());
    checkBoxConfRegLoad->setChecked(!out.readLine().isEmpty());
    // проверки
    comboBoxWrongCfgReg->setCurrentIndex(out.readLine().toInt());
    comboBoxErrRegComp->setCurrentIndex(out.readLine().toInt());
    comboBoxErrStatusBC->setCurrentIndex(out.readLine().toInt());
    comboBoxErrStatusRT->setCurrentIndex(out.readLine().toInt());
    comboBoxBCIntErr->setCurrentIndex(out.readLine().toInt());
    //comboBoxCheckRTA->setCurrentIndex(out.readLine().toInt());
    // устройства
    lineEditDevBC->setText(out.readLine());
    lineEditDevRT->setText(out.readLine());
    // паузы
    lineEditWinModePause->setText(out.readLine());
    lineEditTime->setText(out.readLine());
    lineEditPause->setText(out.readLine());
    // прерывание
    checkBoxUseInt->setChecked(!out.readLine().isEmpty());
}

void NoiseTest::startTest()
{
    noiseObjToThread* curThread = (noiseObjToThread*)objToThread;

    curThread->trbit = 1 - comboBoxTestType->currentIndex();
    curThread->useInt = checkBoxUseInt->isChecked() && checkBoxEnaInt->isChecked();
    curThread->setOutEnabled(checkBoxOut->isChecked());
    curThread->waitTime = lineEditTime->text().toInt(0, 16);
    curThread->rtaddr = comboBoxRTA->currentIndex();
    curThread->writeCfg = checkBoxConfRegLoad->isChecked();
    curThread->initEnable = checkBoxInit->isChecked();
    curThread->pauseTime = lineEditPause->text().toInt(0, 10);
    curThread->delayTime = lineEditWinModePause->text().toInt(0, 10);

    if (radioButtonEnter->isChecked())
        curThread->iterCycle = spinBoxCycle->value();
    else if (radioButtonUnlimited->isChecked())
        curThread->iterCycle = -1;
    else
        curThread->iterCycle = 1;

    curThread->devBC = deviceList.at(0);
    quint32 cfgBC = curThread->devBC->configuration.getConfigReg();
    REG_HSC_cfg* curreg = &(curThread->devBC->reg_hsc_cfg);
    curreg->setData(cfgBC);
    curreg->rt_bc = CFG_MODE_BC;   // признак КШ
    curreg->type_man = comboBoxManType->currentIndex();
    curreg->en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;
    curreg->ena_codec = checkBoxCodec->isChecked() ? 1 : 0;

    curThread->devRT = deviceList.at(1);
    curreg = &(curThread->devRT->reg_hsc_cfg);
    quint32 cfgRT = curThread->devRT->configuration.getConfigReg();
    curreg->setData(cfgRT);
    curreg->rt_bc = CFG_MODE_RT;     // признак ОУ
    curreg->type_man = comboBoxManType->currentIndex();
//        curreg->en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;   // прерывание ОУ не было включено
    curreg->ena_codec = checkBoxCodec->isChecked() ? 1 : 0;
    curreg->rtavsk_ena = checkBoxEnaAddr->isChecked() ? 1 : 0;
    if (checkBoxEnaAddr->isChecked())
        curreg->rtavsk = curThread->rtaddr;

    curThread->timeMeasure = comboBoxSpeed->currentIndex();
    curThread->timeOverhead = lineEditOver->text().isEmpty() ? 0 : lineEditOver->text().toInt(0, 10);
    curThread->checkLoadCfg = comboBoxWrongCfgReg->currentIndex() > 0;

    // длина пакета
    curThread->num_s = lineEditNumS->text().toInt();
    if (curThread->num_s >= MAXNUMSYM)
        curThread->num_s = MAXNUMSYM-1;

    curThread->checkStatusErrBC = (comboBoxErrStatusBC->currentIndex() > 0);
    curThread->checkStatusErrRT = (comboBoxErrStatusRT->currentIndex() > 0);

    curThread->checkRTA = (comboBoxCheckRTA->currentIndex() > 0);
/*
    QComboBox *comboBoxExitRule;  // условие выхода
    // вывод
    QComboBox *comboBoxShow;
    // регистры
    QCheckBox *checkBoxRTALoad;
    // проверки
    QComboBox *comboBoxErrRegComp;
    QComboBox *comboBoxBCIntErr;
    QComboBox *comboBoxCheckRTA;


    curThread->compEnableData = comboBoxCompData->currentIndex();
    curThread->compEnableReg = comboBoxErrRegComp->currentIndex();
    curThread->noIntFatalBC = (comboBoxBCIntErr->currentIndex() != 0);
    curThread->noIntFatalRT = comboBoxRTIntErr->currentIndex();
    curThread->statusBCOut = checkBoxBCOut->isChecked();
    curThread->statusRTOut = checkBoxRTOut->isChecked();
    curThread->perOutBC = lineEditBCOut->text().isEmpty() ? 0 : lineEditBCOut->text().toInt(0, 10);
    curThread->perOutRT = lineEditRTOut->text().isEmpty() ? 0 : lineEditRTOut->text().toInt(0, 10);
    curThread->modeOutBC = comboBoxBCOutPref->currentIndex();
    curThread->modeOutRT = comboBoxRTOutPref->currentIndex();

    curThread->manipulation = comboBoxManType->currentText();
    curThread->codec = checkBoxCodec->isChecked();

    curThread->nwrd = test.numWordInSymbol();
*/

    curThread->dataSize = dataGen.getDataLen();
    curThread->testData = dataGen.createData(curThread->dataSize * 2);

    emit startTestTh();
}

void NoiseTest::disableUnused()
{
    // ПОКА НЕ РЕАЛИЗОВАНЫ
    checkBoxParamView->setDisabled(true);
    labelParamView->setDisabled(true);
    checkBoxUseInt->setDisabled(true);
    labelUseInt->setDisabled(true);
}








void NoiseTest::setEnabledSpecial(bool b) {}

void noiseObjToThread::perform() {}

void noiseObjToThread::terminate(int )
{
    if (!isRunning())  // (fl == AbstractTest::ErrorIsOccured || fl == AbstractTest::Completed || fl == AbstractTest::Stopped)
    {
        if (tcpSocketBC.state() == QAbstractSocket::ConnectedState)
            tcpSocketBC.abort();
        if (tcpSocketRT.state() == QAbstractSocket::ConnectedState)
            tcpSocketRT.abort();
    }
}
