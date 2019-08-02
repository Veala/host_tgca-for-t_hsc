
#include "noisetest.h"
#include "../testutil.h"
#include "../ctestbc.h"

void NoiseTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    defineFields();
    load(fName);

    setFieldConnections();
    setTestConnections();
    onRadioCycle();
    disableUnimplemented();
    checkBoxParamView->setVisible(false);
    settings->findChild<QLabel*>("labelParamView")->setVisible(false);

    deviceLineEditList.append(lineEditDevBC);
    deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability(3);

    objToThread = new noiseObjToThread();

    connectThread();

    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("totalBit", stats->findChild<QLabel*>("totalBit"));
    statsMap.insert("errPack", stats->findChild<QLabel*>("errPack"));
    statsMap.insert("errBit", stats->findChild<QLabel*>("errBit"));
    statsMap.insert("otherErr", stats->findChild<QLabel*>("otherErr"));
    statsMap.insert("errConfig", stats->findChild<QLabel*>("errConfig"));
    connectStatisticSlots();
    connect(objToThread,SIGNAL(statsOutputReadyLongLong(QString,quint64)), this, SLOT(statsLongLongOutput(QString,quint64)));

    testThread.start();
}

void NoiseTest::statsLongLongOutput(QString str, quint64 n)
{
    QMap<QString, QLabel*>::iterator it = statsMap.find(str);
    if (it != statsMap.end())
    {
        QLabel* l = it.value();
        l->setText(QString::number(l->text().toULongLong()+n));
    }
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
    lineEditReservePause = settings->findChild<QLineEdit*>("lineEditReservePause");
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

void NoiseTest::setEnabledSpecial(bool b)
{
    AbstractTest::setEnabledSpecial(b);

    if (b)
    {
        onRadioCycle();
        disableUnimplemented();    // отключаются нереализованные настройки
    }
    else
    {
        comboBoxRTA->setEnabled(true);
        checkBoxCodec->setEnabled(true);

        settings->findChild<QLabel*>("labelHeaderCommand")->setEnabled(true);
        settings->findChild<QLabel*>("labelRTA")->setEnabled(true);
        settings->findChild<QLabel*>("labelNumS")->setEnabled(true);
        comboBoxRTA->setEnabled(true);
        lineEditNumS->setEnabled(true);

        settings->findChild<QLabel*>("labelHeaderConfig")->setEnabled(true);
        settings->findChild<QLabel*>("labelCodec")->setEnabled(true);
        settings->findChild<QLabel*>("labelManType")->setEnabled(true);
        comboBoxManType->setEnabled(true);
        checkBoxCodec->setEnabled(true);

        settings->findChild<QLabel*>("labelHeaderTest")->setEnabled(true);
        settings->findChild<QLabel*>("labelTime")->setEnabled(true);
        settings->findChild<QLabel*>("labelPause")->setEnabled(true);
        settings->findChild<QLabel*>("labelSpeed")->setEnabled(true);
        settings->findChild<QLabel*>("labelOver")->setEnabled(true);
        lineEditTime->setEnabled(true);
        lineEditPause->setEnabled(true);
        comboBoxSpeed->setEnabled(true);
        lineEditOver->setEnabled(true);

        dataGen.enable(true);
    }
}

void NoiseTest::setFieldConnections()
{
    connect(radioButtonSingle, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(radioButtonUnlimited, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(radioButtonEnter, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
}


void NoiseTest::onRadioCycle()
{
    if (!userMode())
        spinBoxCycle->setEnabled(radioButtonEnter->isChecked());
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
    in << spinBoxCycle->value() << endl;
    in << (radioButtonEnter->isChecked() ? 0 : (radioButtonSingle->isChecked() ? 1 : 2)) << endl;
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
    in << lineEditReservePause->text() << endl;
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
    comboBoxCheckRTA->setCurrentIndex(out.readLine().toInt());
    // устройства
    lineEditDevBC->setText(out.readLine());
    lineEditDevRT->setText(out.readLine());
    // паузы
    lineEditReservePause->setText(out.readLine());
    lineEditTime->setText(out.readLine());
    lineEditPause->setText(out.readLine());
    // прерывание
    checkBoxUseInt->setChecked(!out.readLine().isEmpty());
}

void NoiseTest::startTest()
{
    noiseObjToThread* curThread = (noiseObjToThread*)objToThread;

    curThread->trbit = 1 - comboBoxTestType->currentIndex();
    curThread->useInt = checkBoxUseInt->isChecked() && (checkBoxEnaInt->isChecked() || !checkBoxConfRegLoad->isChecked());
        // Если загрузка cfg_reg выключена, то теоретически может быть загружен флаг en_rt_bc_int. Если проверка cfg_reg
        // включена и в реальном регистре флаг en_rt_bc_int равен 0, то режим useInt автоматически выключится.
        // Иначе за противоречия в настройках отвечает пользователь.
    if (checkBoxUseInt->isChecked() && !checkBoxEnaInt->isChecked() && checkBoxConfRegLoad->isChecked())
    {
        message(tr("Использование прерывание КШ не разрешено"));
    }
    curThread->setOutEnabled(checkBoxOut->isChecked());
    curThread->waitTime = lineEditTime->text().toInt(0, 16);
    curThread->rtaddr = comboBoxRTA->currentIndex();
    curThread->writeCfg = checkBoxConfRegLoad->isChecked();
    curThread->initEnable = checkBoxInit->isChecked();
    curThread->pauseTime = lineEditPause->text().toInt(0, 10);
    curThread->postponeTime = lineEditReservePause->text().toInt(0, 10);
    curThread->outMode = comboBoxShow->currentIndex();

    if (radioButtonEnter->isChecked())
        curThread->iterCycle = spinBoxCycle->value();
    else if (radioButtonUnlimited->isChecked())
        curThread->iterCycle = -1;
    else
        curThread->iterCycle = 1;

//  Сейчас не реализована ветка с итерациями. Будет передано ровно 1 000 000 000 бит.
    curThread->iterCycle = 1;

    curThread->codec = checkBoxCodec->isChecked();
    curThread->manipulation = comboBoxManType->currentText();

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
//        прерывание ОУ не включается настройками теста, его можно изменить в файле начальной конфигурации
//        curreg->en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;
    curreg->ena_codec = checkBoxCodec->isChecked() ? 1 : 0;
    curreg->rtavsk_ena = checkBoxEnaAddr->isChecked() ? 1 : 0;
    if (checkBoxEnaAddr->isChecked())
        curreg->rtavsk = curThread->rtaddr;

    curThread->timeMeasure = comboBoxSpeed->currentIndex();
    curThread->timeOverhead = lineEditOver->text().isEmpty() ? 0 : lineEditOver->text().toInt(0, 10);

    // длина пакета
    curThread->numOFDM = lineEditNumS->text().toUShort();
    if (curThread->numOFDM >= MAXNUMSYM || curThread->numOFDM <= 0)
        curThread->numOFDM = MAXNUMSYM-1;

    curThread->loadRTA = checkBoxRTALoad->isChecked();

    curThread->checkStatusErrBC = comboBoxErrStatusBC->currentIndex();  // в этом тесте только эта ошибка допускает 3 варианта реакции
    curThread->checkStatusErrRT = (comboBoxErrStatusRT->currentIndex() > 0);
    curThread->noIntFatalBC = (comboBoxBCIntErr->currentIndex() != 0);
    curThread->checkRTA = (comboBoxCheckRTA->currentIndex() > 0);
    curThread->checkLoadCfg = comboBoxWrongCfgReg->currentIndex() > 0;
    curThread->compEnableReg = comboBoxErrRegComp->currentIndex();

    curThread->dataSize = dataGen.getDataLen();
    int dataSizeRequired = curThread->numOFDM * calcNumWordInSymbol(comboBoxManType->currentIndex(), curThread->codec?1:0) - sizeof(word32_t);
    if (curThread->dataSize < dataSizeRequired)
        curThread->dataSize = dataSizeRequired;
    curThread->testData = dataGen.createData(curThread->dataSize, 2);

    emit startTestTh();
}

void NoiseTest::disableUnimplemented()
{
    // ПОКА НЕ РЕАЛИЗОВАНЫ
    checkBoxParamView->setDisabled(true);
    labelParamView->setDisabled(true);
    comboBoxExitRule->setDisabled(true);  // условие выхода
    settings->findChild<QLabel*>("labelExitRule")->setDisabled(true);
//    comboBoxShow->setDisabled(true);     // периодичность вывода
//    settings->findChild<QLabel*>("labelShow")->setDisabled(true);

    settings->findChild<QLabel*>("labelHeaderRepeat")->setVisible(false);
    radioButtonEnter->setVisible(false);
    radioButtonUnlimited->setVisible(false);
    radioButtonSingle->setVisible(false);
    spinBoxCycle->setVisible(false);
}

void noiseObjToThread::doWork()
{
    try
    {
        emit resultReady((int)AbstractTest::Running);
        perform();
    }
    catch(const QString& exception)
    {
        stdOutput(tr("Ошибка: ") + exception, tr("Error: ") + exception);

        emit statsOutputReady("errConnect", 1);
        emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}

bool noiseObjToThread::checkStatusRegBC(int statusBC, int interruption, int it, bool *error)
{
    bool bNoInt = false;
    bool bRet = true;

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
        stdOutput(tr("Итерация = %1   Ошибка в статусе КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')),
                  tr("Iter = %1   Errors in BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')));
        *error = true;
        if (checkStatusErrBC > 1)
            bRet = false;
    }
    else if (bNoInt)
    {
            stdOutput(tr("Статус КШ: %1").arg(statusBC, 4, 16, QLatin1Char('0')),
                      tr("BC status: %1").arg(statusBC, 4, 16, QLatin1Char('0')));
    }

    if (noIntFatalBC && bNoInt)
        bRet = false;

    return bRet;
}

void noiseObjToThread::checkStatusRT(REG_HSC_status &status, int it, bool *error)
{
    devRT->readReg(&status);
    if (checkStatusErrRT && (status.rt_bc_int == 0 || status.rs_err != 0))
    {
        quint32 status_data = status.getData();
        if (status.rt_bc_int == 0)
        {
            stdOutput(tr("Итерация = %1   Нет признака завершения обмена ОУ: %2").arg(it, 6).arg(status_data, 4, 16, QLatin1Char('0')),
                      tr("Iter = %1   No interruption flag in RT status: %2").arg(it, 6).arg(status_data, 4, 16, QLatin1Char('0')));
            *error = true;
        }
        if (codec && (status.rs_err != 0))
        {
            stdOutput(tr("Итерация = %1   Ошибка кодека в статусе ОУ: %2").arg(it, 6).arg(status_data, 4, 16, QLatin1Char('0')),
                      tr("Iter = %1   Codec error in RT status: %2").arg(it, 6).arg(status_data, 4, 16, QLatin1Char('0')));
            emit statsOutputReady("errStatusRT", 1);
            *error = true;
        }
    }
}


bool noiseObjToThread::outInfoEnable(quint64 curbit, quint64 prevbit, bool err) const
{
    if (curbit == 0)
        return true;

    quint64 compbit;
    switch(outMode)
    {
    case 0:
        return false;
    case 1:
        return err;
    case 2:
        return true;
    case 3:
        compbit = 1000000;
        break;
    case 4:
        compbit = 10000000;
        break;
    case 5:
        compbit = 20000000;
        break;
    case 6:
        compbit = 100000000;
        break;
    default:
        return false;
    }
    return ((curbit/compbit) > (prevbit/compbit));
}

bool noiseObjToThread::outTimeEnable(quint64 curbit, quint64 prevbit) const
{
    if (timeMeasure == 1)
        return true;
    else if (timeMeasure != 2)
        return false;
    quint64 compbit = 20000000;
    return ((curbit/compbit) > (prevbit/compbit));
}

void noiseObjToThread::printCurrInfo(int iter, quint64 curbit, quint64 prevbit, bool err)
{
    if (outInfoEnable(curbit, prevbit, err))
    {
        QString rus1 = tr("Итерация %1: ").arg(iter);
        QString eng1 = tr("Iteration %1: ").arg(iter);
        if (err)
        {
            rus1 += tr("ошибка");
            eng1 += tr("error");
        }
        else
        {
            rus1 += tr("успех");
            eng1 += tr("ok");
        }
        QString strCurBit = formattedLongInteger(curbit);
        QString rus2 = tr(". Всего передано %1 вит. Ошибочных пакетов %2. Ошибочных бит %3").arg(strCurBit).arg(errCounter).arg(wrongBit);
        QString eng2 = tr(". Total bit sent %1. Wrong packages: %2. Wrong bit: %3").arg(strCurBit).arg(errCounter).arg(wrongBit);
        stdOutput(rus1+rus2, eng1+eng2);
    }
}

// Вывод средней скорости передачи данных
void noiseObjToThread::averageSpeed(long time_ms, long wholePackBits, int iter)
{
    if (timeMeasure>0 && iter>0)
    {
        double time_sec = ((double)time_ms) * 0.001 / iter;
        stdOutput(tr("Среднее время передачи %1 бит равно %2 секунд").arg(wholePackBits).arg(time_sec),
                  tr("Average transmission time for %1 bit is %2 seconds").arg(wholePackBits).arg(time_sec));
        if (wholePackBits > 0 && time_sec > 0)
        {
            int mb = wholePackBits / time_sec;
            QString word = formattedLongInteger(mb);

            QString rus = tr("Оценка скорости передачи данных - ") + word + tr(" бит/с");
            QString eng = tr("Estimated transmission speed = ") + word + tr(" bps");
            stdOutput(rus, eng);//tr("Оценка скорости передачи данных - %1 бит/с").arg(mb),
        }
        else if (timeMeasure > 1)
            stdOutput(tr("Оценка скорости невозможна"), tr("Speed estimation is impossible"));
    }
}

void noiseObjToThread::perform()
{
    errCounter = 0;
    wrongBit = 0;

    if (testData == 0)
    {
        stdOutput(tr("Не удалось сгенерировать данные"), tr("Data generation failed"));
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        emit statsOutputReady("otherErr", 1);
        return;
    }

    qDebug() << "noiseObjToThread::perform() started";
    stdOutput(QObject::tr("Тест на помехоустойчивость запущен"), tr("Noise test started"));
//    goto skipDevices;

    if (connectBC() != AbstractTest::Running)
    {
        emit statsOutputReady("errConnect", 1);
        return;
    }

    if (connectRT() != AbstractTest::Running)
    {
        emit statsOutputReady("errConnect", 1);
        return;
    }

    if (initEnable)
    {
        // Начальная конфигурация ОУ
        qDebug() << "start RT configuration";
        if (writeCfg)
            devRT->configuration.setChecked(config_NUMREG_cfg, false);
        softReset(devRT);
        QByteArray regRT = devRT->configuration.getRegistersToWrite();
        if (regRT.size() > 0)
        {
            devRT->write_F1(regRT.data(), regRT.size());
            devRT->configuration.doneWriteReg(regRT);

            ///   Сравнение конфигурации
            if (compEnableReg)
            {
                QByteArray addrArray, valArray;
                if (splitOddEven(regRT, addrArray, valArray))
                {
                    // Чтение регистров ОУ для сравнения
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
                        stdOutput(tr("Возвращенные значения = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')),
                                  tr("Returned values = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')));
                        emit resultReady((int)AbstractTest::ErrorIsOccured);
                        emit statsOutputReady("errConfig", 1);
                        return;
                    }
                }
            }
        }
    }
    bool cfg_err = false;
    if (writeCfg)
    {
        devRT->writeReg(&devRT->reg_hsc_cfg);
        setRegWritten(devRT, devRT->reg_hsc_cfg);
        if ((checkLoadCfg || compEnableReg) && (devRT->reg_hsc_cfg.getData() != readRegVal(devRT, &devRT->reg_hsc_cfg)))
            cfg_err = true;
    }
    else if (checkLoadCfg)
    {
        REG_HSC_cfg cfg;
        devRT->readReg(&cfg);
        qDebug() << "RT cfg  " << cfg.ena_aru << " " << cfg.ena_codec << " " << cfg.ena_mem_vsk << " " << cfg.en_rt_bc_int << " "
                 << cfg.rtavsk << " " << cfg.rtavsk_ena << " " << cfg.rt_bc << " " << cfg.type_man;
        if (cfg.rt_bc != CFG_MODE_RT || cfg.ena_codec != codec ||
            cfg.type_man != devRT->reg_hsc_cfg.type_man || cfg.rtavsk_ena != devRT->reg_hsc_cfg.rtavsk_ena)
            cfg_err = true;
        /*else if (useInt && (devRT->reg_hsc_cfg.en_rt_bc_int != cfg.en_rt_bc_int))  // прерывание ОУ сейчас не используется, поэтому признак разрешения прерывания не важен
             cfg_err = true;*/
        else if ((cfg.rtavsk_ena==1) && (cfg.rtavsk!=rtaddr)) // иначе адрес ОУ должен быть во вспомогательном регистре reg_aux_rtaddr, но это не будем проверять,
                                    // потому что это регулируется записью и проверкой регистра reg_aux_rtaddr
              cfg_err = true;
    }
    if (cfg_err)
    {
        stdOutput(tr("Ошибка сравнения конфигурационного регистра ОУ"), tr("Comparison RT cfg register wrong"));
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        emit statsOutputReady("errConfig", 1);
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
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                emit statsOutputReady("errConfig", 1);
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
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                emit statsOutputReady("errConfig", 1);
                return;
            }
            if (cfg.rtavsk != rtaddr)
            {
                stdOutput(tr("Ошибка сравнения адреса ОУ"), tr("Comparison RT address wrong"));
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                emit statsOutputReady("errConfig", 1);
                return;
            }
        }
    }

    if (initEnable)
    {
        // Начальная конфигурация КШ
        qDebug() << "start BC configuration";
        if (writeCfg)
            devBC->configuration.setChecked(config_NUMREG_cfg, false);
        softReset(devBC);
        QByteArray regBC = devBC->configuration.getRegistersToWrite();
        if (regBC.size() > 0)
        {
            QByteArray regs;
            regs.append(regBC);
            devBC->write_F1(regs.data(), regs.size());
            devBC->configuration.doneWriteReg(regBC);

            ///   Сравнение конфигурации
            if (compEnableReg)
            {
                QByteArray addrArray, valArray;
                if (splitOddEven(regBC, addrArray, valArray))
                {
                    // Чтение регистров ОУ для сравнения
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
                        stdOutput(tr("Возвращенные значения = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')),
                                  tr("Returned values = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')));
                        emit resultReady((int)AbstractTest::ErrorIsOccured);
                        emit statsOutputReady("errConfig", 1);
                        return;
                    }
                }
            }
        }
    }

    cfg_err = false;
    if (writeCfg)
    {
        devBC->writeReg(&devBC->reg_hsc_cfg);
        setRegWritten(devBC, devBC->reg_hsc_cfg);
        devBC->configuration.setWritten(devBC->reg_hsc_cfg.address, devBC->reg_hsc_cfg.getData());
        if ((checkLoadCfg || compEnableReg) && devBC->reg_hsc_cfg.getData() != readRegVal(devBC, &devBC->reg_hsc_cfg))
            cfg_err = true;
    }
    else if (checkLoadCfg)
    {
        REG_HSC_cfg cfg;
        devBC->readReg(&cfg);
        // здесь значение регистра devBC->reg_hsc_cfg не может отличаться от настроечного, так как
        // даже если бы оно было прочитано из устройства, то при несовпадении был бы выход по ошибке
        qDebug() << "BC cfg  " << cfg.ena_aru << " " << cfg.ena_codec << " " << cfg.ena_mem_vsk << " " << cfg.en_rt_bc_int << " "
                 << cfg.rtavsk << " " << cfg.rtavsk_ena << " " << cfg.rt_bc << " " << cfg.type_man;
        if (cfg.rt_bc != CFG_MODE_BC || cfg.ena_codec != codec ||
            cfg.type_man != devBC->reg_hsc_cfg.type_man || (useInt && (cfg.en_rt_bc_int != devBC->reg_hsc_cfg.en_rt_bc_int)))
            cfg_err = true;
        if (useInt && (cfg.en_rt_bc_int == 0))
        {
            useInt = false;
            stdOutput(tr("Использование прерывание КШ не разрешено"), tr("Interruption is not enabled in BC configuration"));
        }
    }
    if (cfg_err)
    {
        stdOutput(tr("Ошибка сравнения конфигурационного регистра КШ"), tr("Comparison BC cfg register wrong"));
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        emit statsOutputReady("errConfig", 1);
        return;
    }
    // Еще одна возможность проверки разрешения использования прерывания КШ
    if (useInt && !writeCfg && !checkLoadCfg && initEnable && devBC->configuration.disInt())
    {
        useInt = false;
        stdOutput(tr("Использование прерывание КШ не разрешено"), tr("Interruption is not enabled in BC configuration"));
    }

    if (onPauseStop() == -1)
        return;
//skipDevices:
    initStartBC();

    CTestBC test;
    test.setConfigFlds(devBC->reg_hsc_cfg.type_man, devBC->reg_hsc_cfg.ena_codec!=0);

    quint64 nBitSpecification = 1000000000;
    qint64 nByteIterated = nBitSpecification / 8 * iterCycle;
    int nByteMax = test.numWordInSymbol() * sizeof(word32_t) * numOFDM;

    quint32 curBitErr = 0;

    REG_HSC_status regStatusRT;
    devRT->readReg(&regStatusRT);
    int statusBC = getStatusReg(devBC);


//stdOutput();

    int pos = 0;
    char *pData = (char*)testData;

    qDebug() << "numIter = " << nByteIterated / numOFDM;
    qDebug() << "nByte = " << nByteIterated << "  " << test.numWordInSymbol() << "  " << numOFDM;

    int iter = 0;
    long totalTime = 0;
    //int timeCounter = 0;
    quint64 totalBit = 0;

    QTime beginTime = QTime::currentTime();
    while (nByteIterated != 0)
    {
        char trmBuf[MAXPACKAGESIZE];
        char recBuf[NUMBYTEINOFDMSYM];

        bool errorOccured = false;
        int addr_rx_rt = getBufAddrRec(regStatusRT.rx_num_buf);  // getBufRec(statusRT);
        // int addr_tr_rt = getBufTrm(statusRT);
        int addr_rx_bc = getBufRec(statusBC);

        if (pos > dataSize)
            pos -= dataSize;

        int n_byte = nByteMax;
        if (nByteIterated > 0)
        {
            if (n_byte > nByteIterated)
                n_byte = nByteIterated;
            nByteIterated -= n_byte;
        }
        int n_cur_bit = n_byte * 8;
        totalBit += n_cur_bit;
        quint32 curMaxBitErr = totalBit / nBitSpecification + 1 - curBitErr;


        emit statsOutputReady("totalIter", 1);
        emit statsOutputReadyLongLong("totalBit", n_cur_bit);

        if (trbit == tgca_tr_TRM)   // ОУ-КШ
        {
            // создание командного пакета и данных для передачи по МКПД
            if (!test.createCommandPack((void*)recBuf, NUMBYTEINOFDMSYM, 0, n_byte, rtaddr, trbit, 0))
            {
                stdOutput(tr("Ошибка создания командного пакета"), tr("Command pack creation error"));
                emit statsOutputReady("otherErr", 1);
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                return;
            }
            if (!test.array2Pack((void*)trmBuf, MAXPACKAGESIZE, (void*)(pData+pos), n_byte))
            {
                stdOutput(tr("Ошибка создания тестового образца данных"), tr("Test data etalon creation error"));
                emit statsOutputReady("otherErr", 1);
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                return;
            }
            // Запись командного пакета в буфер передачи КШ
            devBC->write_F2(getBufTrm(statusBC), recBuf, NUMBYTEINOFDMSYM);
            // Запись данных в буфер передачи ОУ
            devRT->write_F2(getBufAddrTrm(regStatusRT.tx_num_buf) + sizeof(word32_t), trmBuf + sizeof(word32_t), numOFDM*NUMBYTEINOFDMSYM - sizeof(word32_t));
        }
        else   // КШ-ОУ
        {
            // создание командного пакета и данных для передачи по МКПД
            if (!test.createCommandPack((void*)trmBuf, MAXPACKAGESIZE, (void*)(pData+pos), n_byte, rtaddr, trbit, 0))
            {
                stdOutput(tr("Ошибка создания командного пакета"), tr("Command pack creation error"));
                emit statsOutputReady("otherErr", 1);
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                return;
            }
            // Запись командного пакета в буфер передачи КШ
            devBC->write_F2(getBufTrm(statusBC), trmBuf, numOFDM*NUMBYTEINOFDMSYM);
        }

        // Оконный режим
        switchWindow(1);
        thread()->msleep(delayTime);

        QTime curTime;
        if (timeMeasure)
            curTime = QTime::currentTime();

        // Старт обмена
        devBC->writeReg(&devBC->reg_hsc_creg);
        int interruption = waitForInterruption(devBC, useInt, waitTime, &statusBC);

        if (timeMeasure)
        {
            int t = curTime.msecsTo(QTime::currentTime());
            if (t > timeOverhead)
                t -= timeOverhead;
            totalTime += t;
            if (outTimeEnable(totalBit, totalBit-n_cur_bit))
                stdOutput(tr("Время передачи %1 мс, полное время %2 мс, данных %3 бит").arg(t).arg(totalTime).arg(n_cur_bit),
                          tr("Transmission time = %1 ms, total time = %2 ms, data size = %3 bit").arg(t).arg(totalTime).arg(n_cur_bit));
        }

        // Оконный режим
        switchWindow(0);

        //statusRT = getStatusReg(devRT);
        bool isOk = checkStatusRegBC(statusBC, interruption, iter, &errorOccured);
        checkStatusRT(regStatusRT, iter, &errorOccured);

        if (!isOk)
        {
            // сообщение об ошибке было внутри checkStatusRegBC()
            emit statsOutputReady("errPack", 1);
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }

        // Сравнение переданных данных
        char responseBuf[MAXPACKAGESIZE];
        if (trbit == tgca_tr_TRM)   // ОУ-КШ
        {
            // Чтение данных из буфера приёма КШ для проверки ответного слова и сравнения переданных данных
            devBC->read_F2(addr_rx_bc, numOFDM*NUMBYTEINOFDMSYM, responseBuf);
            // Проверка ответного слова
            if (checkStatusErrBC)
            {
            }
        }
        else   // КШ-ОУ
        {
            // Чтение данных из буфера приёма ОУ для сравнения
            if (postponeTime > 0)
                thread()->msleep(postponeTime);
            devRT->read_F2(addr_rx_rt, numOFDM*NUMBYTEINOFDMSYM, responseBuf);
            // Проверка ответного слова
            if (checkStatusErrBC)
            {
                devBC->read_F2(addr_rx_bc, NUMBYTEINOFDMSYM, recBuf);
            }
        }
        if (!test.cmpPackDataBit((void*)responseBuf, (void*)(pData+pos), n_byte, &n_cur_bit, curMaxBitErr))
        {
            stdOutput(tr("Ошибка сравнения переданных данных"), tr("Data comparison error"));
            errorOccured = true;
        }

        printCurrInfo(iter, totalBit, n_cur_bit, errorOccured);

        if (errorOccured)
        {
            errCounter ++;
            emit statsOutputReady("errPack", 1);
        }

        if (pauseTime > 0)
            thread()->msleep(pauseTime);

        if (onPauseStop() == -1)
            return;

        pos += n_byte;
        iter++;

    } // while

    emit resultReady(errCounter == 0 ? (int)AbstractTest::Completed : (int)AbstractTest::ErrorIsOccured);
}
