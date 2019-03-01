#include "varbroadtest.h"
#include "../testutil.h"
#include "../ctestbc.h"

void VarBroadTest::setStatSettings()
{
    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("totalErr", stats->findChild<QLabel*>("totalErr"));
    statsMap.insert("errYesSW", stats->findChild<QLabel*>("errYesSW"));
    statsMap.insert("errNoFinBC", stats->findChild<QLabel*>("errNoFinBC"));
    statsMap.insert("errCompare", stats->findChild<QLabel*>("errCompare"));
    statsMap.insert("errStatusRT", stats->findChild<QLabel*>("errStatusRT"));
    statsMap.insert("errStatusBC", stats->findChild<QLabel*>("errStatusBC"));
    statsMap.insert("errBefore", stats->findChild<QLabel*>("errBefore"));
    statsMap.insert("errFatal", stats->findChild<QLabel*>("errFatal"));
    statsMap.insert("otherErr", stats->findChild<QLabel*>("otherErr"));
    connectStatisticSlots();
}

void VarBroadTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    defineFields();

    load(fName);

    setFieldConnections();
    setTestConnections();

    disableUnimplemented();    // отключить нереализованные настройки

    deviceLineEditList.append(lineEditDevBC);
    deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability(3);

    objToThread = new varBroadObjToThread();
    connectThread();

    setStatSettings();

    testThread.start();
}

void VarBroadTest::defineFields()
{
    dataGen.settings();

    lineEditTime = settings->findChild<QLineEdit*>("lineEditTime");
    lineEditPause = settings->findChild<QLineEdit*>("lineEditPause");

    comboBoxManType = settings->findChild<QComboBox*>("comboBoxManType");

    checkBoxCodec = settings->findChild<QCheckBox*>("checkBoxCodec");
    checkBoxEnaInt = settings->findChild<QCheckBox*>("checkBoxEnaInt");
    checkBoxUseInt = settings->findChild<QCheckBox*>("checkBoxUseInt");
    labelUseInt = settings->findChild<QLabel*>("labelUseInt");
    checkBoxOut = settings->findChild<QCheckBox*>("checkBoxOut");
    checkBoxInit = settings->findChild<QCheckBox*>("checkBoxInit");

    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");

    lineEditCycle = settings->findChild<QLineEdit*>("lineEditCycle");

    checkBoxBCOut = settings->findChild<QCheckBox*>("checkBoxBCOut");
    checkBoxRTOut = settings->findChild<QCheckBox*>("checkBoxRTOut");
    comboBoxBCOutPref = settings->findChild<QComboBox*>("comboBoxBCOutPref");
    comboBoxRTOutPref = settings->findChild<QComboBox*>("comboBoxRTOutPref");
    lineEditBCOut = settings->findChild<QLineEdit*>("lineEditBCOut");
    lineEditRTOut = settings->findChild<QLineEdit*>("lineEditRTOut");

    lineEditReservePause = settings->findChild<QLineEdit*>("lineEditReservePause");

    comboBoxErrStatusBC = settings->findChild<QComboBox*>("comboBoxErrStatusBC");
    comboBoxErrStatusRT = settings->findChild<QComboBox*>("comboBoxErrStatusRT");
    comboBoxRec = settings->findChild<QComboBox*>("comboBoxRec");
    comboBoxBCIntErr = settings->findChild<QComboBox*>("comboBoxBCIntErr");
    comboBoxMemRT = settings->findChild<QComboBox*>("comboBoxMemRT");

    checkBoxParamView = settings->findChild<QCheckBox*>("checkBoxParamView");

    pushButtonCodes = settings->findChild<QPushButton*>("pushButtonCodes");
 }

void VarBroadTest::load(QString fName)
{
top_1

    comboBoxManType->setCurrentText(out.readLine());
    checkBoxCodec->setChecked(!out.readLine().isEmpty());
    checkBoxEnaInt->setChecked(!out.readLine().isEmpty());

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
    comboBoxBCOutPref->setCurrentText(out.readLine());
    comboBoxRTOutPref->setCurrentText(out.readLine());

    lineEditBCOut->setText(out.readLine());
    lineEditRTOut->setText(out.readLine());

    comboBoxErrStatusBC->setCurrentIndex(out.readLine().toInt());
    comboBoxErrStatusRT->setCurrentIndex(out.readLine().toInt());
    comboBoxRec->setCurrentIndex(out.readLine().toInt());
    comboBoxBCIntErr->setCurrentIndex(out.readLine().toInt());
    comboBoxMemRT->setCurrentIndex(out.readLine().toInt());
    lineEditReservePause->setText(out.readLine());

    QString genDataLen = out.readLine();

    checkBoxBCOut->setChecked(!out.readLine().isEmpty());
    checkBoxRTOut->setChecked(!out.readLine().isEmpty());

    settingsFile.close();
    dataGen.init(genDataType, genDataBegin, genDataStep, genDataNumStep, genDataUnit, genDataLen);
}

void VarBroadTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)

    in << comboBoxManType->currentText() << endl;
    in << (checkBoxCodec->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaInt->isChecked() ? "1" : "") << endl;

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

    in << comboBoxBCOutPref->currentText() << endl;
    in << comboBoxRTOutPref->currentText() << endl;
    in << lineEditBCOut->text() << endl;
    in << lineEditRTOut->text() << endl;

    in << comboBoxErrStatusBC->currentIndex() << endl;
    in << comboBoxErrStatusRT->currentIndex() << endl;
    in << comboBoxRec->currentIndex() << endl;
    in << comboBoxBCIntErr->currentIndex() << endl;
    in << comboBoxMemRT->currentIndex() << endl;
    in << lineEditReservePause->text() << endl;

    in << dataGen.dataLen() << endl;
    in << (checkBoxBCOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTOut->isChecked() ? "1" : "") << endl;

    settingsFile.close();
}

void VarBroadTest::setFieldConnections()
{
    connect(checkBoxUseInt, SIGNAL(clicked()), this, SLOT(onCheckUseInt()));
    connect(checkBoxEnaInt, SIGNAL(clicked()), this, SLOT(onCheckEnaInt()));
    connect(pushButtonCodes, SIGNAL(clicked()), this, SLOT(onPushCodes()));
}

void VarBroadTest::onPushCodes()
{
}

void VarBroadTest::onCheckUseInt()
{
    qDebug() << "onCheckUseInt() " << checkBoxUseInt->checkState();
    if (checkBoxUseInt->checkState() == Qt::PartiallyChecked)
        checkBoxUseInt->setCheckState(Qt::Checked);
}

void VarBroadTest::onCheckEnaInt()
{
    qDebug() << "onCheckEnaInt() " << checkBoxEnaInt->isChecked();
    checkBoxUseInt->setEnabled(checkBoxEnaInt->isChecked());
    if (checkBoxUseInt->checkState() == Qt::PartiallyChecked)
        checkBoxUseInt->setCheckState(Qt::Checked);
    else if (checkBoxUseInt->checkState() == Qt::Checked)
        checkBoxUseInt->setCheckState(Qt::PartiallyChecked);
}

void VarBroadTest::disableUnimplemented()
{
    // ПОКА НЕ РЕАЛИЗОВАНЫ
    comboBoxRec->setDisabled(true);                                 // проверка, что сообщение от КШ принято ОУ
    settings->findChild<QLabel*>("labelReceived")->setDisabled(true);
    checkBoxParamView->setDisabled(true);
    settings->findChild<QLabel*>("labelParamView")->setDisabled(true);
}

void VarBroadTest::setEnabledSpecial(bool b)
{
    AbstractTest::setEnabledSpecial(b);

    if (b)
    {
        disableUnimplemented();    // отключить нереализованные настройки
    }
    else
    {
        checkBoxCodec->setEnabled(true);
        settings->findChild<QLabel*>("labelCodec")->setEnabled(true);
        settings->findChild<QLabel*>("labelHeaderConfig")->setEnabled(true);

//        dataGen.enable(true);

        settings->findChild<QLabel*>("labelHeaderPause")->setEnabled(true);
        settings->findChild<QLabel*>("labelTime")->setEnabled(true);
        settings->findChild<QLabel*>("labelPause")->setEnabled(true);
        lineEditTime->setEnabled(true);
        lineEditPause->setEnabled(true);
    }
}

void VarBroadTest::startTest()
{
    varBroadObjToThread* curThread = (varBroadObjToThread*)objToThread;

    curThread->useInt = ((checkBoxUseInt->checkState() == Qt::Checked) && checkBoxEnaInt->isChecked()); // напоминание: загрузка cfg_reg в этом тесте всегда включена
    curThread->setOutEnabled(checkBoxOut->isChecked());
    curThread->waitTime = lineEditTime->text().toInt(0, 16);

    curThread->compEnableMemRT = (comboBoxMemRT->currentIndex() > 0);
    curThread->iterCycle = lineEditCycle->text().toInt(0, 10);
    if (curThread->iterCycle <= 0)
        curThread->iterCycle = 1;
    curThread->pauseTime = lineEditPause->text().toInt(0, 10);
    curThread->postponeTime = lineEditReservePause->text().toInt(0, 10);
    curThread->checkStatusErrBC = (comboBoxErrStatusBC->currentIndex() > 0);
    curThread->checkStatusErrRT = (comboBoxErrStatusRT->currentIndex() > 0);
    curThread->noIntFatalBC = (comboBoxBCIntErr->currentIndex() != 0);
    curThread->statusBCOut = checkBoxBCOut->isChecked();
    curThread->statusRTOut = checkBoxRTOut->isChecked();
    curThread->perOutBC = lineEditBCOut->text().isEmpty() ? 0 : lineEditBCOut->text().toInt(0, 10);
    curThread->perOutRT = lineEditRTOut->text().isEmpty() ? 0 : lineEditRTOut->text().toInt(0, 10);
    curThread->modeOutBC = comboBoxBCOutPref->currentIndex();
    curThread->modeOutRT = comboBoxRTOutPref->currentIndex();
    curThread->initEnable = checkBoxInit->isChecked();
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

    curThread->dataSize = dataGen.getDataLen();
    int dataSizeRequired = MAXNUMSYM * calcNumWordInSymbol(comboBoxManType->currentIndex(), curThread->codec?1:0) - sizeof(word32_t);
    if (curThread->dataSize < dataSizeRequired)
        curThread->dataSize = dataSizeRequired;
    curThread->testData = dataGen.createData(curThread->dataSize, 2);

    emit startTestTh();
}

bool varBroadObjToThread::checkStatusRegBC(int statusBC, int interruption, int it, bool *error)
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
    if ((statusBC & fl_REG_STATUS_rx_num_buf) != buf_rx_bc)
    {
        buf_rx_bc = statusBC & fl_REG_STATUS_rx_num_buf;
        stdOutput(tr("Неожиданное изменение активного буфера приёма КШ"), tr("BC rec. buffer unexpectedly changed"));
    }

    if ( checkStatusErrBC && ( (/*codec && */((statusBC & fl_REG_STATUS_rs_err) != 0)) ||
        (statusBC & fl_REG_STATUS_no_aw_err) != 0 || (statusBC & fl_REG_STATUS_yes_aw_gr_err) != 0) )
    {
            stdOutput(tr("Итерация = %1   Есть ошибки в статусе КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')),
                      tr("Iter = %1   Errors in BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')));
        *error = true;
        emit statsOutputReady("errStatusBC", 1);
        QString rus, eng;
        if (statusBC & fl_REG_STATUS_rs_err)
        {
            rus = tr("       ") + tr("  Ошибка кодека");
            eng = tr("       ") + tr("  Codec error");
        }
        if (statusBC & fl_REG_STATUS_no_aw_err)
        {
            if (rus.isEmpty())
            {
                rus = tr("       Н");
                eng = tr("       U");
            }
            else
            {
                rus = tr(",  н");
                eng = tr(",  u");
            }
            rus += tr("еуместный флаг ошибки");
            eng += tr("nsuitable error flag");
        }
        if (!rus.isEmpty())
            stdOutput(rus, eng);
        if (statusBC & fl_REG_STATUS_yes_aw_gr_err)
        {
            emit statsOutputReady("errYesSW", 1);
            stdOutput(tr("Признак получения ответа от групповой команды"), tr("Response flag for broadcast command"));
        }
    }
    else if ((!checkStatusErrBC) && (statusBC & fl_REG_STATUS_yes_aw_gr_err) != 0)
    {
        *error = true;
        emit statsOutputReady("errYesSW", 1);
        stdOutput(tr("Итерация = %1   Ошибка в статусе КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')),
                  tr("Iter = %1   Error in BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')));
        stdOutput(tr("Признак получения ответа от групповой команды"), tr("Response flag for broadcast command"));
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

bool varBroadObjToThread::checkStatusRegRT(int status, int it, bool *error)
{
    bool bNoInt = false;

    if ((status & fl_REG_STATUS_rt_bc_int) == 0 && checkStatusErrRT)
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
    return bNoInt;
}

varBroadObjToThread::varBroadObjToThread():
    commonObjToThread()
{
}

void varBroadObjToThread::perform()
{
    int errorsBefore = 0;
    if (testData == 0)
    {
        stdOutput(tr("Не удалось сгенерировать данные"), tr("Data generation failed"));
        //emit resultReady((int)AbstractTest::ErrorIsOccured);
        emit statsOutputReady("otherErr", 1);
        setErrorsBeforeCycle(++errorsBefore);
        return;
    }

    qDebug() << "varBroadObjToThread::perform() started";

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
    setRegWritten(devRT, devRT->reg_hsc_cfg);

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
    setRegWritten(devBC, devBC->reg_hsc_cfg);

    initStartBC();

    CTestBC test;
    test.setConfigFlds(devBC->reg_hsc_cfg.type_man, devBC->reg_hsc_cfg.ena_codec!=0);
    int n_wrd = test.numWordInSymbol();

    int statusRT = getStatusReg(devRT);
    int statusBC = getStatusReg(devBC);
    buf_rx_bc = statusBC & fl_REG_STATUS_rx_num_buf;
    buf_tx_rt = statusRT & fl_REG_STATUS_tx_num_buf;

    int it = 0;
    int errCounter = 0;
    int pos = 0;
    char *pData = (char*)testData;

    if (pause_stop() == -1)
        return;

    for (int iter = 0; iter < iterCycle; iter++)
    {
        for (int code = 0; code < 64; code++)
        {
            for (int num=1; num<=MAXNUMSYM; num++)
            {
                char trmBuf[MAXPACKAGESIZE];
                int num_b = (n_wrd * num - 1) * sizeof(word32_t); // число байт данных в пакете из num символов
                bool errorOccured = false;
                int addr_rx_rt = getBufRec(statusRT);

                if (pos > dataSize)
                    pos -= dataSize;

                emit statsOutputReady("totalIter", 1);

                // создание командного пакета с данными для передачи по МКПД КШ-ОУ
                if (!test.createCommandPack((void*)trmBuf, MAXPACKAGESIZE, (void*)(pData+pos), num_b, BRD_RT_ADDR, tgca_tr_REC, code))
                {
                    stdOutput(tr("Ошибка создания командного пакета"), tr("Command pack creation error"));
                    emit statsOutputReady("otherErr", 1);
                    setErrorsWithinCycle(false);
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
                checkStatusRegRT(statusRT, it, &errorOccured);

                if (bNoIntBC && noIntFatalBC)
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
                            stdOutput(tr("Итерация %1: сравнение буфера приёма ОУ успешно").arg(it), tr("Iter %1: comparison RT rec buffer OK").arg(it));
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
}

//lineEditSPIData->setInputMask(QApplication::translate("TestTrmSingle", "HHHH;_", 0));

void varBroadObjToThread::setErrorsBeforeCycle(int errors)
{
    emit resultReady((int)AbstractTest::ErrorIsOccured);
    emit statsOutputReady("errBefore", errors);
    emit statsOutputReady("totalErr", errors);
}

void varBroadObjToThread::setErrorsWithinCycle(bool fatal)
{
    emit resultReady((int)AbstractTest::ErrorIsOccured);
    if (fatal)
        emit statsOutputReady("errFatal", 1);
    emit statsOutputReady("totalErr", 1);
}

void varBroadObjToThread::destroyData()
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
