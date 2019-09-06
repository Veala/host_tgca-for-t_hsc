#include "varcommtest.h"
#include "../ctestbc.h"
#include "../codeselect.h"
#include "../command.h"
#include "../testutil.h"

void VarCommandTest::setStatSettings()
{
    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("totalErr", stats->findChild<QLabel*>("totalErr"));
    statsMap.insert("errNoSW", stats->findChild<QLabel*>("errNoSW"));
    statsMap.insert("errNoFinRT", stats->findChild<QLabel*>("errNoFinRT"));
    statsMap.insert("errNoFinBC", stats->findChild<QLabel*>("errNoFinBC"));
    statsMap.insert("errCompare", stats->findChild<QLabel*>("errCompare"));
    statsMap.insert("errStatusRT", stats->findChild<QLabel*>("errStatusRT"));
    statsMap.insert("errStatusBC", stats->findChild<QLabel*>("errStatusBC"));
    statsMap.insert("errReceive", stats->findChild<QLabel*>("errReceive"));
    statsMap.insert("errRW", stats->findChild<QLabel*>("errRW"));
    statsMap.insert("errBefore", stats->findChild<QLabel*>("errBefore"));
    statsMap.insert("errFatal", stats->findChild<QLabel*>("errFatal"));
    statsMap.insert("otherErr", stats->findChild<QLabel*>("otherErr"));

    connectStatisticSlots();
    stats->findChild<QLabel*>("errFatal")->setVisible(false);
    stats->findChild<QLabel*>("labelErrFatal")->setVisible(false);
}

void VarCommandTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    defineFields();

    load(fName);

    setFieldConnections();
    setTestConnections();

    disableUnimplemented();    // отключить нереализованные настройки
    checkBoxParamView->setVisible(false);
    settings->findChild<QLabel*>("labelParamView")->setVisible(false);

    deviceLineEditList.append(lineEditDevBC);
    deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability(3);

    objToThread = new varCommandObjToThread();
    connectThread();

    setStatSettings();

    testThread.start();
}

void VarCommandTest::defineFields()
{
    dataGen.settings();

    lineEditTime = settings->findChild<QLineEdit*>("lineEditTime");
    lineEditPause = settings->findChild<QLineEdit*>("lineEditPause");

    comboBoxRTA = settings->findChild<QComboBox*>("comboBoxRTA");
    comboBoxManType = settings->findChild<QComboBox*>("comboBoxManType");

    checkBoxCodec = settings->findChild<QCheckBox*>("checkBoxCodec");
    checkBoxEnaInt = settings->findChild<QCheckBox*>("checkBoxEnaInt");
    checkBoxEnaAddr = settings->findChild<QCheckBox*>("checkBoxEnaAddr");
    checkBoxUseInt = settings->findChild<QCheckBox*>("checkBoxUseInt");
    labelUseInt = settings->findChild<QLabel*>("labelUseInt");
    checkBoxOut = settings->findChild<QCheckBox*>("checkBoxOut");
    checkBoxInit = settings->findChild<QCheckBox*>("checkBoxInit");

    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");

    lineEditCycle = settings->findChild<QLineEdit*>("lineEditCycle");

    checkBoxRTALoad = settings->findChild<QCheckBox*>("checkBoxRTALoad");

    comboBoxBCOutPref = settings->findChild<QComboBox*>("comboBoxBCOutPref");
    comboBoxRTOutPref = settings->findChild<QComboBox*>("comboBoxRTOutPref");
    lineEditBCOut = settings->findChild<QLineEdit*>("lineEditBCOut");
    lineEditRTOut = settings->findChild<QLineEdit*>("lineEditRTOut");

    lineEditReservePause = settings->findChild<QLineEdit*>("lineEditReservePause");

    comboBoxErrStatusBC = settings->findChild<QComboBox*>("comboBoxErrStatusBC");
    comboBoxErrStatusRT = settings->findChild<QComboBox*>("comboBoxErrStatusRT");
    comboBoxCheckRW = settings->findChild<QComboBox*>("comboBoxCheckRW");
    comboBoxRec = settings->findChild<QComboBox*>("comboBoxRec");
    comboBoxBCIntErr = settings->findChild<QComboBox*>("comboBoxBCIntErr");
    comboBoxRTIntErr = settings->findChild<QComboBox*>("comboBoxRTIntErr");
    comboBoxMemBC = settings->findChild<QComboBox*>("comboBoxMemBC");
    comboBoxMemRT = settings->findChild<QComboBox*>("comboBoxMemRT");

    checkBoxParamView = settings->findChild<QCheckBox*>("checkBoxParamView");
    comboBoxCheckRTA = settings->findChild<QComboBox*>("comboBoxCheckRTA");

    pushButtonCodes = settings->findChild<QPushButton*>("pushButtonCodes");
    lineViewCodes = settings->findChild<QLineEdit*>("lineViewCodes");
}

void VarCommandTest::load(QString fName)
{
top_1

    comboBoxRTA->setCurrentText(out.readLine());

    comboBoxManType->setCurrentText(out.readLine());
    checkBoxCodec->setChecked(!out.readLine().isEmpty());
    checkBoxEnaInt->setChecked(!out.readLine().isEmpty());
    checkBoxEnaAddr->setChecked(!out.readLine().isEmpty());

    QString genDataType = out.readLine();
    QString genDataBegin = out.readLine();
    QString genDataStep = out.readLine();
    QString genDataNumStep = out.readLine();
    QString genDataUnit = out.readLine();

    lineEditTime->setText(out.readLine());
    lineEditPause->setText(out.readLine());
    checkBoxUseInt->setCheckState(out.readLine().isEmpty() ? Qt::Unchecked : Qt::Checked);
    checkBoxOut->setChecked(!out.readLine().isEmpty());

    lineEditDevBC->setText(out.readLine());
    lineEditDevRT->setText(out.readLine());

    checkBoxParamView->setChecked(!out.readLine().isEmpty());
    checkBoxInit->setChecked(!out.readLine().isEmpty());
    lineEditCycle->setText(out.readLine());
    checkBoxRTALoad->setChecked(!out.readLine().isEmpty());
    comboBoxBCOutPref->setCurrentText(out.readLine());
    comboBoxRTOutPref->setCurrentText(out.readLine());

    lineEditBCOut->setText(out.readLine());
    lineEditRTOut->setText(out.readLine());

    comboBoxErrStatusBC->setCurrentIndex(out.readLine().toInt());
    comboBoxErrStatusRT->setCurrentIndex(out.readLine().toInt());
    comboBoxCheckRW->setCurrentIndex(out.readLine().toInt());
    comboBoxRec->setCurrentIndex(out.readLine().toInt());
    comboBoxBCIntErr->setCurrentIndex(out.readLine().toInt());
    comboBoxRTIntErr->setCurrentIndex(out.readLine().toInt());
    comboBoxMemBC->setCurrentIndex(out.readLine().toInt());
    comboBoxMemRT->setCurrentIndex(out.readLine().toInt());
    out.readLine();
    lineEditReservePause->setText(out.readLine());
    QString genDataLen = out.readLine();
    comboBoxCheckRTA->setCurrentIndex(out.readLine().toInt());
    QString codes = out.readLine();

    settingsFile.close();
    dataGen.init(genDataType, genDataBegin, genDataStep, genDataNumStep, genDataUnit, genDataLen);
    codesToLabel(codes, lineViewCodes);
}

void VarCommandTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << comboBoxRTA->currentText() << endl;

    in << comboBoxManType->currentText() << endl;
    in << (checkBoxCodec->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaAddr->isChecked() ? "1" : "") << endl;

    in << dataGen.genType() << endl;
    in << dataGen.begin() << endl;
    in << dataGen.step() << endl;
    in << dataGen.numStep() << endl;
    in << dataGen.unit() << endl;

    in << lineEditTime->text() << endl;
    in << lineEditPause->text() << endl;
    in << (checkBoxUseInt->checkState() == Qt::Checked ? "1" : "") << endl;
    in << (checkBoxOut->isChecked() ? "1" : "") << endl;

    in << lineEditDevBC->text() << endl;
    in << lineEditDevRT->text() << endl;

    in << (checkBoxParamView->isChecked() ? "1" : "") << endl;
    in << (checkBoxInit->isChecked() ? "1" : "") << endl;
    in << lineEditCycle->text() << endl;
    in << (checkBoxRTALoad->isChecked() ? "1" : "") << endl;

    in << comboBoxBCOutPref->currentText() << endl;
    in << comboBoxRTOutPref->currentText() << endl;
    in << lineEditBCOut->text() << endl;
    in << lineEditRTOut->text() << endl;

    in << comboBoxErrStatusBC->currentIndex() << endl;
    in << comboBoxErrStatusRT->currentIndex() << endl;
    in << comboBoxCheckRW->currentIndex() << endl;
    in << comboBoxRec->currentIndex() << endl;
    in << comboBoxBCIntErr->currentIndex() << endl;
    in << comboBoxRTIntErr->currentIndex() << endl;
    in << comboBoxMemBC->currentIndex() << endl;
    in << comboBoxMemRT->currentIndex() << endl;
    in << tr("reserve") << endl;
    in << lineEditReservePause->text() << endl;
    in << dataGen.dataLen() << endl;
    in << comboBoxCheckRTA->currentIndex() << endl;
    in << commmonTest_labelToCodes(lineViewCodes);

    settingsFile.close();
}

void VarCommandTest::setFieldConnections()
{
    connect(checkBoxUseInt, SIGNAL(clicked()), this, SLOT(onCheckUseInt()));
    connect(checkBoxEnaInt, SIGNAL(clicked()), this, SLOT(onCheckEnaInt()));
    connect(pushButtonCodes, SIGNAL(clicked()), this, SLOT(onPushCodes()));
}

void VarCommandTest::onPushCodes()
{
    CodeSelect cs(this);
    cs.labelToCodes(lineViewCodes->text());
    connect(&cs, SIGNAL(applyCodes(QString)), this, SLOT(applyCodes(QString)));
    cs.exec();
}

void VarCommandTest::applyCodes(QString str)
{
    commmonTest_applyCodesToLabel(lineViewCodes, str);
}

void VarCommandTest::onCheckUseInt()
{
    if (checkBoxUseInt->checkState() == Qt::PartiallyChecked)
        checkBoxUseInt->setCheckState(Qt::Checked);
}

void VarCommandTest::onCheckEnaInt()
{
    checkBoxUseInt->setEnabled(checkBoxEnaInt->isChecked());
    if (checkBoxUseInt->checkState() == Qt::PartiallyChecked)
        checkBoxUseInt->setCheckState(Qt::Checked);
    else if (checkBoxUseInt->checkState() == Qt::Checked)
        checkBoxUseInt->setCheckState(Qt::PartiallyChecked);
}

void VarCommandTest::disableUnimplemented()
{
    // ПОКА НЕ РЕАЛИЗОВАНЫ
    checkBoxParamView->setDisabled(true);
    settings->findChild<QLabel*>("labelParamView")->setDisabled(true);
    return;
    comboBoxCheckRW->setDisabled(true);                             // проверка ответного пакета
    settings->findChild<QLabel*>("labelCheckRW")->setDisabled(true);
    comboBoxRec->setDisabled(true);                                 // проверка, что сообщение от КШ принято ОУ
    settings->findChild<QLabel*>("labelReceived")->setDisabled(true);
}

void VarCommandTest::setEnabledSpecial(bool b)
{
    AbstractTest::setEnabledSpecial(b);

    if (b)
    {
        disableUnimplemented();    // отключить нереализованные настройки
    }
    else
    {
        comboBoxRTA->setEnabled(true);
        checkBoxRTALoad->setEnabled(true);
        //checkBoxCodec->setEnabled(true);
        //settings->findChild<QLabel*>("labelCodec")->setEnabled(true);
        settings->findChild<QLabel*>("labelRTA")->setEnabled(true);
        settings->findChild<QLabel*>("labelRTARegLoad")->setEnabled(true);
        settings->findChild<QLabel*>("labelHeaderRegisters")->setEnabled(true);

        //dataGen.enable(true);

        settings->findChild<QLabel*>("labelHeaderPause")->setEnabled(true);
        settings->findChild<QLabel*>("labelTime")->setEnabled(true);
        settings->findChild<QLabel*>("labelPause")->setEnabled(true);
        lineEditTime->setEnabled(true);
        lineEditPause->setEnabled(true);

        lineViewCodes->setEnabled(true);
        settings->findChild<QLabel*>("labelCodes")->setEnabled(true);
    }
}

void VarCommandTest::startTest()
{
    varCommandObjToThread* curThread = (varCommandObjToThread*)objToThread;

    curThread->useInt = ((checkBoxUseInt->checkState() == Qt::Checked) && checkBoxEnaInt->isChecked()); // напоминание: загрузка cfg_reg в этом тесте всегда включена
    curThread->setOutEnabled(checkBoxOut->isChecked());
    curThread->waitTime = lineEditTime->text().toInt(0, 16);
    curThread->rtaddr = comboBoxRTA->currentIndex();

    //curThread->testData = 0;
    curThread->compEnableMemBC = (comboBoxMemBC->currentIndex() > 0);
    curThread->compEnableMemRT = (comboBoxMemRT->currentIndex() > 0);
    curThread->iterCycle = lineEditCycle->text().toInt(0, 10);
    if (curThread->iterCycle <= 0)
        curThread->iterCycle = 1;
    curThread->pauseTime = lineEditPause->text().toInt(0, 10);
    curThread->postponeTime = lineEditReservePause->text().toInt(0, 10);
    curThread->checkStatusErrBC = (comboBoxErrStatusBC->currentIndex() > 0);
    curThread->checkStatusErrRT = (comboBoxErrStatusRT->currentIndex() > 0);
    curThread->noIntFatalBC = (comboBoxBCIntErr->currentIndex() != 0);
    curThread->noIntFatalRT = comboBoxRTIntErr->currentIndex();
    curThread->perOutBC = lineEditBCOut->text().isEmpty() ? 0 : lineEditBCOut->text().toInt(0, 10);
    curThread->perOutRT = lineEditRTOut->text().isEmpty() ? 0 : lineEditRTOut->text().toInt(0, 10);
    curThread->modeOutBC = comboBoxBCOutPref->currentIndex();
    curThread->modeOutRT = comboBoxRTOutPref->currentIndex();
    curThread->initEnable = checkBoxInit->isChecked();

    //curThread->manipulation = comboBoxManType->currentText();
    curThread->codec = checkBoxCodec->isChecked();

    curThread->devBC = deviceList.at(0);
    quint32 cfgBC = curThread->devBC->configuration.getConfigReg();
    curThread->devBC->reg_hsc_cfg.setData(cfgBC);
    curThread->devBC->reg_hsc_cfg.rt_bc = CFG_MODE_BC;   // признак КШ
    curThread->devBC->reg_hsc_cfg.type_man = comboBoxManType->currentIndex();
    curThread->devBC->reg_hsc_cfg.en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;
    curThread->devBC->reg_hsc_cfg.ena_codec = checkBoxCodec->isChecked() ? 1 : 0;

    curThread->devRT = deviceList.at(1);

    quint32 cfgRT = curThread->devRT->configuration.getConfigReg();
    curThread->devRT->reg_hsc_cfg.setData(cfgRT);
    curThread->devRT->reg_hsc_cfg.rt_bc = CFG_MODE_RT;     // признак ОУ
    curThread->devRT->reg_hsc_cfg.type_man = comboBoxManType->currentIndex();
    curThread->devRT->reg_hsc_cfg.en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;
    curThread->devRT->reg_hsc_cfg.ena_codec = checkBoxCodec->isChecked() ? 1 : 0;
    curThread->devRT->reg_hsc_cfg.rtavsk_ena = checkBoxEnaAddr->isChecked() ? 1 : 0;
    if (checkBoxEnaAddr->isChecked())
        curThread->devRT->reg_hsc_cfg.rtavsk = curThread->rtaddr;

    // Конфигурационный регистр будет загружен обязательно.
    // Если адрес ОУ содержится в конфигурационном регистре, то мы его не будем загружать во вспомогательный регистр.
    // Проверка записи регистров в этом тесте не предусмотрена, поэтому проверка адреса ОУ
    // осуществляется только на ветке, когда запись этого адреса в регистры выключена.
    curThread->loadRTA = checkBoxRTALoad->isChecked() && !checkBoxEnaAddr->isChecked();
    curThread->checkRTA = (comboBoxCheckRTA->currentIndex() > 0) && !checkBoxRTALoad->isChecked() && !checkBoxEnaAddr->isChecked();

    curThread->dataSize = dataGen.getDataLen();
    int dataSizeRequired = MAXNUMSYM * calcNumWordInSymbol(comboBoxManType->currentIndex(), curThread->codec?1:0) - sizeof(word32_t);
    if (curThread->dataSize < dataSizeRequired)
        curThread->dataSize = dataSizeRequired;
    curThread->testData = dataGen.createData(curThread->dataSize, 2);

    curThread->codeMask = lineViewCodes->text();

    curThread->checkResponse = comboBoxCheckRW->currentIndex();
    curThread->checkReceiving = (comboBoxRec->currentIndex() > 0);

    emit startTestTh();
}

bool varCommandObjToThread::checkStatusRegBC(int statusBC, int interruption, int it, bool *error)
{
    bool bNoInt = false;

    if (interruption == 0)
    {
        stdOutput(tr("Итерация = %1   Нет прерывания КШ").arg(it, 6), tr("Iter = %1   No interruption from BC").arg(it, 6));
        *error = true;
        bNoInt = true;
    }
#if CHECKFINBIT
    if ((statusBC & fl_REG_STATUS_rt_bc_int) == 0)
    {
        stdOutput(tr("Итерация = %1   Нет признака завершения обмена КШ").arg(it, 6),
                  tr("Iter = %1   No interruption flag in BS status").arg(it, 6));
        *error = true;
        bNoInt = true;
    }
#endif

    if ( checkStatusErrBC && ( (codec && ((statusBC & fl_REG_STATUS_rs_err) != 0)) ||
        (statusBC & fl_REG_STATUS_no_aw_err) != 0 || (statusBC & fl_REG_STATUS_yes_aw_gr_err) != 0) )
    {
            stdOutput(tr("Итерация = %1   Ошибка в статусе КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')),
                      tr("Iter = %1   Errors in BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')));
        *error = true;
        emit statsOutputReady("errStatusBC", 1);
    }
    else if (bNoInt)
    {
            stdOutput(tr("Статус КШ: %1").arg(statusBC, 4, 16, QLatin1Char('0')),
                      tr("BC status: %1").arg(statusBC, 4, 16, QLatin1Char('0')));
    }
    else if ((it < perOutBC && modeOutBC == 0) || (perOutBC != 0 && modeOutBC == 1 && (it%perOutBC) == 0))
    {
            stdOutput(tr("Итерация = %1   Статус КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')),
                      tr("Iter = %1   BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')));
    }
    if (bNoInt)
        emit statsOutputReady("errNoFinBC", 1);

    return bNoInt;
}

bool varCommandObjToThread::checkStatusRegRT(int status, int it, bool *error)
{
    bool bNoInt = false;

    /*
    if (useInt && noIntFatalRT)
    {
        devRT->readReg(&devRT->reg_aux_interruption);
        if (devRT->reg_aux_interruption.inter == 0)
        stdOutput(tr("Итерация = %1   Нет прерывания ОУ").arg(it, 6), tr("Iter = %1   No interruption from RT").arg(it, 6));
        *error = true;
        bNoInt = true;
    }
    */
    if ((status & fl_REG_STATUS_rt_bc_int) == 0 && (noIntFatalRT || checkStatusErrRT))
    {
        stdOutput(tr("Итерация = %1   Нет признака завершения обмена ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')),
                  tr("Iter = %1   No interruption flag in RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')));
        *error = true;
        bNoInt = true;
    }
    if (checkStatusErrRT && codec && ((status & fl_REG_STATUS_rs_err) != 0))
    {
        stdOutput(tr("Итерация = %1   Ошибка кодека в статусе ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')),
                  tr("Iter = %1   Codec error in RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')));
        emit statsOutputReady("errStatusRT", 1);
        *error = true;
    }
    else if ((it < perOutRT && modeOutRT == 0) || (perOutRT != 0 && modeOutRT == 1 && (it%perOutRT) == 0))
    {
            stdOutput(tr("Итерация = %1   Статус ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')),
                      tr("Iter = %1   RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')));
    }
    if (bNoInt)
        emit statsOutputReady("errNoFinRT", 1);
    return bNoInt;
}

varCommandObjToThread::varCommandObjToThread():
    commonObjToThread()
{
}

void varCommandObjToThread::perform()
{
    int errorsBefore = 0;
    if (testData == 0)
    {
        stdOutput(tr("Не удалось сгенерировать данные"), tr("Data generation failed"));
        emit statsOutputReady("otherErr", 1);
        setErrorsBeforeCycle(++errorsBefore);
        return;
    }

    if (codeMask.length() != (MAX_COMMAND_CODE*2+1))
    {
        stdOutput(tr("Некорректно задан список кодов"), tr("Invalid command code set"));
        emit statsOutputReady("otherErr", 1);
        setErrorsBeforeCycle(++errorsBefore);
        return;
    }

    qDebug() << "varCommandObjToThread::perform() started";

    if (connectBC() != AbstractTest::Running)
        return;

    if (connectRT() != AbstractTest::Running)
        return;

    if (initEnable)
    {
        // Начальная конфигурация ОУ
        qDebug() << "start RT configuration";
        devRT->configuration.setChecked(config_NUMREG_cfg, false);
        softReset(devRT);
        QByteArray regRT = devRT->configuration.getRegistersToWrite();
        if (regRT.size() > 0)
        {
            devRT->write_F1(regRT.data(), regRT.size());
            devRT->configuration.doneWriteReg(regRT);
        }
    }
    devRT->writeReg(&devRT->reg_hsc_cfg);
    setRegWritten(devRT, devRT->reg_hsc_cfg);

    if (loadRTA)
    {
        devRT->reg_aux_rtaddr.addr = rtaddr;
        devRT->writeReg(&devRT->reg_aux_rtaddr);
    }

    if (checkRTA) // проверка адреса ОУ
    {
        if (devRT->reg_hsc_cfg.rtavsk_ena == 0)
        {
            // проверяем вспомогательный регистр
            REG_AUX_rtaddr reg;
            devRT->readReg(&reg);
            if (reg.addr != rtaddr)
            {
                stdOutput(tr("Ошибка сравнения адреса ОУ"), tr("Comparison RT address wrong"));
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                setErrorsBeforeCycle(++errorsBefore);
                return;
            }
        }
        // иначе ничего не проверяем, потому что адрес ОУ сидит в конфигурационном регистре, который в этом тесте всегда загружается
    }

    if (initEnable)
    {
        // Начальная конфигурация КШ
        qDebug() << "start BC configuration";
        devBC->configuration.setChecked(config_NUMREG_cfg, false);
        softReset(devBC);
        QByteArray regBC = devBC->configuration.getRegistersToWrite();
        if (regBC.size() > 0)
        {
            QByteArray regs;
            regs.append(regBC);
            devBC->write_F1(regs.data(), regs.size());
            devBC->configuration.doneWriteReg(regBC);
        }
    }

    devBC->writeReg(&devBC->reg_hsc_cfg);
    setRegWritten(devBC, devBC->reg_hsc_cfg);

    if (onPauseStop() == -1)
        return;

    initStartBC();

    CTestBC test;
    test.setConfigFlds(devBC->reg_hsc_cfg.type_man, devBC->reg_hsc_cfg.ena_codec!=0);
    int n_wrd = test.numWordInSymbol();

    int statusRT = getStatusReg(devRT);
    int statusBC = getStatusReg(devBC);

    int it = 0;
    int errCounter = 0;
    int pos = 0;
    char *pData = (char*)testData;

    for (int iter = 0; iter < iterCycle; iter++)
    {
        for (int code = 0; code <= MAX_COMMAND_CODE; code++)
        {
            if (codeMask.at(code*2) == '1')
            {
                for (int num=1; num<=MAXNUMSYM; num++)
                {
                    int num_b = (n_wrd * num - 1) * sizeof(word32_t); // число байт данных в пакете из num символов
                    int trbit = tgca_tr_REC;   // КШ-ОУ
                    {
                        char trmBuf[MAXPACKAGESIZE];
                        bool errorOccured = false;
                        int addr_rx_rt = getBufRec(statusRT);
                        int addr_rx_bc, buf_tr_rt;
                        int buff_rx_rt = statusRT & fl_REG_STATUS_rx_num_buf;

                        if (pos > dataSize)
                            pos -= dataSize;

                        emit statsOutputReady("totalIter", 1);

                        // создание командного пакета и данных для передачи по МКПД
                        if (!test.createCommandPack((void*)trmBuf, MAXPACKAGESIZE, (void*)(pData+pos), num_b, rtaddr, trbit, code))
                        {
                            stdOutput(tr("Ошибка создания командного пакета"), tr("Command pack creation error"));
                            emit statsOutputReady("otherErr", 1);
                            setErrorsWithinCycle(false);
                            return;
                        }
                        // Запись командного пакета в буфер передачи КШ
                        devBC->write_F2(getBufTrm(statusBC), trmBuf, num*NUMBYTEINOFDMSYM);
                        if (checkResponse)
                        {
                            // распишем первое слово буфера приёма КШ, чтобы можно было потом сравнить
                            buf_tr_rt = statusRT & fl_REG_STATUS_tx_num_buf;
                            addr_rx_bc = getBufRec(statusBC);
                            word32_t FF = 0xFFFFFFFF;
                            devBC->write_F2(addr_rx_bc, (char*)(&FF), sizeof(word32_t));
                        }

                        // Оконный режим
                        switchWindow(1);
                        thread()->msleep(delayTime);

                        // Старт обмена
                        devBC->writeReg(&devBC->reg_hsc_creg);
                        int interruption = waitForInterruptionBC(&statusBC, false);

                        // Оконный режим
                        switchWindow(0);

                        statusRT = getStatusReg(devRT);
                        bool bNoIntBC = checkStatusRegBC(statusBC, interruption, it, &errorOccured);
                        bool bNoIntRT = checkStatusRegRT(statusRT, it, &errorOccured);

                        if ((bNoIntBC && noIntFatalBC) || (bNoIntRT && noIntFatalRT > 1))
                        {
                            setErrorsWithinCycle(true);
                            return;
                        }

                        if (compEnableMemRT)
                        {
                            // Чтение данных из буфера приёма ОУ для сравнения
                            QByteArray readArrayC;
                            readArrayC.resize(num*NUMBYTEINOFDMSYM);
                            if (postponeTime > 0)
                                thread()->msleep(postponeTime);
                            devRT->read_F2(addr_rx_rt, num*NUMBYTEINOFDMSYM, readArrayC.data());
                            if (test.cmpPackData((void*)(readArrayC.data()), (void*)(pData+pos), num_b, true))
                            {
                                if (it <= 1)
                                    stdOutput(tr("Сравнение буфера приёма ОУ успешно"), tr("Comparison RT rec buffer OK"));
                            }
                            else
                            {
                                stdOutput(tr("Ошибка сравнения буфера приёма ОУ"), tr("Comparison RT rec buffer wrong"));
                                //stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                                emit statsOutputReady("errCompare", 1);
                                errorOccured = true;
                            }
                        }
                        if (checkReceiving) // здесь проверяем, что буферы переключились, а содержимое буфера проверяли при compEnableMemBC
                        {
                            if ((statusRT & fl_REG_STATUS_rx_num_buf) == buff_rx_rt)
                            {
                                stdOutput(tr("Сообщение не принято оконечным устройством"), tr("RT did not receive package"));
                                emit statsOutputReady("errReceive", 1);
                                errorOccured = true;
                            }
                        }
                        if (checkResponse)
                        {
                            bool errRW = false;
                            // 1) ошибка в регистре состояния
                            if (!checkStatusErrBC && ((statusBC & fl_REG_STATUS_no_aw_err)))
                            {
                                errRW = true;
                                stdOutput(tr("Ошибка неполучения ответа в статусе КШ"), tr("Flag no_aw_err in BC status"));
                            }
                            // 2) переключение буфера приёма КШ и буфера передачи ОУ
                            if (getBufRec(statusBC) == addr_rx_bc)
                            {
                                errRW = true;
                                stdOutput(tr("Ошибка переключения буфера приёма КШ"), tr("BC receiving buffer did not switch"));
                            }
                            if ((statusRT & fl_REG_STATUS_tx_num_buf) == buf_tr_rt)
                            {
                                errRW = true;
                                stdOutput(tr("Ошибка переключения буфера передачи ОУ"), tr("RT transmit buffer did not switch"));
                            }
                            // 3) должно измениться первое слово буфера приёма
                            word32_t comp;
                            devBC->read_F2(addr_rx_bc, sizeof(word32_t), (char*)(&comp));
                            if (checkResponse > 1)
                            {
                                if (getRtaFromResponse(&comp) != rtaddr || (comp&tgca_fl_SW_DATERR) != 0)
                                {
                                    if (getRtaFromResponse(&comp) != rtaddr)
                                        stdOutput(tr("Ошибка ответного слова: адрес ОУ"), tr("Wrong response: RT address"));
                                    if (comp&tgca_fl_SW_DATERR)
                                        stdOutput(tr("Ошибка ответного слова: флаг ошибки данных"), tr("Data error flag in response"));
                                    emit statsOutputReady("errRW", 1);
                                    errorOccured = true;
                                }
                            }
                            else if (comp == 0xFFFFFFFF)
                            {
                                errRW = true;
                                stdOutput(tr("Ошибка содержимого буфера приёма КШ"), tr("BC receiving buffer change content"));
                            }
                            if (errRW)
                            {
                                emit statsOutputReady("errNoSW", 1);
                                errorOccured = true;
                            }
                        }
                        if (errorOccured)
                        {
                            emit statsOutputReady("totalErr", 1);
                            errCounter ++;
                        }
                    }

                    it++;
                    pos += num_b;
                    if (pauseTime > 0)
                        thread()->msleep(pauseTime);
                    if (onPauseStop() == -1)
                        return;

                    trbit = tgca_tr_TRM;   // ОУ-КШ
                    {
                        char trmBuf[MAXPACKAGESIZE];
                        char recBuf[NUMBYTEINOFDMSYM];

                        bool errorOccured = false;
                        int addr_tr_rt = getBufTrm(statusRT);
                        int addr_rx_bc = getBufRec(statusBC);
                        int buff_rx_rt = statusRT & fl_REG_STATUS_rx_num_buf;

                        if (pos > dataSize)
                            pos -= dataSize;

                        emit statsOutputReady("totalIter", 1);

                        // создание командного пакета и данных для передачи по МКПД
                        if (!test.createCommandPack((void*)recBuf, NUMBYTEINOFDMSYM, 0, num_b, rtaddr, trbit, code))
                        {
                            stdOutput(tr("Ошибка создания командного пакета"), tr("Command pack creation error"));
                            emit statsOutputReady("otherErr", 1);
                            setErrorsWithinCycle(false);
                            return;
                        }
                        if (!test.array2Pack((void*)trmBuf, MAXPACKAGESIZE, (void*)(pData+pos), num_b))
                        {
                            stdOutput(tr("Ошибка создания тестового образца данных"), tr("Test data etalon creation error"));
                            emit statsOutputReady("otherErr", 1);
                            setErrorsWithinCycle(false);
                            return;
                        }
                        // Запись командного пакета в буфер передачи КШ
                        devBC->write_F2(getBufTrm(statusBC), recBuf, NUMBYTEINOFDMSYM);
                        // Запись данных в буфер передачи ОУ
                        devRT->write_F2(addr_tr_rt + sizeof(word32_t), trmBuf + sizeof(word32_t), num*NUMBYTEINOFDMSYM - sizeof(word32_t));
                        if (checkResponse)
                        {
                            // распишем первое слово буфера приёма КШ, чтобы можно было потом сравнить
                            word32_t FF = 0xFFFFFFFF;
                            devBC->write_F2(addr_rx_bc, (char*)(&FF), sizeof(word32_t));
                        }

                        // Оконный режим
                        switchWindow(1);
                        thread()->msleep(delayTime);

                        // Старт обмена
                        devBC->writeReg(&devBC->reg_hsc_creg);
                        int interruption = waitForInterruptionBC(&statusBC, false);

                        // Оконный режим
                        switchWindow(0);

                        statusRT = getStatusReg(devRT);
                        bool bNoIntBC = checkStatusRegBC(statusBC, interruption, it, &errorOccured);
                        bool bNoIntRT = checkStatusRegRT(statusRT, it, &errorOccured);

                        if ((bNoIntBC && noIntFatalBC) || (bNoIntRT && noIntFatalRT > 1))
                        {
                            setErrorsWithinCycle(true);
                            return;
                        }

                        // Проверки
                        if (compEnableMemBC || (checkResponse > 0))
                        {
                            // Чтение данных из буфера приёма КШ для сравнения
                            QByteArray readArrayC;
                            readArrayC.resize(num*NUMBYTEINOFDMSYM);
                            devBC->read_F2(addr_rx_bc, num*NUMBYTEINOFDMSYM, readArrayC.data());
                            if (compEnableMemBC)
                            {
                                if (test.cmpPackData((void*)(readArrayC.data()), (void*)(pData+pos), num_b, true))
                                {
                                    if (it <= 1)
                                        stdOutput(tr("Сравнение буфера приёма КШ успешно"), tr("Comparison BC rec buffer OK"));
                                }
                                else
                                {
                                    stdOutput(tr("Ошибка сравнения буфера приёма КШ"), tr("Comparison BC rec buffer wrong"));
                                    //stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                                    emit statsOutputReady("errCompare", 1);
                                    errorOccured = true;
                                }
                            }
                            if (checkResponse)
                            {
                                bool errRW = false;
                                if (!checkStatusErrBC && ((statusBC & fl_REG_STATUS_no_aw_err)))
                                {
                                    errRW = true;
                                    stdOutput(tr("Ошибка неполучения ответа в статусе КШ"), tr("Flag no_aw_err in BC status"));
                                }
                                if (getBufRec(statusBC) == addr_rx_bc)
                                {
                                    errRW = true;
                                    stdOutput(tr("Ошибка переключения буфера приёма КШ"), tr("BC receiving buffer did not switch"));
                                }
                                if (getBufTrm(statusRT) == addr_tr_rt)
                                {
                                    errRW = true;
                                    stdOutput(tr("Ошибка переключения буфера передачи ОУ"), tr("RT transmit buffer did not switch"));
                                }
                                //  первое слово буфера приёма
                                    word32_t comp = (word32_t)(*((word32_t*)(readArrayC.data())));
                                    if (checkResponse > 1)
                                    {
                                        if (getRtaFromResponse(&comp) != rtaddr || (comp&tgca_fl_SW_DATERR) != 0)
                                        {
                                            if (getRtaFromResponse(&comp) != rtaddr)
                                                stdOutput(tr("Ошибка ответного слова: адрес ОУ"), tr("Wrong response: RT address"));
                                            if (comp&tgca_fl_SW_DATERR)
                                                stdOutput(tr("Ошибка ответного слова: флаг ошибки данных"), tr("Data error flag in response"));
                                            emit statsOutputReady("errRW", 1);
                                            errorOccured = true;
                                        }
                                    }
                                    else if (comp == 0xFFFFFFFF)
                                    {
                                        errRW = true;
                                        stdOutput(tr("Ошибка содержимого буфера приёма КШ"), tr("BC receiving buffer change content"));
                                    }
                                if (errRW)
                                {
                                    emit statsOutputReady("errNoSW", 1);
                                    errorOccured = true;
                                }
                            }  // if (checkResponce)
                        }
                        if (checkReceiving) // здесь проверяем, что буфер приёма переключился, а содержимое буфера проверяли при compEnableMemBC
                        {
                            if ((statusRT & fl_REG_STATUS_rx_num_buf) == buff_rx_rt)
                            {
                                stdOutput(tr("Сообщение не принято оконечным устройством"), tr("RT did not receive package"));
                                emit statsOutputReady("errReceive", 1);
                                errorOccured = true;
                            }
                        }
                        if (errorOccured)
                        {
                            emit statsOutputReady("totalErr", 1);
                            errCounter ++;
                        }
                    }
                    it++;
                    pos += num_b;
                    if (pauseTime > 0)
                        thread()->msleep(pauseTime);
                    if (onPauseStop() == -1)
                        return;

                } //length cycle
            } // if (код в списке)
        } // code cycle
    } // iter cycle

    if (errCounter)
        emit resultReady((int)AbstractTest::TestFault);
    else
        emit resultReady((int)AbstractTest::Completed);
}

//lineEditSPIData->setInputMask(QApplication::translate("TestTrmSingle", "HHHH;_", 0));

void varCommandObjToThread::setErrorsBeforeCycle(int errors)
{
    emit resultReady((int)AbstractTest::ErrorIsOccured);
    emit statsOutputReady("errBefore", errors);
    emit statsOutputReady("totalErr", errors);
}

void varCommandObjToThread::setErrorsWithinCycle(bool fatal)
{
    emit resultReady((int)AbstractTest::TestFault);
    if (fatal)
        emit statsOutputReady("errFatal", 1);
    emit statsOutputReady("totalErr", 1);
}

void varCommandObjToThread::destroyData()
{
    if  (testData)
        free (testData);
    testData = 0;
}

/*
void createTrm()
{
    //curThread->trm_size = (test.NumSymOFDM(num_b) + 1) * NUMBYTEINOFDMSYM;

    //int ns = test.NumSymOFDM(num_b) + 1;

    // Создаём пакет передачи данных КШ-ОУ
    // В случае команды передачи ОУ-КШ массив данных, следующий за командным словом,
    // будет использоваться для передачи и сравнения.

    if (curThread->BCtoRT)
        qDebug() << "BC-RT:  sizes " << num_b << " " << test.NumSymOFDM(num_b) + 1 << " " << curThread->trm_size;
    curThread->trmData = malloc(curThread->trm_size);
    if (curThread->trmData == 0)
    {
        message(tr("not enough memory"));
        destroyData(rawData);
        return;
    }
    memset(curThread->trmData, 0, curThread->trm_size);
    if (!test.createCommandPack(curThread->trmData, curThread->trm_size,
                                rawData, num_b, rta, tgca_tr_REC, comboBoxCode->currentIndex()))
    {
        message(tr("Error in createCommandPack() BC-RT !!!"));
        destroyData(rawData);
        curThread->destroyData();
        return;
    }

 }
void createRec()
{
    if (curThread->RTtoBC)  // приём от ОУ или циркулярный возврат
    {
        // Создаём командный пакет на передачу ОУ-КШ
        qDebug() << "RT-BC:  sizes " << num_b << " " << test.NumSymOFDM(num_b) + 1 << " " << NUMBYTEINOFDMSYM;
        curThread->recData = malloc(NUMBYTEINOFDMSYM);
        if (curThread->recData == 0)
        {
            message(tr("not enough memory"));
            destroyData(rawData);
            return;
        }
        memset(curThread->recData, 0, NUMBYTEINOFDMSYM);
        if (!test.createCommandPack(curThread->recData, curThread->trm_size,
                                    0, num_b, rta, tgca_tr_TRM, comboBoxCode->currentIndex()))
        {
            message(tr("Error in createCommandPack() RT-BC !!!"));
            destroyData(rawData);
            curThread->destroyData();
            return;
        }
    }
}
*/
