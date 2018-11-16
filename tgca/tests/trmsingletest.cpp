#include "trmsingletest.h"
//#include "../registers.h"
#include "../ctestbc.h"
#include "../gendata.h"


void TrmSingleTest::setStatSettings()
{
    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("totalErr", stats->findChild<QLabel*>("totalErr"));
       statsMap.insert("errReceive", stats->findChild<QLabel*>("errReceive"));  ///////////
    statsMap.insert("errNoSW", stats->findChild<QLabel*>("errNoSW"));
    statsMap.insert("errYesSW", stats->findChild<QLabel*>("errYesSW"));
    statsMap.insert("errNoFin", stats->findChild<QLabel*>("errNoFin"));
    statsMap.insert("errKodec", stats->findChild<QLabel*>("errKodec"));
    statsMap.insert("errCompare", stats->findChild<QLabel*>("errCompare"));
    statsMap.insert("errNoIntBC", stats->findChild<QLabel*>("errNoIntBC"));
       statsMap.insert("errNoIntRT", stats->findChild<QLabel*>("errNoIntRT"));   ////////
       statsMap.insert("errStatusRT", stats->findChild<QLabel*>("errStatusRT"));           ///////////////
       statsMap.insert("errBefore", stats->findChild<QLabel*>("errBefore"));
       statsMap.insert("errFatal", stats->findChild<QLabel*>("errFatal"));
       statsMap.insert("totalReset", stats->findChild<QLabel*>("totalReset"));

    connect(stats->findChild<QPushButton*>("save"),SIGNAL(clicked(bool)),this,SLOT(statsSave()));
    connect(stats->findChild<QPushButton*>("toZero"),SIGNAL(clicked(bool)),this,SLOT(statsToZero()));
    connect(objToThread,SIGNAL(statsOutputReady(QString,long)), this, SLOT(statsTestOutput(QString,long)));
}

void TrmSingleTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    disconnect(settings,SIGNAL(finished(int)),this,SLOT(checkDeviceAvailability(int)));
    connect(settings,SIGNAL(finished(int)),this,SLOT(updateSettings()));
    connect(this,SIGNAL(settingsClosed(int)),this,SLOT(checkDeviceAvailability(int)));

    lineEditLen = settings->findChild<QLineEdit*>("lineEditLen");
    lineEditBegin = settings->findChild<QLineEdit*>("lineEditBegin");
    lineEditStep = settings->findChild<QLineEdit*>("lineEditStep");
    lineEditNumStep = settings->findChild<QLineEdit*>("lineEditNumStep");
    lineEditTime = settings->findChild<QLineEdit*>("lineEditTime");
    lineEditPause = settings->findChild<QLineEdit*>("lineEditPause");

    comboBoxRTA = settings->findChild<QComboBox*>("comboBoxRTA");
    comboBoxCode = settings->findChild<QComboBox*>("comboBoxCode");
    comboBoxManType = settings->findChild<QComboBox*>("comboBoxManType");
    comboBoxAmplBC = settings->findChild<QComboBox*>("comboBoxAmplBC");
    comboBoxAmplRT = settings->findChild<QComboBox*>("comboBoxAmplRT");
    comboBoxUnit = settings->findChild<QComboBox*>("comboBoxUnit");

    checkBoxBroad = settings->findChild<QCheckBox*>("checkBoxBroad");
    checkBoxCodec = settings->findChild<QCheckBox*>("checkBoxCodec");
    checkBoxEnaInt = settings->findChild<QCheckBox*>("checkBoxEnaInt");
    checkBoxEnaAddr = settings->findChild<QCheckBox*>("checkBoxEnaAddr");
    checkBoxUseInt = settings->findChild<QCheckBox*>("checkBoxUseInt");
    checkBoxOut = settings->findChild<QCheckBox*>("checkBoxOut");
    checkBoxDevBC = settings->findChild<QCheckBox*>("checkBoxDevBC");
    checkBoxDevRT = settings->findChild<QCheckBox*>("checkBoxDevRT");

    radioButtonLin = settings->findChild<QRadioButton*>("radioButtonLin");
    radioButtonSingle = settings->findChild<QRadioButton*>("radioButtonSingle");
    radioButtonUnlimited = settings->findChild<QRadioButton*>("radioButtonUnlimited");
    radioButtonEnter = settings->findChild<QRadioButton*>("radioButtonEnter");

    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");

    labelLen = settings->findChild<QLabel*>("labelLen");
    labelBegin = settings->findChild<QLabel*>("labelBegin");
    labelStep = settings->findChild<QLabel*>("labelStep");
    labelNumStep = settings->findChild<QLabel*>("labelNumStep");
    radioButtonRand = settings->findChild<QRadioButton*>("radioButtonRand");

    spinBoxCycle = settings->findChild<QSpinBox*>("spinBoxCycle");
    checkBoxInit = settings->findChild<QCheckBox*>("checkBoxInit");

    lineEditSPIData = settings->findChild<QLineEdit*>("lineEditSPIData");
    checkBoxConfRegLoad = settings->findChild<QCheckBox*>("checkBoxConfRegLoad");
    checkBoxRTALoad = settings->findChild<QCheckBox*>("checkBoxRTALoad");
    comboBoxSPILoad = settings->findChild<QComboBox*>("comboBoxSPILoad");
    checkBoxCounters = settings->findChild<QCheckBox*>("checkBoxCounters");
    comboBoxSPIdr = settings->findChild<QComboBox*>("comboBoxSPIdr");
    comboBoxSPIen = settings->findChild<QComboBox*>("comboBoxSPIen");

    checkBoxBCOut = settings->findChild<QCheckBox*>("checkBoxBCOut");
    checkBoxRTOut = settings->findChild<QCheckBox*>("checkBoxRTOut");
//    labelBCStatOut = settings->findChild<QLabel*>("labelBCStatOut");
//    labelRTStatOut = settings->findChild<QLabel*>("labelRTStatOut");
//    labelCounters = settings->findChild<QLabel*>("labelCounters");

//    checkBoxCounters->setTristate(true);
    comboBoxCountPref = settings->findChild<QComboBox*>("comboBoxCountPref");
    comboBoxBCOutPref = settings->findChild<QComboBox*>("comboBoxBCOutPref");
    comboBoxRTOutPref = settings->findChild<QComboBox*>("comboBoxRTOutPref");
    lineEditCountOut = settings->findChild<QLineEdit*>("lineEditCountOut");
    lineEditBCOut = settings->findChild<QLineEdit*>("lineEditBCOut");
    lineEditRTOut = settings->findChild<QLineEdit*>("lineEditRTOut");

    checkBoxWinMode = settings->findChild<QCheckBox*>("checkBoxWinMode");
    lineEditWinModePause = settings->findChild<QLineEdit*>("lineEditWinModePause");

    comboBoxErrStatusBC = settings->findChild<QComboBox*>("comboBoxErrStatusBC");
    comboBoxErrStatusRT = settings->findChild<QComboBox*>("comboBoxErrStatusRT");
    comboBoxCheckRW = settings->findChild<QComboBox*>("comboBoxCheckRW");
    comboBoxRec = settings->findChild<QComboBox*>("comboBoxRec");
    comboBoxBCIntErr = settings->findChild<QComboBox*>("comboBoxBCIntErr");
    comboBoxRTIntErr = settings->findChild<QComboBox*>("comboBoxRTIntErr");
    comboBoxMemBC = settings->findChild<QComboBox*>("comboBoxMemBC");
    comboBoxMemRT = settings->findChild<QComboBox*>("comboBoxMemRT");
    comboBoxErrRegComp = settings->findChild<QComboBox*>("comboBoxErrRegComp");
    comboBoxWrongCfgReg = settings->findChild<QComboBox*>("comboBoxWrongCfgReg");

    comboBoxTestType = settings->findChild<QComboBox*>("comboBoxTestType");

    labelSPIData = settings->findChild<QLabel*>("labelSPIData");
    labelSPIdr = settings->findChild<QLabel*>("labelSPIdr");
    labelSPIen = settings->findChild<QLabel*>("labelSPIen");

    labelBroad = settings->findChild<QLabel*>("labelBroad");

    labelCntOut = settings->findChild<QLabel*>("labelCntOut");
    labelCounterSuf = settings->findChild<QLabel*>("labelCounterSuf");
    labelBCStatOut = settings->findChild<QLabel*>("labelBCStatOut");
    labelBCOutSuf = settings->findChild<QLabel*>("labelBCOutSuf");
    labelRTStatOut = settings->findChild<QLabel*>("labelRTStatOut");
    labelRTOutSuf = settings->findChild<QLabel*>("labelRTOutSuf");



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
    lineEditPause->setText(out.readLine());
    checkBoxUseInt->setChecked(!out.readLine().isEmpty());
    checkBoxOut->setChecked(!out.readLine().isEmpty());

    recalcMax();
    onRadioData();
    radioButtonRand->setChecked(!radioButtonLin->isChecked());

    lineEditDevBC->setText(out.readLine());
    checkBoxDevBC->setChecked(!out.readLine().isEmpty());
    lineEditDevRT->setText(out.readLine());
    checkBoxDevRT->setChecked(!out.readLine().isEmpty());

    out.readLine();      ///////checkBoxCompare->setChecked(!out.readLine().isEmpty());
    checkBoxInit->setChecked(!out.readLine().isEmpty());
    spinBoxCycle->setValue(out.readLine().toInt());
    int cycleMode = out.readLine().toInt();
    if (cycleMode == 0)
        radioButtonEnter->setChecked(true);
    else if (cycleMode == 1)
        radioButtonSingle->setChecked(true);
    else
        radioButtonUnlimited->setChecked(true);
    spinBoxCycle->setEnabled(radioButtonEnter->isChecked());

    lineEditSPIData->setText(out.readLine());
    checkBoxConfRegLoad->setChecked(!out.readLine().isEmpty());
    checkBoxRTALoad->setChecked(!out.readLine().isEmpty());
    comboBoxSPILoad->setCurrentText(out.readLine());
    checkBoxCounters->setChecked(!out.readLine().isEmpty());
    comboBoxSPIdr->setCurrentIndex(out.readLine().toInt());
    comboBoxSPIen->setCurrentIndex(out.readLine().toInt());

    checkBoxBCOut->setChecked(!out.readLine().isEmpty());
    checkBoxRTOut->setChecked(!out.readLine().isEmpty());
    checkBoxWinMode->setChecked(!out.readLine().isEmpty());
    comboBoxCountPref->setCurrentText(out.readLine());
    comboBoxBCOutPref->setCurrentText(out.readLine());
    comboBoxRTOutPref->setCurrentText(out.readLine());
    lineEditCountOut->setText(out.readLine());
    lineEditBCOut->setText(out.readLine());
    lineEditRTOut->setText(out.readLine());
//    lineEditWinModePause->setText(out.readLine());
    lineEditWinModePause->setText("500");

    settingsFile.close();

    trmode = comboBoxTestType->currentIndex() ? tgca_tr_TRM : tgca_tr_REC;

    if (checkBoxDevBC->isChecked())
        deviceLineEditList.append(lineEditDevBC);
    if (checkBoxDevRT->isChecked())
        deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability((checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0));

    connect(comboBoxManType, SIGNAL(activated(int)), this, SLOT(recalcMax()));
    connect(checkBoxCodec, SIGNAL(clicked(bool)), this, SLOT(recalcMax()));
    connect(radioButtonLin, SIGNAL(toggled(bool)), this, SLOT(onRadioData()));
    connect(radioButtonSingle, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(radioButtonUnlimited, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(radioButtonEnter, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(checkBoxInit, SIGNAL(clicked()), this, SLOT(onCheckInit()));

    objToThread = new trmSingleObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread, SIGNAL(resultReady(int)), objToThread, SLOT(terminate(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));

    setStatSettings();

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
    in << ((str.isEmpty() || str.toInt(0,16) == 0) ? "" : str) << endl;
    in << comboBoxUnit->currentText() << endl;

    in << lineEditTime->text() << endl;
    in << lineEditPause->text() << endl;
    in << (checkBoxUseInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxOut->isChecked() ? "1" : "") << endl;

    in << lineEditDevBC->text() << endl;
    in << (checkBoxDevBC->isChecked() ? "1" : "") << endl;
    in << lineEditDevRT->text() << endl;
    in << (checkBoxDevRT->isChecked() ? "1" : "") << endl;

    in << "0";     /////(checkBoxCompare->isChecked() ? "1" : "") << endl;
    in << (checkBoxInit->isChecked() ? "1" : "") << endl;
    in << spinBoxCycle->text() << endl;
    int cycleMode = radioButtonEnter->isChecked() ? 0 : (radioButtonSingle->isChecked() ? 1 : 2);
    in << cycleMode << endl;

    in << lineEditSPIData->text() << endl;
    in << (checkBoxConfRegLoad->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTALoad->isChecked() ? "1" : "") << endl;
    in << comboBoxSPILoad->currentText() << endl;
    in << (checkBoxCounters->isChecked() ? "1" : "") << endl;
    in << comboBoxSPIdr->currentIndex() << endl;
    in << comboBoxSPIen->currentIndex() << endl;

    in << (checkBoxBCOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxWinMode->isChecked() ? "1" : "") << endl;
    in << comboBoxCountPref->currentText() << endl;
    in << comboBoxBCOutPref->currentText() << endl;
    in << comboBoxRTOutPref->currentText() << endl;
    in << lineEditCountOut->text() << endl;
    in << lineEditBCOut->text() << endl;
    in << lineEditRTOut->text() << endl;

    settingsFile.close();
}

void TrmSingleTest::onRadioData()
{
    labelBegin->setText(radioButtonLin->isChecked() ? tr("Начальное значение (hex)") : tr("Зерно инициализации"));
    labelStep->setText(radioButtonLin->isChecked() ? tr("Приращение (со знаком)") : tr("Минимум (hex)"));
    labelNumStep->setText(radioButtonLin->isChecked() ? tr("Число шагов до сброса") : tr("Максимум (hex)"));
}

void TrmSingleTest::onRadioCycle()
{
    spinBoxCycle->setEnabled(radioButtonEnter->isChecked());
}

unsigned int TrmSingleTest::maxNumByte()
{
    return calcNumWordInSymbol(comboBoxManType->currentIndex(), checkBoxCodec->isChecked())*MAXNUMSYMBINPACK*sizeof(word32_t) - sizeof(word32_t);
}

void TrmSingleTest::recalcMax()
{
    mnb = maxNumByte();
    labelLen->setText(QString(tr("Длина сообщения в байтах (0-%1)").arg(mnb)));
    if (lineEditLen->text() != "max" && lineEditLen->text().toUInt() > mnb)
        lineEditLen->setText("max");
}

void TrmSingleTest::updateDeviceList()
{
    deviceLineEditList.clear();
    if (checkBoxDevBC->isChecked())
        deviceLineEditList.append(lineEditDevBC);
    if (checkBoxDevRT->isChecked())
        deviceLineEditList.append(lineEditDevRT);
    //qDebug() << deviceLineEditList.size() << " devices";
}

void TrmSingleTest::updateSettings()
{
    // Проверка устройств. Можно проверять только с признаком необходимости.
    updateDeviceList();
    int done = (checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0);
    emit settingsClosed(done);
    // Здесь можно проверить и другие настройки.
}

void TrmSingleTest::onCheckInit()
{
    spinBoxCycle->setMinimum(checkBoxInit->isChecked() ? 0 : 1);
}

void TrmSingleTest::startTest()
{
    trmSingleObjToThread* curThread = (trmSingleObjToThread*)objToThread;

    curThread->trmode = trmode;
    curThread->broad = trmode == tgca_tr_REC ? checkBoxBroad->isChecked() : false;
    curThread->useInt = checkBoxUseInt->isChecked() && checkBoxEnaInt->isChecked();
    curThread->outEnable = checkBoxOut->isChecked();
    curThread->waitTime = lineEditTime->text().toInt(0, 16);
    curThread->rtaddr = comboBoxRTA->currentIndex();
    curThread->checkCountersEnable = checkBoxCounters->isChecked();
    curThread->writeCfg = checkBoxConfRegLoad->isChecked();
    //curThread->modeSPI = val_REG_CR_SPI_dr_UNDEF;
    curThread->loadSPI = comboBoxSPILoad->currentText();
    curThread->data_size = 0;
    curThread->trmData = 0;
    curThread->testData = 0;
    curThread->windowMode = checkBoxWinMode->isChecked();
    curThread->compEnableMemBC = comboBoxMemBC->currentIndex();
    curThread->compEnableMemRT = comboBoxMemRT->currentIndex();
    curThread->compEnableReg = comboBoxErrRegComp->currentIndex();
    if (radioButtonEnter->isChecked())
        curThread->iterCycle = spinBoxCycle->value();
    else if (radioButtonUnlimited->isChecked())
        curThread->iterCycle = -1;
    else
        curThread->iterCycle = 1;
    curThread->initEnable = checkBoxInit->isChecked();
    curThread->pauseTime = lineEditPause->text().toInt(0, 10);
    curThread->delayTime = lineEditWinModePause->text().toInt(0, 10);


    ushort rta = curThread->broad ? BRD_RT_ADDR : curThread->rtaddr;

    int index = 0;
    if (checkBoxDevBC->isChecked())
    {
        curThread->devBC = deviceList.at(0);
        index ++;
        quint32 cfgBC = curThread->devBC->configuration.getConfigReg();
        curThread->devBC->reg_hsc_cfg.setData(cfgBC);
        curThread->devBC->reg_hsc_cfg.rt_bc = val_REG_CFG_mode_BC;   // признак КШ
        curThread->devBC->reg_hsc_cfg.type_man = comboBoxManType->currentIndex();
        curThread->devBC->reg_hsc_cfg.en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;
        curThread->devBC->reg_hsc_cfg.ena_codec = checkBoxCodec->isChecked() ? 1 : 0;

        if (comboBoxSPILoad->currentText() == "КШ")
        {
            curThread->dataSPI = lineEditSPIData->text().toInt(0, 16);
            curThread->devBC->reg_hsc_cr_spi.dr8_16_32 = comboBoxSPIdr->currentIndex();
            curThread->devBC->reg_hsc_cr_spi.spi_en = comboBoxSPIen->currentIndex();
        }

        curThread->amplBC= comboBoxAmplBC->currentIndex();

        if (curThread->windowMode)
            curThread->devBC->reg_aux_winmode.mode = 1;
    }
    else
        curThread->devBC = 0;


    if (checkBoxDevRT->isChecked())
    {
        curThread->devRT = deviceList.at(index);

        quint32 cfgRT = curThread->devRT->configuration.getConfigReg();
        curThread->devRT->reg_hsc_cfg.setData(cfgRT);
        curThread->devBC->reg_hsc_cfg.rt_bc = val_REG_CFG_mode_RT;     // признак ОУ
        curThread->devBC->reg_hsc_cfg.type_man = comboBoxManType->currentIndex();
//        curThread->devBC->reg_hsc_cfg.en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;   // прерывание ОУ не было включено
        curThread->devBC->reg_hsc_cfg.ena_codec = checkBoxCodec->isChecked() ? 1 : 0;
        curThread->devBC->reg_hsc_cfg.rtavsk_ena = checkBoxEnaAddr->isChecked() ? 1 : 0;
        if (checkBoxEnaAddr->isChecked())
        {
            curThread->devBC->reg_hsc_cfg.rtavsk = curThread->rtaddr;
            curThread->rtaddr = MAX_RT_ADDR + 1;
        }
        curThread->amplRT = comboBoxAmplRT->currentIndex();
        if (comboBoxSPILoad->currentText() == "ОУ")
        {
            curThread->dataSPI = lineEditSPIData->text().toInt(0, 16);
            curThread->devRT->reg_hsc_cr_spi.dr8_16_32 = comboBoxSPIdr->currentIndex();
            curThread->devRT->reg_hsc_cr_spi.spi_en = comboBoxSPIen->currentIndex();
        }
        if (curThread->windowMode)
            curThread->devRT->reg_aux_winmode.mode = 1;
    }
    else
        curThread->devRT = 0;

    if (!checkBoxRTALoad->isChecked())
        curThread->rtaddr = MAX_RT_ADDR + 1;

    uint num_b = mnb;
    if (lineEditLen->text() != "max")
        num_b = lineEditLen->text().toInt(0, 10); // или 16
    if (num_b > mnb)
    {
        num_b = mnb;
        lineEditLen->setText("max");
    }
    CTestBC test;
    test.setConfigFlds(curThread->devBC->reg_hsc_cfg.type_man, curThread->devBC->reg_hsc_cfg.ena_codec);
    if (test.maxNumByte() != mnb)
        qDebug() << "Error max num byte: " << mnb << " " << test.maxNumByte();

    void* rawData;
    if (radioButtonLin->isChecked())
        rawData = createRegularData(num_b, lineEditBegin->text().toInt(0, 16), lineEditStep->text().toInt(0, 10),
                                    lineEditNumStep->text().isEmpty() ? 0 : lineEditNumStep->text().toInt(0, 10),
                                    comboBoxUnit->currentText().toInt(0,10));
    else
        rawData = createRandomData(num_b, lineEditBegin->text().toInt(0, 16), lineEditStep->text().toInt(0, 16),
                                    lineEditNumStep->text().toInt(0, 16), comboBoxUnit->currentText().toInt(0,10));


    int ns = trmode == tgca_tr_REC ? test.NumSymOFDM(num_b) + 1 : 1;
    int sz_dst = ns * NUMWORDINOFDMSYM * sizeof(word32_t);
    qDebug() << "sizes: " << num_b << " " << ns << " " << sz_dst;
    curThread->trmData = malloc(sz_dst);
    if (curThread->trmData == 0)
    {
        message(tr("not enough memory"));
        destroyData(rawData);
        return;
    }
    memset(curThread->trmData, 0, sz_dst);
    if (!test.createCommandPack(curThread->trmData, sz_dst, rawData, num_b, rta, trmode, comboBoxCode->currentIndex()))
    {
        message(tr("Error in createCommandPack() !!!"));
        destroyData(rawData);
        free(curThread->trmData);
        curThread->trmData = 0;
        return;
    }
    curThread->data_size = sz_dst;

    if (trmode != tgca_tr_REC)
    {
        curThread->testData = malloc(MAXPACKAGESIZE);
        if (curThread->testData == 0)
        {
            message(tr("not enough memory"));
            destroyData(rawData);
            free(curThread->trmData);
            curThread->trmData = 0;
            return;
        }
        memset(curThread->testData, 0, MAXPACKAGESIZE);
        if (!test.createTestPack(curThread->testData, MAXPACKAGESIZE, rawData, num_b))
        {
            message(tr("Error in createTestPack() !!!"));
            destroyData(rawData);
            free(curThread->trmData);
            curThread->trmData = 0;
            free(curThread->testData);
            curThread->testData = 0;
            return;
        }
    }

    destroyData(rawData);

    emit startTestTh();
}

int readRegVal(Device* dev, quint32 addr)
{
    BaseReg reg(addr);
    dev->readReg(&reg);
    return reg.getData();
}

void setRegWritten(Device *dev, BaseReg &reg)
{
    dev->configuration.setWritten(reg.address, reg.getData());
}

void writeSPI(Device* dev, quint32 dataSPI)
{
    QVector<BaseReg*> regs;
    regs.append(&dev->reg_hsc_dr_spi_lsw);
    if (dev->reg_hsc_cr_spi.dr8_16_32 == val_REG_CR_SPI_dr_DEP32)
    {
        regs.append(&dev->reg_hsc_dr_spi_msw);
        dev->reg_hsc_dr_spi_msw.data = dataSPI >> 16;
    }
    regs.append(&dev->reg_hsc_cr_spi);
    dev->reg_hsc_dr_spi_lsw.data = dataSPI & 0xFFFF;

    dev->writeRegs(regs);

    setRegWritten(dev, dev->reg_hsc_cr_spi);
    setRegWritten(dev, dev->reg_hsc_dr_spi_lsw);
    if (dev->reg_hsc_cr_spi.dr8_16_32 == val_REG_CR_SPI_dr_DEP32)
        setRegWritten(dev, dev->reg_hsc_dr_spi_msw);
}

/*
void trmSingleObjToThread::checkStatusReg(Device *dev, int *status)
{
    int addr_stat = REG_VSK_status;
    QByteArray answer;
    QByteArray readArray = cmdHead(3, 4);
    readArray.append((char*)&addr_stat, 4);
    dev->read_F1(readArray, answer);

    *status = *(int*)(answer.data());
}
*/

bool checkSPI(Device* dev, int time)
{
    int oldval = dev->reg_hsc_cr_spi.getData();
    QTimer timer;
    timer.setInterval(time);
    timer.start();
    do
    {
        dev->readReg(&dev->reg_hsc_cr_spi);
        if (dev->reg_hsc_cr_spi.spif == 1)
        {
            dev->reg_hsc_cr_spi.spif = 0;
            return (oldval == dev->reg_hsc_cr_spi.getData());
        }
    }
    while (timer.remainingTime() > 0);
    return false;
}

int trmSingleObjToThread::checkStatusRegBC()
{
    int status = readRegVal(devBC, REG_VSK_status);
    /*
    if (status & FL_REG_STATUS_ERR_flags)
        qDebug() << "BC status  Trm buf = " << ((status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((status)&fl_REG_STATUS_rx_num_buf ? 1 : 0)
                    << "  !!! Error flags in status:  " << QString::number(status, 16);
    else
        qDebug() << "BC status  Trm buf = " << ((status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((status)&fl_REG_STATUS_rx_num_buf ? 1 : 0);
    */
    return status;
}

int trmSingleObjToThread::checkStatusRegRT()
{
    int status = readRegVal(devRT, REG_VSK_status);

    if ((status) & FL_REG_STATUS_ERR_flags)
        qDebug() << "RT status  Trm buf = " << ((status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((status)&fl_REG_STATUS_rx_num_buf ? 1 : 0)
                    << "  !!! Error flags in status:  " << QString("%1").arg(status, 4, 16, QLatin1Char('0'));
    else
        qDebug() << "RT status  Trm buf = " << ((status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((status)&fl_REG_STATUS_rx_num_buf ? 1 : 0);

    return status;
}

void trmSingleObjToThread::checkCounters(Device *dev)
{
    if (checkCountersEnable)
    {
        QVector<BaseReg*> regs;
        regs.append(&dev->reg_hsc_cnt_pct_rx_lsw);
        regs.append(&dev->reg_hsc_cnt_pct_rx_msw);
        regs.append(&dev->reg_hsc_cnt_pct_tx_lsw);
        regs.append(&dev->reg_hsc_cnt_pct_tx_msw);
        dev->readRegs(regs);

        qDebug() << "Counters:  " << dev->reg_hsc_cnt_pct_tx_msw.counter << "  " << dev->reg_hsc_cnt_pct_tx_lsw.counter
                 << "  " << dev->reg_hsc_cnt_pct_rx_msw.counter << "  " << dev->reg_hsc_cnt_pct_rx_lsw.counter;
    }
}

static bool split(const QByteArray all, QByteArray& even, QByteArray& odd)
{
    int sz = all.size();
    if (sz%8 != 0)
        return false;
    for (int i=0; i<sz; i+=8)
    {
        int addr = *(int*)(all.data()+i);
        int val = *(int*)(all.data()+i+4);
        if (addr != REG_VSK_creg)
        {
            even.append((char*)&addr, 4);
            odd.append((char*)&val, 4);
        }
    }
    return (even.size() > 0);
}

trmSingleObjToThread::trmSingleObjToThread():
    absObjToThread(),
    //cfgBC(0), cfgRT(0),
    amplBC(0), amplRT(0),
    devBC(0), devRT(0),
    trmData(0), testData(0)
{
}

void trmSingleObjToThread::stdOutput(QString message_rus, QString message_eng)
{
    if (outEnable)
        emit outputReady(message_rus);
    else
        qDebug() << message_eng;
}

void trmSingleObjToThread::doWork()
{
  try {
    qDebug() << "trmSingleObjToThread::doWork() started";
    int errorsBefore = 0;
    if (iterCycle != 0)
    {
        if (data_size > 0)
            stdOutput(QString(tr("Командное слово: %1")).arg(*(int*)trmData, 8, 16, QLatin1Char('0')),
                      QString(tr("Command word: %1")).arg(*(int*)trmData, 8, 16, QLatin1Char('0')));
        else
            stdOutput(QString(tr("Размер данных для передачи - 0 байт!")), QString(tr("Data size = 0 !")));
    }

    if (devBC)
    {
        QString ip = devBC->connection.getServerIP();
        ushort port = devBC->connection.getServerPORT().toUShort();
        tcpSocketBC.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketBC.waitForConnected(5000))
        {
            stdOutput(tr("Нет соединения с КШ"), tr("BC: no connection"));
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }
        qDebug() << tr("BC connected");
        devBC->setSocket(&tcpSocketBC);

        checkCounters(devBC);
    }
    else
    {
        stdOutput(tr("КШ не используется"), tr("BC not used"));
    }
    if (devRT)
    {
        QString ip = devRT->connection.getServerIP();
        ushort port = devRT->connection.getServerPORT().toUShort();
        tcpSocketRT.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketRT.waitForConnected(5000)) {
            stdOutput(tr("Нет соединения с ОУ"), tr("RT: no connection"));
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }
        qDebug() << tr("RT connected");
        devRT->setSocket(&tcpSocketRT);

        checkCounters(devRT);
    }
    else
    {
        stdOutput(tr("ОУ не используется"), tr("RT not used"));
    }

    //int addr_cfg  = REG_VSK_cfg;
    //int addr_ampl = REG_VSK_amplification_factor;
    //int addr_rta  = REG_AUX_T_rtaddr;          // вспомогательный регистр: адрес ОУ
    //int addr_int  = REG_AUX_T_interruption;  // вспомогательный регистр: прерывание
    //int addr_win  = REG_AUX_T_win_mode;      // вспомогательный регистр: оконный режим
    //int addr_stat = REG_VSK_status;
    //int addr_creg = REG_VSK_creg;
    //int addr_spi  = REG_VSK_cr_spi;
    //int addr_spi_data_low  = REG_VSK_dr_spi_lsw;

    emit resultReady((int)AbstractTest::Running);
    if (pause_stop() == -1)
        return;

    if (devRT)
    {
        if (initEnable)
        {
            qDebug() << "start RT configuration";
            if (writeCfg)
                devRT->configuration.setChecked(config_NUMREG_cfg, false);
            QByteArray regRT = devRT->configuration.getRegistersToWrite();
            if (regRT.size() > 0)
            {
                devRT->write_F1(regRT.data(), regRT.size());
                devRT->configuration.doneWriteReg(regRT);
            }
            ///   Сравнение конфигурации
            if (compEnableReg)
            {
                QByteArray addrArray, valArray;
                if (split(regRT, addrArray, valArray))
                {
                    // Чтение регистров КШ для сравнения
                    QByteArray readArrayC;
                    readArrayC.resize(addrArray.size());
                    devRT->read_F1(addrArray.data(), readArrayC.data(), addrArray.size());

                    if (readArrayC == valArray)
                    {
                        stdOutput(tr("Сравнение регистров ОУ успешно"), tr("Comparison RT registers OK"));
                    }
                    else
                    {
                        stdOutput(tr("Ошибка конфигурации ОУ: регистры не совпали"), tr("Comparison RT registers wrong"));
                        stdOutput(tr("Возвращенный размер = %1").arg(readArrayC.size()), tr("Returned size = %1").arg(readArrayC.size()));
                        stdOutput(tr("Возвращенные данные = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')),
                                  tr("Returned data = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')));
                        emit statsOutputReady("errCompare", 1);
                        errorsBefore++;
                        if (compEnableReg == 2)
                        {
                            setErrorsBeforeCycle(errorsBefore);
                            return;
                        }
                    }
                }
            }
            checkCounters(devRT);
        }

        if (writeCfg)
        {
            devRT->writeReg(&devRT->reg_hsc_cfg);
            setRegWritten(devRT, devRT->reg_hsc_cfg);

            if (compEnableReg > 0 && readRegVal(devRT, REG_VSK_cfg) != devRT->reg_hsc_cfg.getData())
            {
                stdOutput(tr("Ошибка сравнения конфигурационного регистра ОУ"), tr("Comparison RT cfg register wrong"));
                emit statsOutputReady("errCompare", 1);
                setErrorsBeforeCycle(++errorsBefore);
                return;
            }
        }

        if (amplRT)
        {
            int ampl = (amplRT == 1) ? 0 : amplRT + 14;
            devRT->reg_hsc_amplificatoin_factor.rx_oe_ampl = (amplRT == 1) ? 0 : 1;
            devRT->reg_hsc_amplificatoin_factor.rx_ampl = ampl & FL_REG_AMPL_FACTOR_rx_ampl;
            devRT->writeReg(&devRT->reg_hsc_amplificatoin_factor);
            setRegWritten(devRT, devRT->reg_hsc_amplificatoin_factor);
            if (compEnableReg > 0 && readRegVal(devRT, REG_VSK_amplification_factor) != ampl)
            {
                stdOutput(tr("Ошибка сравнения регистра коэффициента усиления ОУ"), tr("Comparison RT amplification factor wrong"));
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                if (compEnableReg == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }
        if (loadSPI == "ОУ")
        {
            writeSPI(devRT, dataSPI);

            if (compEnableReg > 0 && !checkSPI(devRT, waitTime))
            {
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                stdOutput(tr("Ошибка проверки регистра управления обменом по SPI ОУ"), tr("Comparison RT SPI register wrong"));
                if (compEnableReg == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }

        if (rtaddr <= MAX_RT_ADDR)
        {
            quint32 addr = rtaddr;
            devRT->reg_aux_rtaddr.setData(addr);
            devRT->writeReg(&devRT->reg_aux_rtaddr);
            if (compEnableReg > 0 && readRegVal(devRT, REG_AUX_T_rtaddr) != rtaddr)
            {
                stdOutput(tr("Ошибка сравнения регистра адреса ОУ"), tr("Comparison RTA register wrong"));
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                if (compEnableReg == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }
    }
    if (devBC)
    {
        if (initEnable)
        {
            qDebug() << "start BC configuration";
            if (writeCfg)
                devBC->configuration.setChecked(config_NUMREG_cfg, false);
            QByteArray regBC = devBC->configuration.getRegistersToWrite();
            if (regBC.size() > 0)
            {
                QByteArray regs;
                regs.append(regBC);
                devBC->write_F1(regs.data(), regs.size());
                devBC->configuration.doneWriteReg(regBC);
            }
            if (compEnableReg)
            {
                QByteArray addrArray, valArray;
                if (split(regBC, addrArray, valArray))
                {
                    // Чтение регистров КШ для сравнения
                    QByteArray readArrayC;
                    readArrayC.resize(addrArray.size());
                    devBC->read_F1(addrArray.data(), readArrayC.data(), addrArray.size());

                    if (readArrayC == valArray)
                    {
                        stdOutput(tr("Сравнение регистров КШ успешно"), tr("Comparison BC registers OK"));
                    }
                    else
                    {
                        stdOutput(tr("Ошибка конфигурации КШ: регистры не совпали"), tr("Comparison BC registers wrong"));
                        stdOutput(tr("Возвращенный размер = %1").arg(readArrayC.size()), tr("Returned size = %1").arg(readArrayC.size()));
                        stdOutput(tr("Возвращенные данные = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')),
                                  tr("Returned data = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')));
                        errorsBefore++;
                        emit statsOutputReady("errCompare", 1);
                        if (compEnableReg == 2)
                        {
                            setErrorsBeforeCycle(errorsBefore);
                            return;
                        }
                    }
                }
            }
            checkCounters(devBC);
        }

        if (writeCfg)
        {
            devBC->writeReg(&devBC->reg_hsc_cfg);
            setRegWritten(devBC, devBC->reg_hsc_cfg);
        }
        if (compEnableReg > 0 && readRegVal(devBC, REG_VSK_cfg) != devBC->reg_hsc_cfg.getData())
        {
            stdOutput(tr("Ошибка сравнения конфигурационного регистра КШ"), tr("Comparison BC cfg register wrong"));
            emit statsOutputReady("errCompare", 1);
            setErrorsBeforeCycle(errorsBefore++);
            return;
        }

        if (amplBC)
        {
            int ampl = (amplBC == 1) ? 0 : amplBC + 14;
            devBC->reg_hsc_amplificatoin_factor.rx_oe_ampl = (amplBC == 1) ? 0 : 1;
            devBC->reg_hsc_amplificatoin_factor.rx_ampl = ampl & FL_REG_AMPL_FACTOR_rx_ampl;
            devBC->writeReg(&devBC->reg_hsc_amplificatoin_factor);
            setRegWritten(devBC, devBC->reg_hsc_amplificatoin_factor);
            if (compEnableReg > 0 && readRegVal(devBC, REG_VSK_amplification_factor) != ampl)
            {
                stdOutput(tr("Ошибка сравнения регистра коэффициента усиления КШ"), tr("Comparison BC amplification factor wrong"));
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                if (compEnableReg == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }
        if (loadSPI == "КШ")
        {
            writeSPI(devBC, dataSPI);

            if (compEnableReg > 0 && !checkSPI(devBC, waitTime))
            {
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                stdOutput(tr("Ошибка проверки регистра управления обменом по SPI КШ"), tr("Comparison BC SPI register wrong"));
                if (compEnableReg == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }
    }
    qDebug() << "data_size = " << data_size;
    if (data_size > 0 && devBC != 0 && iterCycle != 0)
    {
        int statusBC, statusRT;
        if (devRT)
        {
            statusRT = checkStatusRegRT();
            if (outEnable)
                emit outputReady(tr("iter = %1   RT status = %2").arg(0).arg(statusRT, 4, 16, QLatin1Char('0')));
        }
/*
        QByteArray pack0, pack1;
        if (devBC)
        {
            // формирование массивов для записи в буферы передатчика КШ
            quint32 bufTrm = getBufTrm(0);
            int bufData = trmData;

            pack0.append((char*)&bufTrm, 4);
            pack0.append((char*)&bufData, 4);
            pack0.append((char*)&data_size, 4);

            pack1.append((char*)&bufTrm, 4);
            pack1.append((char*)&bufData, 4);
            pack1.append((char*)&data_size, 4);
        }
        free(trmData);
        trmData = 0;
        */

        QByteArray compArrayW;
        if (devBC)
        {
            statusBC = checkStatusRegBC();
            // формирование массива для сравнения
            if (compEnableMemBC > 0 || compEnableMemRT > 0)
                compArrayW.append((char*)trmData, data_size);
        }
        if (errorsBefore > 0)
        {
            emit statsOutputReady("errBefore", errorsBefore);
            emit statsOutputReady("totalErr", errorsBefore);
        }

        if (devBC)
        {
            quint32 zero = 0;
            devBC->reg_hsc_creg.setData(zero);
            devBC->reg_hsc_creg.start_bc = 1;

            for (uint it = 0; it!=iterCycle; it++)
            {
                bool errorOccured = false;
                if (pause_stop() == -1)
                    return;

                // Запись данных в буфер передатчика
//                if (outEnable)
//                    emit outputReady(tr("Iter=%1 trm buf=%2").arg(it).arg((statusBC & fl_REG_STATUS_tx_num_buf)==0 ? 0 : 1));

                devBC->write_F2(getBufTrm(statusBC & fl_REG_STATUS_tx_num_buf), (char*)trmData, data_size);

                if (compEnableMemBC > 0 && it <= 1)
                {
                // Чтение данных из буфера передатчика КШ для сравнения
                    QByteArray readArray1;
                    readArray1.resize(data_size);
                    devBC->read_F2(getBufTrm(statusBC & fl_REG_STATUS_tx_num_buf), data_size, readArray1.data());

                    if (readArray1 == compArrayW)
                    {
                        stdOutput(tr("Сравнение буфера передачи КШ успешно"), tr("Comparison BC trm buffer OK"));
                    }
                    else
                    {
                        stdOutput(tr("Ошибка сравнения буфера передачи КШ"), tr("Comparison BC trm buffer wrong"));
                        stdOutput(tr("Длина данных = %1 байт").arg(readArray1.size()), tr("Returned size = %1").arg(readArray1.size()));
                        stdOutput(tr("Командное слово = %1").arg(*(int*)(readArray1.data()), 8, 16, QLatin1Char('0')),
                                  tr("Command word = %1").arg(*(int*)(readArray1.data()), 8, 16, QLatin1Char('0')));
                        emit statsOutputReady("errCompare", 1);
                        if (compEnableMemBC == 2)
                        {
                            setErrorsWithinCycle(false);
                            return;
                        }
                        else
                            errorOccured = true;
                    }
                }

                if (devRT!=0)
                    if ((compEnableMemRT>0 && it<=1) || (trmode != tgca_tr_REC && testData != 0))
                    statusRT = checkStatusRegRT();

                // Оконный режим
                if(windowMode && devRT)
                {
                    devRT->writeReg(&devRT->reg_aux_winmode);
                    devBC->writeReg(&devBC->reg_aux_winmode);

                    thread()->usleep(delayTime);
                }

                if (devRT != 0 && trmode != tgca_tr_REC && testData != 0)
                {
                    /// Запись данных в буфер передачи ОУ
                }

                // Старт обмена
                devBC->writeReg(&devBC->reg_hsc_creg);
                emit statsOutputReady("totalIter", 1);

                if (devRT)
                {
                    int answer1 = readRegVal(devRT, REG_VSK_status);
                    if (outEnable)
                        emit outputReady(tr("Итерация = %1   статус ОУ = %2").arg(it).arg(answer1, 4, 16, QLatin1Char('0')));

                    int interruption = 1;
                    {
                        if (useInt)
                        {
                            QTimer timer;
                            timer.setInterval(waitTime);
                            timer.start();
                            do
                            {
                                devBC->readReg(&devBC->reg_aux_interruption);
                                interruption = devBC->reg_aux_interruption.inter;
                            }
                            while (interruption == 0 || timer.remainingTime() > 0);
                            //timer.killTimer();

                            if (interruption == 0)
                                stdOutput(tr("Итерация = %1   нет прерывания КШ").arg(it, 6), tr("Iter = %1   no interruption from BC").arg(it, 6));

                            statusBC = readRegVal(devBC, REG_VSK_status);
                        }
                        else
                        {
                            bool yes_aw_gr_err = false, no_aw_err = false, rs_err = false;
                            QTimer timer;
                            timer.setInterval(waitTime);
                            timer.start();
                            do
                            {
                                statusBC = readRegVal(devBC, REG_VSK_status);
                                if (statusBC & FL_REG_STATUS_ERR_flags)
                                {
                                    if (statusBC & fl_REG_STATUS_rs_err)
                                        rs_err = true;
                                    if (statusBC & fl_REG_STATUS_no_aw_err)
                                        no_aw_err = true;
                                    if (statusBC & fl_REG_STATUS_yes_aw_gr_err)
                                        yes_aw_gr_err = true;
                                }
                            }
                            while (((statusBC & fl_REG_STATUS_rt_bc_int) == 0) && (timer.remainingTime() > 0));
                            //while (((statusBC & FL_REG_STATUS_flags) == 0) && (timer.remainingTime() > 0));
                            //timer.killTimer();
                            bool yes_aw_gr_err_n = statusBC & fl_REG_STATUS_yes_aw_gr_err;
                            bool no_aw_err_n = statusBC & fl_REG_STATUS_no_aw_err;
                            bool rs_err_n = statusBC & fl_REG_STATUS_rs_err;
                            if (yes_aw_gr_err_n != yes_aw_gr_err || no_aw_err_n != no_aw_err || rs_err_n != rs_err)
                            {
                                QString mess = QString("Error flags changed: status=%1  ").arg(statusBC, 4, 16, QLatin1Char('0'));
                                qDebug() << mess << rs_err << "<>" << rs_err_n << " " << no_aw_err << "<>" << no_aw_err_n << " " << yes_aw_gr_err << "<>" << yes_aw_gr_err_n;
                            }
                        }
                    }

                    // Оконный режим
                    if(windowMode)
                    {
                        devRT->writeReg(&devRT->reg_aux_winmode);
                        devBC->writeReg(&devBC->reg_aux_winmode);
                    }
                    if ((statusBC & fl_REG_STATUS_rt_bc_int) == 0)
                    {
                        stdOutput(tr("Нет признака завершения обмена"), tr("No interruption flag in BS status"));
                        stdOutput(tr("Итерация = %1   статус КШ = %2  ФАТАЛЬНАЯ ОШИБКА").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')),
                                  tr("Iter = %1   BC status = %2  FATAL ERROR").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')));
                        emit statsOutputReady("errNoFin", 1);
                        setErrorsWithinCycle(true);
                        return;
                    }
                    else if (statusBC & FL_REG_STATUS_ERR_flags)
                    {
                        stdOutput(QString(tr("Ошибка в статусе КШ: %1").arg(statusBC, 4, 16, QLatin1Char('0'))),
                                  QString(tr("Errors in BC status: %1").arg(statusBC, 4, 16, QLatin1Char('0'))));
                        errorOccured = true;
                        if (statusBC & fl_REG_STATUS_rs_err)
                            emit statsOutputReady("errKodec", 1);
                        if (statusBC & fl_REG_STATUS_no_aw_err)
                            emit statsOutputReady("errNoSW", 1);
                        if (statusBC & fl_REG_STATUS_yes_aw_gr_err)
                            emit statsOutputReady("errYesSW", 1);
                    }
                    if (interruption == 0)
                    {
                        stdOutput(tr("Итерация = %1   статус КШ = %2  ФАТАЛЬНАЯ ОШИБКА").arg(it).arg(statusBC, 4, 16, QLatin1Char('0')),
                                  tr("Iter = %1   BC status = %2  FATAL ERROR").arg(it).arg(statusBC, 4, 16, QLatin1Char('0')));
                        setErrorsWithinCycle(true);
                        emit statsOutputReady("errNoIntBC", 1);
                        return;
                    }
                    checkCounters(devBC);
                    checkCounters(devRT);

                    if (compEnableMemRT > 0 && it <= 1)
                    {
                    // Чтение данных из буфера приёмника ОУ для сравнения
                        QByteArray readArrayC;
                        readArrayC.resize(data_size);
                        devBC->read_F2(getBufRec(statusRT & fl_REG_STATUS_rx_num_buf), data_size, readArrayC.data());
                        if (readArrayC == compArrayW)
                        {
                            stdOutput(tr("Сравнение буфера приёмника ОУ успешно"), tr("Comparison RT rec buffer OK"));
                        }
                        else
                        {
                            stdOutput(tr("Ошибка сравнения буфера приёмника ОУ"), tr("Comparison RT rev buffer wrong"));
                            stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Returned size = %1").arg(readArrayC.size()));
                            stdOutput(tr("Первое прочитанное слово = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')),
                                      tr("First read word = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')));
                            emit statsOutputReady("errCompare", 1);
                            if (compEnableMemRT == 2)
                            {
                                setErrorsWithinCycle(false);
                                return;
                            }
                        }
                    }
                } // devRT
                else
                {
                    statusBC = checkStatusRegBC();
                    if (outEnable)
                        qDebug() << QString(tr("%1").arg(statusBC, 4, 16, QLatin1Char('0')));
                }
                if (errorOccured)
                    emit statsOutputReady("totalErr", 1);

                if (pauseTime > 0)
                    thread()->msleep(pauseTime);
            } // цикл
        } // devBC
    } // data_size > 0 && devBC != 0 && iterCycle != 0
    else
    {
        if (errorsBefore > 0)
        {
            emit statsOutputReady("errBefore", errorsBefore);
            emit statsOutputReady("totalErr", errorsBefore);
        }
    }

    emit resultReady((int)AbstractTest::Completed);
  }
  catch (const QString& exception)
  {
    if (exception == "socket")
        emit resultReady((int)AbstractTest::ErrorIsOccured);
  }
}

void trmSingleObjToThread::terminate(int fl)
{
    qDebug() << "terminates: " << devBC << "  " << devRT;
    if (fl == AbstractTest::ErrorIsOccured || fl == AbstractTest::Completed || fl == AbstractTest::Stopped)
    {
        if (devBC != 0 && tcpSocketBC.state() == QAbstractSocket::ConnectedState)
            tcpSocketBC.abort();
        if (devRT != 0 && tcpSocketRT.state() == QAbstractSocket::ConnectedState)
            tcpSocketRT.abort();
        if (trmData)
        {
            free(trmData);
            trmData = 0;
        }
        if (testData)
        {
            free(testData);
            testData = 0;
        }
    }
}

//lineEditSPIData->setInputMask(QApplication::translate("TestTrmSingle", "HHHH;_", 0));

void trmSingleObjToThread::setErrorsBeforeCycle(int errors)
{
    emit resultReady((int)AbstractTest::ErrorIsOccured);
    emit statsOutputReady("errBefore", errors);
    emit statsOutputReady("totalErr", errors);
}

void trmSingleObjToThread::setErrorsWithinCycle(bool fatal)
{
    emit resultReady((int)AbstractTest::ErrorIsOccured);
    emit statsOutputReady("errFatal", 1);
    if (fatal)
        emit statsOutputReady("errFatal", 1);
    emit statsOutputReady("totalErr", 1);
}
