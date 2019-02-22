#include "varcommtest.h"
#include "../testutil.h"
#include "../ctestbc.h"


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
    statsMap.insert("errBefore", stats->findChild<QLabel*>("errBefore"));
    statsMap.insert("errFatal", stats->findChild<QLabel*>("errFatal"));
    statsMap.insert("otherErr", stats->findChild<QLabel*>("otherErr"));
    connectStats();
}

void VarCommandTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    defineFields();

    load(fName);

    setFieldConnections();

    disableUnused();    // отключить нереализованные настройки

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

    lineEditWinModePause = settings->findChild<QLineEdit*>("lineEditWinModePause");

    comboBoxErrStatusBC = settings->findChild<QComboBox*>("comboBoxErrStatusBC");
    comboBoxErrStatusRT = settings->findChild<QComboBox*>("comboBoxErrStatusRT");
    comboBoxCheckRW = settings->findChild<QComboBox*>("comboBoxCheckRW");
    comboBoxRec = settings->findChild<QComboBox*>("comboBoxRec");
    comboBoxBCIntErr = settings->findChild<QComboBox*>("comboBoxBCIntErr");
    comboBoxRTIntErr = settings->findChild<QComboBox*>("comboBoxRTIntErr");
    comboBoxMemBC = settings->findChild<QComboBox*>("comboBoxMemBC");
    comboBoxMemRT = settings->findChild<QComboBox*>("comboBoxMemRT");
    comboBoxWrongCfgReg = settings->findChild<QComboBox*>("comboBoxWrongCfgReg");

    checkBoxParamView = settings->findChild<QCheckBox*>("checkBoxParamView");
    comboBoxCheckRTA = settings->findChild<QComboBox*>("comboBoxCheckRTA");
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
    checkBoxUseInt->setChecked(!out.readLine().isEmpty());
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
    comboBoxWrongCfgReg->setCurrentIndex(out.readLine().toInt());
    lineEditWinModePause->setText(out.readLine());
    QString genDataLen = out.readLine();
    comboBoxCheckRTA->setCurrentIndex(out.readLine().toInt());

    settingsFile.close();
    dataGen.init(genDataType, genDataBegin, genDataStep, genDataNumStep, genDataUnit, genDataLen);
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
    in << (checkBoxUseInt->isChecked() ? "1" : "") << endl;
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
    in << comboBoxWrongCfgReg->currentIndex() << endl;
    in << lineEditWinModePause->text() << endl;
    in << dataGen.dataLen() << endl;
    in << comboBoxCheckRTA->currentIndex() << endl;

    settingsFile.close();
}

void VarCommandTest::setFieldConnections()
{
}

void VarCommandTest::disableUnused()
{
    // ПОКА НЕ РЕАЛИЗОВАНЫ
    comboBoxCheckRW->setDisabled(true);                             // проверка ответного пакета
    settings->findChild<QLabel*>("labelCheckRW")->setDisabled(true);
    comboBoxRec->setDisabled(true);                                 // проверка, что сообщение от КШ принято ОУ
    settings->findChild<QLabel*>("labelReceived")->setDisabled(true);
    checkBoxParamView->setDisabled(true);
    settings->findChild<QLabel*>("labelParamView")->setDisabled(true);
    checkBoxUseInt->setDisabled(true);
    labelUseInt->setDisabled(true);
}

void VarCommandTest::setEnabledSpecial(bool b)
{
    AbstractTest::setEnabledSpecial(b);

    if (b)
    {
        disableUnused();    // отключить нереализованные настройки
    }
    else
    {
        comboBoxRTA->setEnabled(true);
        checkBoxRTALoad->setEnabled(true);
        checkBoxCodec->setEnabled(true);
        settings->findChild<QLabel*>("labelCodec")->setEnabled(true);
        settings->findChild<QLabel*>("labelRTA")->setEnabled(true);
        settings->findChild<QLabel*>("labelRTARegLoad")->setEnabled(true);
        settings->findChild<QLabel*>("labelHeaderRegisters")->setEnabled(true);

        dataGen.enable(true);

        settings->findChild<QLabel*>("labelHeaderPause")->setEnabled(true);
        settings->findChild<QLabel*>("labelTime")->setEnabled(true);
        settings->findChild<QLabel*>("labelPause")->setEnabled(true);
        lineEditTime->setEnabled(true);
        lineEditPause->setEnabled(true);

/*
        settings->findChild<QLabel*>("labelHeaderConfig")->setEnabled(true);
        settings->findChild<QLabel*>("labelHeaderCommand")->setEnabled(true);
*/
    }
}

void VarCommandTest::startTest()
{
    varCommandObjToThread* curThread = (varCommandObjToThread*)objToThread;

    curThread->useInt = checkBoxUseInt->isChecked() && checkBoxEnaInt->isChecked();
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
    curThread->delayTime = lineEditWinModePause->text().toInt(0, 10);
    curThread->checkStatusErrBC = (comboBoxErrStatusBC->currentIndex() > 0);
    curThread->checkStatusErrRT = (comboBoxErrStatusRT->currentIndex() > 0);
    curThread->noIntFatalBC = (comboBoxBCIntErr->currentIndex() != 0);
    curThread->noIntFatalRT = comboBoxRTIntErr->currentIndex();
    curThread->perOutBC = lineEditBCOut->text().isEmpty() ? 0 : lineEditBCOut->text().toInt(0, 10);
    curThread->perOutRT = lineEditRTOut->text().isEmpty() ? 0 : lineEditRTOut->text().toInt(0, 10);
    curThread->modeOutBC = comboBoxBCOutPref->currentIndex();
    curThread->modeOutRT = comboBoxRTOutPref->currentIndex();
    curThread->initEnable = checkBoxInit->isChecked();
    curThread->checkLoadCfg = comboBoxWrongCfgReg->currentIndex() > 0;

    curThread->manipulation = comboBoxManType->currentText();
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
    curThread->testData = dataGen.createData(curThread->dataSize * 2);

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
    if ((statusBC & fl_REG_STATUS_rt_bc_int) == 0)
    {
        stdOutput(tr("Итерация = %1   Нет признака завершения обмена КШ").arg(it, 6),
                  tr("Iter = %1   No interruption flag in BS status").arg(it, 6));
        *error = true;
        bNoInt = true;
    }

    if ( checkStatusErrBC && ( (codec && ((statusBC & fl_REG_STATUS_rs_err) != 0)) ||
        (statusBC & fl_REG_STATUS_no_aw_err) != 0 || (statusBC & fl_REG_STATUS_yes_aw_gr_err) != 0) )
    {
            stdOutput(QString(tr("Итерация = %1   Ошибка в статусе КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))),
                      QString(tr("Iter = %1   Errors in BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))));
        *error = true;
        emit statsOutputReady("errStatusBC", 1);
    }
    else if (bNoInt)
    {
            stdOutput(QString(tr("Статус КШ: %1").arg(statusBC, 4, 16, QLatin1Char('0'))),
                      QString(tr("BC status: %1").arg(statusBC, 4, 16, QLatin1Char('0'))));
    }
    else if ((it < perOutBC && modeOutBC == 0) || (perOutBC != 0 && modeOutBC == 1 && (it%perOutBC) == 0))
    {
            stdOutput(QString(tr("Итерация = %1   Статус КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))),
                      QString(tr("Iter = %1   BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))));
    }
    if (bNoInt)
        emit statsOutputReady("errNoFinBC", 1);

    return bNoInt;
}

bool varCommandObjToThread::checkStatusRegRT(int status, int it, bool *error)
{
    bool bNoInt = false;

    if (useInt && noIntFatalRT)
    {
        devRT->readReg(&devRT->reg_aux_interruption);
        if (devRT->reg_aux_interruption.inter == 0)
        stdOutput(tr("Итерация = %1   Нет прерывания ОУ").arg(it, 6), tr("Iter = %1   No interruption from RT").arg(it, 6));
        *error = true;
        bNoInt = true;
    }
    if ((status & fl_REG_STATUS_rt_bc_int) == 0 && (noIntFatalRT || checkStatusErrRT))
    {
        stdOutput(QString(tr("Итерация = %1   Нет признака завершения обмена ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))),
                  QString(tr("Iter = %1   No interruption flag in RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))));
        *error = true;
        bNoInt = true;
    }
    if (checkStatusErrRT && codec && ((status & fl_REG_STATUS_rs_err) != 0))
    {
        stdOutput(QString(tr("Итерация = %1   Есть ошибки в статусе ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))),
                  QString(tr("Iter = %1   Error flags in RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))));
        emit statsOutputReady("errStatusRT", 1);
        *error = true;
    }
    else if ((it < perOutRT && modeOutRT == 0) || (perOutRT != 0 && modeOutRT == 1 && (it%perOutRT) == 0))
    {
            stdOutput(QString(tr("Итерация = %1   Статус ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))),
                      QString(tr("Iter = %1   RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))));
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
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }

        qDebug() << "varCommandObjToThread::perform() started";

        if (connectBC() != AbstractTest::Running)
            return;
        qDebug() << tr("BC connected");

        if (connectRT() != AbstractTest::Running)
            return;
        qDebug() << tr("RT connected");

        if (initEnable)
        {
            // Начальная конфигурация ОУ
            qDebug() << "start RT configuration";
            devRT->configuration.setChecked(config_NUMREG_cfg, false);
            QByteArray regRT = devRT->configuration.getRegistersToWrite();
            if (regRT.size() > 0)
            {
                devRT->write_F1(regRT.data(), regRT.size());
                devRT->configuration.doneWriteReg(regRT);
            }
        }
        devRT->writeReg(&devRT->reg_hsc_cfg);
        //setRegWritten(devRT, devRT->reg_hsc_cfg);
        devRT->configuration.setWritten(devRT->reg_hsc_cfg.address, devRT->reg_hsc_cfg.getData());

        if (checkLoadCfg && devRT->reg_hsc_cfg.getData() != readRegVal(devRT, &devRT->reg_hsc_cfg))
        {
            stdOutput(tr("Ошибка сравнения конфигурационного регистра ОУ"), tr("Comparison RT cfg register wrong"));
            emit statsOutputReady("errCompare", 1);
            setErrorsBeforeCycle(++errorsBefore);
            return;
        }

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
            else if (!checkLoadCfg)
            {
                REG_HSC_cfg cfg;
                devRT->readReg(&cfg);
                if (cfg.rtavsk_ena == 0)
                {
                    stdOutput(tr("Ошибка конфигурационного регистра ОУ"), tr("Invalid RT configuration register"));
                    if (cfg.rtavsk != rtaddr)
                        stdOutput(tr("Ошибка сравнения адреса ОУ"), tr("Comparison RT address wrong"));
                    errorsBefore++;
                    emit statsOutputReady("errCompare", 1);
                    setErrorsBeforeCycle(++errorsBefore);
                    return;
                }
                if (cfg.rtavsk != rtaddr)
                {
                    stdOutput(tr("Ошибка сравнения адреса ОУ"), tr("Comparison RT address wrong"));
                    errorsBefore++;
                    emit statsOutputReady("errCompare", 1);
                    setErrorsBeforeCycle(++errorsBefore);
                    return;
                }
            }
        }

        if (initEnable)
        {
            // Начальная конфигурация КШ
            qDebug() << "start BC configuration";
            devBC->configuration.setChecked(config_NUMREG_cfg, false);
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
        //setRegWritten(devBC, devBC->reg_hsc_cfg);
        devBC->configuration.setWritten(devBC->reg_hsc_cfg.address, devBC->reg_hsc_cfg.getData());
        if (checkLoadCfg && devBC->reg_hsc_cfg.getData() != readRegVal(devBC, &devBC->reg_hsc_cfg))
        {
            stdOutput(tr("Ошибка сравнения конфигурационного регистра КШ"), tr("Comparison BC cfg register wrong"));
            emit statsOutputReady("errCompare", 1);
            setErrorsBeforeCycle(++errorsBefore);
            return;
        }

        if (pause_stop() == -1)
            return;

        quint32 zero = 0;
        devBC->reg_hsc_creg.setData(zero);
        devBC->reg_hsc_creg.start_bc = 1;

        CTestBC test;
        test.setConfigFlds(devBC->reg_hsc_cfg.type_man, devBC->reg_hsc_cfg.ena_codec!=0);
        int n_wrd = test.numWordInSymbol();

        int statusRT = getStatusReg(devRT);
        int statusBC = getStatusReg(devBC);

        int it = 0;
        int errCounter = 0;
        int pos = 0;
        char * pData = (char*)testData;

        for (int iter = 0; iter < iterCycle; iter++)
        {
            for (int code = 0; code < 64; code++)
            {
                for (int num=1; num<=MAXNUMSYM; num++)
                {
                    int num_b = (n_wrd * num - 1) * sizeof(word32_t); // число байт данных в пакете из num символов
                    int trbit = tgca_tr_REC;   // КШ -> ОУ
                    {
                        char trmBuf[MAXPACKAGESIZE];
                        bool errorOccured = false;
                        int addr_rx = getBufRec(statusRT);

                        if (pos > dataSize)
                            pos -= dataSize;

                        emit statsOutputReadySimple("totalIter", 1);

                        // создание командного пакета и данных для передачи по МКПД
                        if (!test.createCommandPack((void*)trmBuf, MAXPACKAGESIZE, (void*)(pData+pos), num_b, rtaddr, trbit, code))
                        {
                            emit statsOutputReady("otherErr", 1);
                            setErrorsWithinCycle(false);
                            stdOutput(QString(tr("Ошибка создания командного пакета")), QString(tr("Command pack creation error")));
                            return;
                        }
                        // Запись командного пакета в буфер передачи КШ
                        devBC->write_F2(getBufTrm(statusBC), trmBuf, num*NUMBYTEINOFDMSYM);

                        // Оконный режим
                        switchWindow(1);
                        thread()->msleep(delayTime);
                        // Старт обмена
                        devBC->writeReg(&devBC->reg_hsc_creg);

                        int interruption = waitForInterruption(devBC, useInt, waitTime, &statusBC);
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
                            devRT->read_F2(addr_rx, num*NUMBYTEINOFDMSYM, readArrayC.data());
                            if (test.cmpPackData((void*)(readArrayC.data()), (void*)(pData+pos), num_b, true))
                            {
                                if (it <= 1)
                                    stdOutput(tr("Сравнение буфера приёма ОУ успешно"), tr("Comparison RT rec buffer OK"));
                            }
                            else
                            {
                                stdOutput(tr("Ошибка сравнения буфера приёма ОУ"), tr("Comparison RT rec buffer wrong"));
                                stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                                emit statsOutputReady("errCompare", 1);
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
                    if (pause_stop() == -1)
                        return;

                    trbit = tgca_tr_TRM;   // ОУ -> КШ
                    {
                        char trmBuf[MAXPACKAGESIZE];
                        char recBuf[NUMBYTEINOFDMSYM];

                        bool errorOccured = false;
                        int addr_tr_rt = getBufTrm(statusRT);
                        int addr_rx_bc = getBufRec(statusBC);

                        if (pos > dataSize)
                            pos -= dataSize;

                        emit statsOutputReadySimple("totalIter", 1);

                        // создание командного пакета и данных для передачи по МКПД
                        if (!test.createCommandPack((void*)recBuf, NUMBYTEINOFDMSYM, 0, 0, rtaddr, trbit, code))
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

                        // Оконный режим
                        switchWindow(1);
                        thread()->msleep(delayTime);

                        // Старт обмена
                        devBC->writeReg(&devBC->reg_hsc_creg);

                        int interruption = waitForInterruption(devBC, useInt, waitTime, &statusBC);
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

                        if (compEnableMemBC)
                        {
                            // Чтение данных из буфера приёма ОУ для сравнения
                            QByteArray readArrayC;
                            readArrayC.resize(num*NUMBYTEINOFDMSYM);
                            devBC->read_F2(addr_rx_bc, num*NUMBYTEINOFDMSYM, readArrayC.data());
                            if (test.cmpPackData((void*)(readArrayC.data()), (void*)(pData+pos), num_b, true))
                            {
                                if (it <= 1)
                                    stdOutput(tr("Сравнение буфера приёма КШ успешно"), tr("Comparison BC rec buffer OK"));
                            }
                            else
                            {
                                stdOutput(tr("Ошибка сравнения буфера приёма КШ"), tr("Comparison BC rec buffer wrong"));
                                stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                                emit statsOutputReady("errCompare", 1);
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
                    if (pause_stop() == -1)
                        return;

                } //length cycle
            } // code cycle
        } // iter cycle

        if (errCounter)
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        else
            emit resultReady((int)AbstractTest::Completed);
#if 0
                    devBC->write_F2(getBufTrm(statusBC), (char*)recData, rec_pk_size);
                    int addr_rx = getBufRec(statusBC);

                    if (BCtoRT)
                        stdOutput(QString(tr("Передача")), QString(tr("Transmition command")));

                    // Оконный режим
                    switchWindow(1);

                    // Старт обмена
                    devBC->writeReg(&devBC->reg_hsc_creg);
                    if (!BCtoRT)
                        emit statsOutputReadySimple("totalIter", 1);

                    int interruption = waitForInterruption(devBC, useInt, waitTime, &statusBC);
                    switchWindow(0);

                    if (devRT)
                        statusRT = getStatusReg(devRT);

                    bool bNoIntBC = checkStatusRegBC(statusBC, interruption, it, &errorOccured);
                    bool bNoIntRT = false;

                    if (devRT)
                        bNoIntRT = checkStatusRegRT(statusRT, it, &errorOccured);

                    if ((bNoIntBC && noIntFatalBC) || (bNoIntRT && noIntFatalRT > 1))
                    {
                        setErrorsWithinCycle(true);
                        return;
                    }

                    checkCounters(devBC);

                    if (devRT)
                    {
                        checkCounters(devRT);

                        if ((compEnableMemBC > 0) || (compEnableData > 0 /*&& BCtoRT*/) /* || проверка ответного слова */)
                        {
                            // Чтение данных из буфера приёма КШ для сравнения
                            QByteArray readArrayC;
                            readArrayC.resize(trm_size);
                            devBC->read_F2(addr_rx, trm_size, readArrayC.data());
                            if (cmpPack(readArrayC.data(), (char*)trmData, trm_size, nwrd*sizeof(word32_t), true))
                            {
                                if (it <= 1)
                                    stdOutput(tr("Сравнение буфера приёма КШ успешно"), tr("Comparison BC rec buffer OK"));
                            }
                            else
                            {
                                stdOutput(tr("Ошибка сравнения буфера приёма КШ"), tr("Comparison BC rec buffer wrong"));
                                stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                                stdOutput(tr("Первое прочитанное слово = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')),
                                          tr("First read word = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')));
                                emit statsOutputReady("errCompare", 1);
                                if (compEnableMemRT == 2)
                                {
                                    setErrorsWithinCycle(false);
                                    return;
                                }
                            }
                        } //сравнение данных
                    }
                } // передача ОУ->КШ или циркулярный возврат

                if (errorOccured)
                    emit statsOutputReady("totalErr", 1);

                if (pauseTime > 0)
                    thread()->msleep(pauseTime);
            } // цикл

        } // devBC
#endif
}

void varCommandObjToThread::terminate(int )
{
    if (!isRunning())  // (fl == AbstractTest::ErrorIsOccured || fl == AbstractTest::Completed || fl == AbstractTest::Stopped)
    {
        if (tcpSocketBC.state() == QAbstractSocket::ConnectedState)
            tcpSocketBC.abort();
        if (tcpSocketRT.state() == QAbstractSocket::ConnectedState)
            tcpSocketRT.abort();
        this->destroyData();
    }
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
    emit resultReady((int)AbstractTest::ErrorIsOccured);
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

// Оконный режим
void varCommandObjToThread::switchWindow(int n)
{
    devRT->reg_aux_winmode.mode = n;
    devBC->reg_aux_winmode.mode = n;
    devRT->writeReg(&devRT->reg_aux_winmode);
    devBC->writeReg(&devBC->reg_aux_winmode);
}
/*
void createTrm()
{
    //curThread->trm_size = (test.NumSymOFDM(num_b) + 1) * NUMBYTEINOFDMSYM;

    //int ns = test.NumSymOFDM(num_b) + 1;

    // Создаём пакет передачи данных КШ-ОУ
    // В случае команды передачи ОУ->КШ массив данных, следующий за командным словом,
    // будет использоваться для передачи и сравнения.

    if (curThread->BCtoRT)
        qDebug() << "BC->RT:  sizes " << num_b << " " << test.NumSymOFDM(num_b) + 1 << " " << curThread->trm_size;
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
        message(tr("Error in createCommandPack() BC->RT !!!"));
        destroyData(rawData);
        curThread->destroyData();
        return;
    }

 }
void createRec()
{
    if (curThread->RTtoBC)  // приём от ОУ или циркулярный возврат
    {
        // Создаём командный пакет на передачу ОУ->КШ
        qDebug() << "RT->BC:  sizes " << num_b << " " << test.NumSymOFDM(num_b) + 1 << " " << NUMBYTEINOFDMSYM;
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
            message(tr("Error in createCommandPack() RT->BC !!!"));
            destroyData(rawData);
            curThread->destroyData();
            return;
        }
    }
}
*/
