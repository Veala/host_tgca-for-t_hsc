#include "trmsingletest.h"
//#include "../registers.h"
#include "../ctestbc.h"
#include "../gendata.h"


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
    checkBoxCompare = settings->findChild<QCheckBox*>("checkBoxCompare");
    checkBoxInit = settings->findChild<QCheckBox*>("checkBoxInit");

    lineEditSPIData = settings->findChild<QLineEdit*>("lineEditSPIData");
    checkBoxConfRegLoad = settings->findChild<QCheckBox*>("checkBoxConfRegLoad");
    checkBoxRTALoad = settings->findChild<QCheckBox*>("checkBoxRTALoad");
    checkBoxSPILoad = settings->findChild<QCheckBox*>("checkBoxSPILoad");
    checkBoxCounters = settings->findChild<QCheckBox*>("checkBoxCounters");
    comboBoxSPIdr = settings->findChild<QComboBox*>("comboBoxSPIdr");
    comboBoxSPIen = settings->findChild<QComboBox*>("comboBoxSPIen");

    checkBoxBCOut = settings->findChild<QCheckBox*>("checkBoxBCOut");
    checkBoxRTOut = settings->findChild<QCheckBox*>("checkBoxRTOut");
//    labelBCStat = settings->findChild<QLabel*>("labelBCStat");
//    labelRTStat = settings->findChild<QLabel*>("labelRTStat");
//    labelCounters = settings->findChild<QLabel*>("labelCounters");

//    checkBoxCounters->setTristate(true);
    comboBoxCountPref = settings->findChild<QComboBox*>("comboBoxCountPref");
    comboBoxBCOutPref = settings->findChild<QComboBox*>("comboBoxBCOutPref");
    comboBoxRTOutPref = settings->findChild<QComboBox*>("comboBoxRTOutPref");
    lineEditCountOut = settings->findChild<QLineEdit*>("lineEditCountOut");
    lineEditBCOut = settings->findChild<QLineEdit*>("lineEditBCOut");
    lineEditRTOut = settings->findChild<QLineEdit*>("lineEditRTOut");

    labelMarkFatalErr = settings->findChild<QLabel*>("labelMarkFatalErr");
    labelMarkStatBCErr = settings->findChild<QLabel*>("labelMarkStatBCErr");
    labelMarkStatRTErr = settings->findChild<QLabel*>("labelMarkStatRTErr");
    labelMarkCompErr = settings->findChild<QLabel*>("labelMarkCompErr");
    checkBoxFatalErr = settings->findChild<QCheckBox*>("checkBoxFatalErr");
    checkBoxStatBCErr = settings->findChild<QCheckBox*>("checkBoxStatBCErr");
    checkBoxStatRTErr = settings->findChild<QCheckBox*>("checkBoxStatRTErr");
    checkBoxCompErr = settings->findChild<QCheckBox*>("checkBoxCompErr");
    checkBoxWinMode = settings->findChild<QCheckBox*>("checkBoxWinMode");

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

    checkBoxCompare->setChecked(!out.readLine().isEmpty());
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
    checkBoxSPILoad->setChecked(!out.readLine().isEmpty());
    checkBoxCounters->setChecked(!out.readLine().isEmpty());
    comboBoxSPIdr->setCurrentIndex(out.readLine().toInt());
    comboBoxSPIen->setCurrentIndex(out.readLine().toInt());

    checkBoxBCOut->setChecked(!out.readLine().isEmpty());
    checkBoxRTOut->setChecked(!out.readLine().isEmpty());
    checkBoxFatalErr->setChecked(!out.readLine().isEmpty());
    comboBoxCountPref->setCurrentText(out.readLine());
    comboBoxBCOutPref->setCurrentText(out.readLine());
    comboBoxRTOutPref->setCurrentText(out.readLine());
    lineEditCountOut->setText(out.readLine());
    lineEditBCOut->setText(out.readLine());
    lineEditRTOut->setText(out.readLine());
/*
    QLabel *labelMarkFatalErr;
    QLabel *labelMarkStatBCErr;
    QLabel *labelMarkStatRTErr;
    QLabel *labelMarkCompErr;
    QCheckBox *checkBoxStatBCErr;
    QCheckBox *checkBoxStatRTErr;
    QCheckBox *checkBoxCompErr;
    QCheckBox *checkBoxWinMode;
*/
    labelMarkFatalErr->setText("");
    labelMarkStatBCErr->setText("");
    labelMarkStatRTErr->setText("");
    labelMarkCompErr->setText("");
    checkBoxFatalErr->setTristate(true);
    checkBoxStatBCErr->setTristate(true);
    checkBoxStatRTErr->setTristate(true);
    checkBoxCompErr->setTristate(true);
    checkBoxFatalErr->setCheckable(Qt::PartiallyChecked);
    checkBoxStatBCErr->setCheckable(Qt::PartiallyChecked);
    checkBoxStatRTErr->setCheckable(Qt::PartiallyChecked);
    checkBoxCompErr->setCheckable(Qt::PartiallyChecked);
    checkBoxFatalErr->setDisabled(true);
    checkBoxStatBCErr->setDisabled(true);
    checkBoxStatRTErr->setDisabled(true);
    checkBoxCompErr->setDisabled(true);
    checkBoxWinMode->setChecked(false);

    settingsFile.close();

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

    in << (checkBoxCompare->isChecked() ? "1" : "") << endl;
    in << (checkBoxInit->isChecked() ? "1" : "") << endl;
    in << spinBoxCycle->text() << endl;
    int cycleMode = radioButtonEnter->isChecked() ? 0 : (radioButtonSingle->isChecked() ? 1 : 2);
    in << cycleMode << endl;

    in << lineEditSPIData->text() << endl;
    in << (checkBoxConfRegLoad->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTALoad->isChecked() ? "1" : "") << endl;
    in << (checkBoxSPILoad->isChecked() ? "1" : "") << endl;
    in << (checkBoxCounters->isChecked() ? "1" : "") << endl;
    in << comboBoxSPIdr->currentIndex() << endl;
    in << comboBoxSPIen->currentIndex() << endl;

    in << (checkBoxBCOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxFatalErr->isChecked() ? "1" : "") << endl;
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
    return calcNumWordInSymbol(comboBoxManType->currentIndex(), checkBoxCodec->isChecked())*MAXNUMSYMBINPACK*4 - 4;
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

    curThread->broad = checkBoxBroad->isChecked();
    curThread->useInt = checkBoxUseInt->isChecked();
    curThread->outEnable = checkBoxOut->isChecked();
    curThread->waitTime = lineEditTime->text().toInt(0, 16);
    curThread->rtaddr = comboBoxRTA->currentIndex();
    curThread->checkCountersEnable = checkBoxCounters->isChecked();
    curThread->writeCfg = checkBoxConfRegLoad->isChecked();
    curThread->modeSPI = val_REG_CR_SPI_dr_UNDEF;
    curThread->data_size = 0;
    curThread->data = 0;
    curThread->windowMode = checkBoxWinMode->isChecked();
    curThread->compEnable = checkBoxCompare->isChecked();
    if (radioButtonEnter->isChecked())
        curThread->iterCycle = spinBoxCycle->value();
    else if (radioButtonUnlimited->isChecked())
        curThread->iterCycle = -1;
    else
        curThread->iterCycle = 1;
    curThread->initEnable = checkBoxInit->isChecked();
    curThread->pauseTime = lineEditPause->text().toInt(0, 10);

    ushort rta = curThread->broad ? BRD_RT_ADDR : curThread->rtaddr;

    int index = 0;
    int cfgBC = 0;
    if (checkBoxDevBC->isChecked())
    {
        curThread->devBC = deviceList.at(0);
        index ++;
        cfgBC = curThread->devBC->configuration.getConfigReg();
    }
    else
        curThread->devBC = 0;
    cfgBC = (cfgBC & ~fl_REG_CFG_mode_rt_bc) | val_REG_CFG_mode_BC;  // признак КШ
    chgManType(cfgBC, comboBoxManType->currentIndex());
    cfgBC = (cfgBC & ~fl_REG_CFG_en_rt_bc_int);
    if (checkBoxEnaInt->isChecked())
        cfgBC |= fl_REG_CFG_en_rt_bc_int;  // разрешение выработки прерывания КШ
    cfgBC = (cfgBC & ~fl_REG_CFG_ena_codec);
    if (checkBoxCodec->isChecked())
        cfgBC |= fl_REG_CFG_ena_codec;     // кодек

    curThread->cfgBC = cfgBC;
    curThread->amplBC= comboBoxAmplBC->currentIndex();

    if (checkBoxDevRT->isChecked())
    {
        curThread->devRT = deviceList.at(index);

        int cfgRT = curThread->devRT->configuration.getConfigReg();
        cfgRT = (cfgRT & ~fl_REG_CFG_mode_rt_bc) | val_REG_CFG_mode_RT;  // признак ОУ
        chgManType(cfgRT, comboBoxManType->currentIndex());
        cfgRT = (cfgRT & ~fl_REG_CFG_ena_codec);
        if (checkBoxCodec->isChecked())
            cfgRT |= fl_REG_CFG_ena_codec;     // кодек
        cfgRT = (cfgRT & ~fl_REG_CFG_rtavsk_ena);
        if (checkBoxEnaAddr->isChecked())
        {
            cfgRT |= fl_REG_CFG_rtavsk_ena;
            cfgRT = (cfgRT & ~FL_REG_CFG_rtavsk) | AddrToCfg(curThread->rtaddr);
            curThread->rtaddr = MAX_RT_ADDR + 1;
        }
        curThread->cfgRT = cfgRT;
        curThread->amplRT = comboBoxAmplRT->currentIndex();
        if (checkBoxSPILoad->isChecked())
        {
            curThread->dataSPI = lineEditSPIData->text().toInt(0, 16);
            curThread->modeSPI = (comboBoxSPIdr->currentIndex() << SH_REG_CR_SPI_dr8_16_32) | comboBoxSPIen->currentIndex();
        }
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
    test.setConfiguration(cfgBC);
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


    int sz_dst = (test.NumSymOFDM(num_b) + 1) * NUMWORDINOFDMSYM * sizeof(word32_t);
    qDebug() << "sizes: " << num_b << " " << (test.NumSymOFDM(num_b) + 1) << " " << sz_dst;
    curThread->data = malloc(sz_dst);
    if (curThread->data == 0)
    {
        message(tr("not enough memory"));
        destroyData(rawData);
        return;
    }
    memset(curThread->data, 0, sz_dst);
    if (!test.createCommandPack(curThread->data, sz_dst, rawData, num_b, rta, tgca_tr_REC, comboBoxCode->currentIndex())) // ОУ принимает
    {
        message(tr("Error in createCommandPack() !!!"));
        destroyData(rawData);
        free(curThread->data);
        curThread->data = 0;
        return;
    }
    curThread->data_size = sz_dst;
    destroyData(rawData);

    emit startTestTh();
}

trmSingleObjToThread::trmSingleObjToThread():
    absObjToThread(),
    cfgBC(0), cfgRT(0), amplBC(0), amplRT(0),
    devBC(0), devRT(0),
    data(0)
{
}

bool trmSingleObjToThread::checkStatusReg(Device *dev, int *status)
{
    int addr_stat = REG_VSK_status;
    QByteArray answer;
    QByteArray readArray = cmdHead(3, 4);
    readArray.append((char*)&addr_stat, 4);
    if (dev->read_F1(readArray, answer) == -1)
        return false;

    *status = *(int*)(answer.data());
    return true;
}

bool trmSingleObjToThread::checkStatusRegBC(int *status)
{
    if (devBC == 0 || !checkStatusReg(devBC, status))
    {
        stdOutput(tr("Error reading BC status register"));
        return false;
    }
    /*
    if ((*status) & FL_REG_STATUS_ERR_flags)
        qDebug() << "BC status  Trm buf = " << ((*status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((*status)&fl_REG_STATUS_rx_num_buf ? 1 : 0)
                    << "  !!! Error flags in status:  " << QString::number(*status, 16);
    else
        qDebug() << "BC status  Trm buf = " << ((*status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((*status)&fl_REG_STATUS_rx_num_buf ? 1 : 0);
    */

    return true;
}

bool trmSingleObjToThread::checkStatusRegRT(int *status)
{
    if (devRT == 0 || !checkStatusReg(devRT, status))
    {
        stdOutput(tr("Error reading RT status register"));
        return false;
    }
    if ((*status) & FL_REG_STATUS_ERR_flags)
        qDebug() << "RT status  Trm buf = " << ((*status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((*status)&fl_REG_STATUS_rx_num_buf ? 1 : 0)
                    << "  !!! Error flags in status:  " << QString("%1").arg(*status, 4, 16, QLatin1Char('0'));
    else
        qDebug() << "RT status  Trm buf = " << ((*status)&fl_REG_STATUS_tx_num_buf ? 1 : 0) << "  Rec buf = " << ((*status)&fl_REG_STATUS_rx_num_buf ? 1 : 0);

    return true;
}

bool trmSingleObjToThread::checkCounters(Device *dev)
{
    if (checkCountersEnable)
    {
        int addr_reg = REG_VSK_cnt_pct_tx_msw;
        QByteArray answer;
        QByteArray readArray = cmdHead(3, 16);
        for (int i=0; i<4; i++)
        {
            readArray.append((char*)&addr_reg, 4);
            addr_reg += 4;
        }
        if (dev->read_F1(readArray, answer) == -1)
            return false;

        qDebug() << "Counters:  " << (*(int*)(answer.data())) << "  " << (*(int*)(answer.data()+4))
                 << "  " << (*(int*)(answer.data()+8)) << "  " << (*(int*)(answer.data()+12));
    }
    return true;
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

void trmSingleObjToThread::stdOutput(QString message)
{
    if (outEnable)
        emit outputReady(message);
    else
        qDebug() << message;
}

void trmSingleObjToThread::doWork()
{
    qDebug() << "trmSingleObjToThread::doWork() started";
    if (data_size > 0 && iterCycle > 0)
        stdOutput(QString(tr("Command word = %1")).arg(*(int*)data, 8, 16, QLatin1Char('0')));
    else
        stdOutput(QString(tr("Data size = 0")));

    if (devBC)
    {
        QString ip = devBC->connection.getServerIP();
        ushort port = devBC->connection.getServerPORT().toUShort();
        tcpSocketBC.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketBC.waitForConnected(5000))
        {
            stdOutput(tr("BC: no connection"));
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }
        qDebug() << tr("BC connected");
        devBC->setSocket(&tcpSocketBC);

        if (!checkCounters(devBC))
            return;
    }
    else
    {
        stdOutput(tr("BC not used"));
    }
    if (devRT)
    {
        QString ip = devRT->connection.getServerIP();
        ushort port = devRT->connection.getServerPORT().toUShort();
        tcpSocketRT.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketRT.waitForConnected(5000)) {
            stdOutput(tr("RT: no connection"));
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }
        qDebug() << "RT connected";
        devRT->setSocket(&tcpSocketRT);

        if (!checkCounters(devRT))
            return;
    }
    else
    {
        stdOutput(tr("RT not used"));
    }

    int addr_cfg  = REG_VSK_cfg;
    int addr_ampl = REG_VSK_amplification_factor;
    int addr_rta  = REG_AUX_rtaddr;        // вспомогательный регистр: адрес ОУ
    int addr_int  = REG_AUX_interruption;  // вспомогательный регистр: прерывание
    int addr_win  = REG_AUX_win_mode;      // вспомогательный регистр: оконный режим
    int addr_stat = REG_VSK_status;
    int addr_creg = REG_VSK_creg;
    int addr_spi  = REG_VSK_cr_spi;
    int addr_spi_data_low  = REG_VSK_dr_spi_lsw;

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
                QByteArray regs = cmdHead(1, regRT.size());
                regs.append(regRT);
                if (devRT->write_F1(regs) == -1)
                    return;
                devRT->configuration.doneWriteReg(regRT);
            }
            if (compEnable)
            {
                QByteArray addrArray, valArray;
                if (split(regRT, addrArray, valArray))
                {
                    // Чтение регистров КШ для сравнения
                    QByteArray writeArray = cmdHead(3, addrArray.size());
                    writeArray.append(addrArray);
                    QByteArray readArray;
                    if (devRT->read_F1(writeArray, readArray) == -1)
                        return;

                    if (readArray == valArray)
                    {
                        stdOutput(tr("Comparison RT registers OK"));
                    }
                    else
                    {
                        stdOutput(tr("Comparison RT registers wrong"));
                        stdOutput(tr("Returned size = %1").arg(readArray.size()));
                        stdOutput(tr("Returned data = %1").arg(*(int*)(readArray.data()), 4, 16, QLatin1Char('0')));
                    }
                }
            }
            if (!checkCounters(devRT))
                return;
        }

        if (writeCfg)
        {
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_cfg, 4);
            array.append((char*)&cfgRT, 4);
            if (devRT->write_F1(array) == -1)
                return;
            devRT->configuration.setWritten(addr_cfg, cfgRT);
        }
        if (compEnable)
        {
            int rv;
            if (devRT->readReg(addr_cfg, &rv) != -1)
            {
                if (rv != cfgRT)
                {
                    stdOutput(tr("Comparison RT cfg register wrong"));
                    emit resultReady((int)AbstractTest::ErrorIsOccured);
                    return;
                }
            }
            else
                return;
        }

        if (amplRT)
        {
            int ampl = (amplRT == 1) ? 0 : amplRT + 14;
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_ampl, 4);
            array.append((char*)&ampl, 4);
            if (devRT->write_F1(array) == -1)
                return;
            devRT->configuration.setWritten(addr_ampl, ampl);
            if (compEnable)
            {
                int rv;
                if (devRT->readReg(addr_ampl, &rv)!= -1)
                {
                    if (rv != ampl)
                    {
                        stdOutput(tr("Comparison RT amplification factor wrong"));
                    }
                }
                else
                    return;
            }
        }
        if ((modeSPI & FL_REG_CR_SPI_dr8_16_32) != val_REG_CR_SPI_dr_UNDEF)
        {
            QByteArray array = cmdHead(1, 16);
            array.append((char*)&addr_spi_data_low, 4);
            array.append((char*)&dataSPI, 4);
            array.append((char*)&addr_spi, 4);
            array.append((char*)&modeSPI, 4);
            if (devRT->write_F1(array) == -1)
                return;
            if (compEnable)
            {
                int rv;
                if (devRT->readReg( addr_spi, &rv) != -1)
                {
                    if (rv != (modeSPI | fl_REG_CR_SPI_spif))
                    {
                        stdOutput(tr("Comparison SPI register wrong"));
                    }
                }
                else
                    return;
            }
        }

        if (rtaddr <= MAX_RT_ADDR)
        {
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_rta, 4);
            array.append((char*)&rtaddr, 4);
            if (devRT->write_F1(array) == -1)
                return;
            if (compEnable)
            {
                int rv;
                if (devRT->readReg(addr_rta, &rv) != -1)
                {
                    if (rv != rtaddr)
                    {
                        stdOutput(tr("Comparison RTA register wrong"));
                    }
                }
                else
                    return;
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
                QByteArray regs = cmdHead(1, regBC.size());
                regs.append(regBC);
                if (devBC->write_F1(regs) == -1)
                    return;
                devBC->configuration.doneWriteReg(regBC);
            }
            if (compEnable)
            {
                QByteArray addrArray, valArray;
                if (split(regBC, addrArray, valArray))
                {
                    // Чтение регистров КШ для сравнения
                    QByteArray writeArray = cmdHead(3, addrArray.size());
                    writeArray.append(addrArray);
                    QByteArray readArray;
                    if (devBC->read_F1(writeArray, readArray) == -1)
                        return;

                    if (readArray == valArray)
                    {
                        stdOutput(tr("Comparison BC registers OK"));
                    }
                    else
                    {
                        stdOutput(tr("Comparison BC registers wrong"));
                        stdOutput(tr("Returned size = %1").arg(readArray.size()));
                        stdOutput(tr("Returned data = %1").arg(*(int*)(readArray.data()), 4, 16, QLatin1Char('0')));
                    }
                }
            }
            if (!checkCounters(devBC))
                return;
        }

        if (writeCfg)
        {
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_cfg, 4);
            array.append((char*)&cfgBC, 4);
            if (devBC->write_F1(array) == -1)
                return;
            devBC->configuration.setWritten(addr_cfg, cfgBC);
        }
        if (compEnable)
        {
            int rv;
            if (devBC->readReg(addr_cfg, &rv) != -1)
            {
                if (rv != cfgBC)
                {
                    stdOutput(tr("Comparison BC cfg register wrong"));
                    emit resultReady((int)AbstractTest::ErrorIsOccured);
                    return;
                }
            }
            else
                return;
        }

        if (amplBC)
        {
            int ampl = (amplBC == 1) ? 0 : amplBC + 14;
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_ampl, 4);
            array.append((char*)&ampl, 4);
            if (devBC->write_F1(array) == -1)
                return;
            devBC->configuration.setWritten(addr_ampl, ampl);
            if (compEnable)
            {
                int rv;
                if (devBC->readReg(addr_ampl, &rv) != -1)
                {
                    if (rv != ampl)
                    {
                        stdOutput(tr("Comparison BC amplification factor wrong"));
                    }
                }
                else
                    return;
            }
        }
    }
    qDebug() << "data_size = " << data_size;
    if (data_size > 0 && devBC != 0 && iterCycle != 0)
    {
        int statusBC, statusRT;
        if (devRT)
        {
            if (!checkStatusRegRT(&statusRT))
                return;
            if (outEnable)
                emit outputReady(tr("iter = %1   RT status = %2").arg(0).arg(statusRT, 4, 16, QLatin1Char('0')));
        }

        QByteArray pack0 = cmdHead(2, data_size+4);
        QByteArray pack1 = cmdHead(2, data_size+4);
        QByteArray compArrayW;
        if (devBC)
        {
            if (!checkStatusRegBC(&statusBC))
                return;

            // формирование массивов для записи в буферы передатчика КШ
            uint buf = getBufTrm(0);
            pack0.append((char*)&buf, 4);
            pack0.append((char*)data, data_size);

            buf = getBufTrm(1);
            pack1.append((char*)&buf, 4);
            pack1.append((char*)data, data_size);

            // формирование массива для сравнения
            if (compEnable)
                compArrayW.append((char*)data, data_size);
        }
        free(data);
        data = 0;

        if (devBC)
        {
            for (uint it = 0; it!=iterCycle; it++)
            {
                if (pause_stop() == -1)
                    return;

                // Запись данных в буфер передатчика
//                if (outEnable)
//                    emit outputReady(tr("iter=%1 trm buf=%2").arg(it).arg((statusBC & fl_REG_STATUS_tx_num_buf)==0 ? 0 : 1));
                if (statusBC & fl_REG_STATUS_tx_num_buf)
                {
                    if (devBC->write_F2(pack1) == -1)
                        return;
                }
                else
                {
                    if (devBC->write_F2(pack0) == -1)
                        return;
                }

                if (compEnable && it<=1)
                {
                // Чтение данных из буфера передатчика КШ для сравнения
                    QByteArray writeArray = cmdHead(4, 8);
                    int buf = getBufTrm(statusBC & fl_REG_STATUS_tx_num_buf);
                    writeArray.append((char*)&buf, 4);
                    writeArray.append((char*)&data_size, 4);
                    QByteArray readArray;
                    if (devBC->read_F2(writeArray, readArray) == -1)
                        return;

                    if (readArray == compArrayW)
                    {
                        stdOutput(tr("Comparison BC trm buffer OK"));
                    }
                    else
                    {
                        stdOutput(tr("Comparison BC trm buffer wrong"));
                        stdOutput(tr("Returned size = %1").arg(readArray.size()));
                        stdOutput(tr("Command word = %1").arg(*(int*)(readArray.data()), 8, 16, QLatin1Char('0')));
                    }
                }

                // Оконный режим
                if(windowMode && devRT)
                {
                    QByteArray array = cmdHead(1, 8);
                    int val = 1;
                    array.append((char*)&addr_win, 4);
                    array.append((char*)&val, 4);
                    if (devRT->write_F1(array) == -1)
                        return;
                    if (devBC->write_F1(array) == -1)
                        return;
                }
                // Старт обмена
                QByteArray array = cmdHead(1, 8);
                int start_bc = fl_REG_CREG_start_bc;
                array.append((char*)&addr_creg, 4);
                array.append((char*)&start_bc, 4);
                QByteArray readArray = cmdHead(3, 4);
                readArray.append((char*)&addr_stat, 4);
                QByteArray readInterrupt;
                if (useInt && (cfgBC & fl_REG_CFG_en_rt_bc_int) != 0)
                {
                    readInterrupt.append(cmdHead(3, 4));
                    readInterrupt.append((char*)&addr_int, 4);
                }
                if (devRT!=0 && compEnable && it<=1)
                    checkStatusRegRT(&statusRT);

                if (devBC->write_F1(array) == -1)
                    return;

                if (devRT)
                {

                        //qDebug() << "Check RT status";
                        QByteArray answer1;
                        if (devRT->read_F1(readArray, answer1) == -1) return;
                        if (outEnable)
                            emit outputReady(tr("iter = %1   RT status = %2").arg(it).arg(*(int*)(answer1.data()), 4, 16, QLatin1Char('0')));
                        //answer1.clear();

                    int interruption = 1;
                    {
                        if (useInt && (cfgBC & fl_REG_CFG_en_rt_bc_int) != 0)
                        {
                            QTimer timer;
                            timer.setInterval(waitTime);
                            timer.start();
                            do
                            {
                                QByteArray answer;
                                if (devBC->read_F1(readInterrupt, answer) == -1) return;
                                interruption = *(int*)(answer.data());
                            }
                            while (interruption == 0 || timer.remainingTime() > 0);
                            //timer.killTimer();

                            if (interruption == 0)
                                stdOutput(tr("iter = %1   нет прерывания КШ").arg(it));

                            QByteArray answer;
                            if (devBC->read_F1(readArray, answer) == -1) return;
                            statusBC = *(int*)(answer.data());
                        }
                        else
                        {
                            bool yes_aw_gr_err = false, no_aw_err = false, rs_err = false;
                            QTimer timer;
                            timer.setInterval(waitTime);
                            timer.start();
                            do
                            {
                                QByteArray answer;
                                if (devBC->read_F1(readArray, answer) == -1) return;
                                statusBC = *(int*)(answer.data());
                                //counter++;
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
                        QByteArray array = cmdHead(1, 8);
                        int val = 0;
                        array.append((char*)&addr_win, 4);
                        array.append((char*)&val, 4);
                        if (devRT->write_F1(array) == -1)
                            return;
                        if (devBC->write_F1(array) == -1)
                            return;
                    }
                    if ((statusBC & fl_REG_STATUS_rt_bc_int) == 0)
                    {
                        stdOutput(QString("Ошибка признака завершения обмена"));
                        stdOutput(tr("iter = %1   BC status = %2").arg(it).arg(statusBC, 4, 16, QLatin1Char('0')));
                        emit resultReady((int)AbstractTest::ErrorIsOccured);
                        return;
                    }
                    else if (statusBC & FL_REG_STATUS_ERR_flags)
                    {
                        stdOutput(QString("Errors in BC status: %1").arg(statusBC, 4, 16, QLatin1Char('0')));
                    }
                    if (interruption == 0)
                    {
                        stdOutput(tr("iter = %1   BC status = %2").arg(it).arg(statusBC, 4, 16, QLatin1Char('0')));
                        emit resultReady((int)AbstractTest::ErrorIsOccured);
                        return;
                    }
                    if (!checkCounters(devBC) || ! checkCounters(devRT))
                        return;

                    if (compEnable && it<=1)
                    {
                    // Чтение данных из буфера приёмника ОУ для сравнения
                        QByteArray writeArray = cmdHead(4, 8);
                        int buf = getBufRec(statusRT & fl_REG_STATUS_rx_num_buf);
                        writeArray.append((char*)&buf, 4);
                        writeArray.append((char*)&data_size, 4);
                        QByteArray readArray;
                        if (devRT->read_F2(writeArray, readArray) == -1)
                            return;

                        if (readArray == compArrayW)
                        {
                            stdOutput(tr("Comparison RT rec buffer OK"));
                        }
                        else
                        {
                            stdOutput(tr("Comparison RT rec buffer wrong"));
                            stdOutput(tr("Returned size = %1").arg(readArray.size()));
                            stdOutput(tr("Read word = %1").arg(*(int*)(readArray.data()), 8, 16, QLatin1Char('0')));
                        }
                    }
                } // devRT
                else
                {
                    if (!checkStatusRegBC(&statusBC))
                        return;
                    if (outEnable)
                        qDebug() << QString(tr("%1").arg(statusBC, 4, 16, QLatin1Char('0')));
                }
                if (pauseTime > 0)
                    thread()->msleep(pauseTime);
            } // цикл
        } // devBC
    } // data_size > 0

    emit resultReady((int)AbstractTest::Completed);
}

void trmSingleObjToThread::terminate(int fl)
{
    qDebug() << "terminates: " << devBC << "  " << devRT << "  " << data;
    if (fl == AbstractTest::ErrorIsOccured || fl == AbstractTest::Completed || fl == AbstractTest::Stopped)
    {
        if (devBC != 0 && tcpSocketBC.state() == QAbstractSocket::ConnectedState)
            tcpSocketBC.abort();
        if (devRT != 0 && tcpSocketRT.state() == QAbstractSocket::ConnectedState)
            tcpSocketRT.abort();
        if (data)
            free(data);
    }
}

//lineEditSPIData->setInputMask(QApplication::translate("TestTrmSingle", "HHHH;_", 0));
