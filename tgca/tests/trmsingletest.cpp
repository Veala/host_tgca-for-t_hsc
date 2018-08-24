#include "trmsingletest.h"
//#include "../registers.h"
#include "../ctestbc.h"
#include "../gendata.h"


void TrmSingleTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,pB,tB);

    disconnect(settings,SIGNAL(finished(int)),this,SLOT(checkDeviceAvailability(int)));
    connect(settings,SIGNAL(finished(int)),this,SLOT(updateSettings()));
    connect(this,SIGNAL(settingsClosed(int)),this,SLOT(checkDeviceAvailability(int)));

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
}

void TrmSingleTest::updateSettings()
{
    // Проверка устройств. Можно проверять только с признаком необходимости.
    updateDeviceList();
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
    }

    curThread->broad = checkBoxBroad->isChecked();
    curThread->useInt = checkBoxUseInt->isChecked();
    curThread->output = checkBoxOut->isChecked();
    curThread->time = lineEditTime->text().toInt(0, 16);
    curThread->rtaddr = comboBoxRTA->currentIndex();
    curThread->data_size = 0;
    curThread->data = 0;

    ushort rta = curThread->broad ? BRD_RT_ADDR : curThread->rtaddr;

    int index = 0;
    int cfgBC = 0;
    if (checkBoxDevBC->isChecked())
    {
        curThread->devBC = deviceList.at(0);
        index ++;
        cfgBC = curThread->devBC->configuration.getConfigReg();
    }
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
        curThread->cfgRT= cfgRT;
        curThread->amplRT= comboBoxAmplRT->currentIndex();
    }

    uint num_b = mnb;
    if (lineEditLen->text() != "max")
        num_b = lineEditLen->text().toInt(0, 16);
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
        rawData = createRegulatData(num_b, lineEditBegin->text().toInt(0, 16), lineEditStep->text().toInt(0, 16),
                                    lineEditNumStep->text().isEmpty() ? 0 : lineEditNumStep->text().toInt(0, 16),
                                    comboBoxUnit->currentText().toInt(0,16));
    else
        rawData = createRandomData(num_b, lineEditBegin->text().toInt(0, 16), lineEditStep->text().toInt(0, 16),
                                    lineEditNumStep->text().toInt(0, 16), comboBoxUnit->currentText().toInt(0,16));


    int sz_dst = (test.NumSymOFDM(num_b) + 1) * NUMWORDINOFDMSYM * sizeof(word32_t);
    curThread->data = malloc(sz_dst);
    if (!test.createCommandPack(curThread->data, sz_dst, rawData, num_b, rta, tgca_tr_REC, comboBoxCode->currentIndex())) // ОУ принимает
    {
        qDebug() << "Error createCommandPack !!!";
        free(curThread->data);
        curThread->data = 0;
        return;
    }
    curThread->data_size = sz_dst;
    free(rawData);

    emit startTestTh();
}

trmSingleObjToThread::trmSingleObjToThread():
    absObjToThread(),
    cfgBC(0), cfgRT(0), amplBC(0), amplRT(0),
    devBC(0), devRT(0),
    data(0)
{
}

bool trmSingleObjToThread::checkStatusReg(QTcpSocket* socket, int *status)
{
    int addr_stat = REG_VSK_status;
    QByteArray answer;
    QByteArray readArray = cmdHead(3, 4);
    readArray.append((char*)&addr_stat, 4);
    if (read_F1(socket, readArray, answer) == -1)
        return false;

    *status = *(int*)(answer.data());
    if ((*status) & FL_REG_STATUS_flags)
        qDebug() << "Error flags in status:  " << QString::number(*status, 16);

    return true;
}

bool trmSingleObjToThread::checkCounters(QTcpSocket* socket)
{
    int addr_reg = REG_VSK_cnt_pct_tx_msw;
    QByteArray answer;
    QByteArray readArray = cmdHead(3, 16);
    for (int i=0; i<4; i++)
    {
        readArray.append((char*)&addr_reg, 4);
        addr_reg += 4;
    }
    if (read_F1(socket, readArray, answer) == -1)
        return false;

    qDebug() << "Counters:  " << (*(int*)(answer.data())) << "  " << (*(int*)(answer.data()+4))
             << "  " << (*(int*)(answer.data()+8)) << "  " << (*(int*)(answer.data()+12));

    return true;
}

void trmSingleObjToThread::doWork()
{
    qDebug() << "trmSingleObjToThread doWork start";

    if (devBC)
    {
        QString ip = devBC->connection.getServerIP();
        ushort port = devBC->connection.getServerPORT().toUShort();
        tcpSocketBC.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketBC.waitForConnected(5000)) {
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }
        qDebug() << "BC connected";
        checkCounters(&tcpSocketBC);
    }
    if (devRT)
    {
        QString ip = devRT->connection.getServerIP();
        ushort port = devRT->connection.getServerPORT().toUShort();
        tcpSocketRT.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketRT.waitForConnected(5000)) {
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }
        qDebug() << "RT connected";
        checkCounters(&tcpSocketRT);
    }
    emit resultReady((int)AbstractTest::Running);

    int addr_cfg  = REG_VSK_cfg;
    int addr_ampl = REG_VSK_amplification_factor;
    int addr_rta  = 0x180;  // вспомогательный регистр
    int addr_stat = REG_VSK_status;
    int addr_creg = REG_VSK_creg;

    if (devBC)
    {
        qDebug() << "start BC configuration";
        devBC->configuration.setChecked(config_NUMREG_cfg, false);
        QByteArray regBC = devBC->configuration.getRegistersToWrite();
        if (regBC.size() > 0)
        {
            QByteArray regs = cmdHead(1, regBC.size());
            regs.append(regBC);
            if (write_F1(&tcpSocketBC, regs) == -1)
                return;
            devBC->configuration.doneWriteReg(regBC);
        }
        checkCounters(&tcpSocketBC);

        QByteArray array = cmdHead(1, 8);
        array.append((char*)&addr_cfg, 4);
        array.append((char*)&cfgBC, 4);
        if (write_F1(&tcpSocketBC, array) == -1)
            return;
        devBC->configuration.setWritten(addr_cfg, cfgBC);

        if (amplBC)
        {
            int ampl = (amplBC == 1) ? 0 : amplBC + 14;
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_ampl, 4);
            array.append((char*)&ampl, 4);
            if (write_F1(&tcpSocketBC, array) == -1)
                return;
            devBC->configuration.setWritten(addr_ampl, ampl);
        }
    }
    if (devRT)
    {
        qDebug() << "start RT configuration";
        devRT->configuration.setChecked(config_NUMREG_cfg, false);
        QByteArray regRT = devRT->configuration.getRegistersToWrite();
        if (regRT.size() > 0)
        {
            QByteArray regs = cmdHead(1, regRT.size());
            regs.append(regRT);
            if (write_F1(&tcpSocketRT, regs) == -1)
                return;
            devRT->configuration.doneWriteReg(regRT);
        }
        checkCounters(&tcpSocketRT);

        QByteArray array = cmdHead(1, 8);
        array.append((char*)&addr_cfg, 4);
        array.append((char*)&cfgRT, 4);
        if (write_F1(&tcpSocketRT, array) == -1)
            return;
        devRT->configuration.setWritten(addr_cfg, cfgRT);

        if (amplRT)
        {
            int ampl = (amplRT == 1) ? 0 : amplRT + 14;
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_ampl, 4);
            array.append((char*)&ampl, 4);
            if (write_F1(&tcpSocketRT, array) == -1)
                return;
            devRT->configuration.setWritten(addr_ampl, ampl);
        }

        if (rtaddr <= MAX_RT_ADDR)
        {
            QByteArray array = cmdHead(1, 8);
            array.append((char*)&addr_rta, 4);
            array.append((char*)&rtaddr, 4);
            if (write_F1(&tcpSocketRT, array) == -1)
                return;
        }
    }
    if (data_size <= 0)
    {
        qDebug() << "data_size = " << data_size;
    }
    else
    {
        int statusBC, statusRT;
        if (devBC)
        {
            qDebug() << "Check BC status";
            if (!checkStatusReg(&tcpSocketBC, &statusBC))
                return;
        }
        if (devRT)
        {
            qDebug() << "Check RT status";
            if (!checkStatusReg(&tcpSocketRT, &statusRT))
                return;
        }
        if (devBC)
        {
            QByteArray pack = cmdHead(2, data_size+4);
            int buf = getBufTrm(statusBC & fl_REG_STATUS_tx_num_buf);
            pack.append((char*)&buf, 4);
            pack.append((char*)data, data_size);
            free(data);
            data = 0;
            if (write_F2(&tcpSocketBC, pack) == -1)
                return;

            qDebug() << "Ready to transmit";
            if (devRT)
            {
                int counter = 0;
                QByteArray array = cmdHead(1, 8);
                int start_bc = fl_REG_CREG_start_bc;
                array.append((char*)&addr_creg, 4);
                array.append((char*)&start_bc, 4);
                QByteArray readArray = cmdHead(3, 4);
                readArray.append((char*)&addr_stat, 4);
                if (write_F1(&tcpSocketBC, array) == -1)
                    return;
#if 0
                checkCounters(&tcpSocketRT);

                //qDebug() << "Check RT status";
                /*if (read_F1(&tcpSocketRT, readArray, answer) == -1) return;
                status = *(int*)(answer.data());
            */
                checkStatusReg(&tcpSocketRT, &statusRT);
                qDebug() << "RT status = " << QString::number(statusRT, 16);
                checkCounters(&tcpSocketRT);

                qDebug() << "Check BC status";
                QTimer timer;
                timer.setInterval(time);
                timer.start();
#endif
                int status;
                do
                {
                    QByteArray answer;
                    if (read_F1(&tcpSocketBC, readArray, answer) == -1) return;
                    status = *(int*)(answer.data());
                    counter++;
                }
                while ((status & FL_REG_STATUS_flags) == 0/* && timer.remainingTime()>0*/);
                qDebug() << "counter = " << counter;
                qDebug() << "BC status = " << QString::number(status, 16);
                checkCounters(&tcpSocketBC);
                checkCounters(&tcpSocketRT);




            }
        }
    }

    emit resultReady((int)AbstractTest::Completed);
}

void trmSingleObjToThread::terminate(int fl)
{
    //qDebug() << "terminates: " << devBC << "  " << devRT << "  " << data;
    if (fl == AbstractTest::ErrorIsOccured || fl == AbstractTest::Completed || fl == AbstractTest::Stopped)
    {
        if (devBC)
            tcpSocketBC.abort();
        if (devRT)
            tcpSocketRT.abort();
        if (data)
            free(data);
    }
}
