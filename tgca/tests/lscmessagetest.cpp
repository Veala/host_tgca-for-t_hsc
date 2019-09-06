#include "lscmessagetest.h"
#include "../regNSK.h"

void LscMessageTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    deviceEditBC = settings->findChild<QLineEdit*>("BC");
    deviceLineEditList.append(deviceEditBC);
    deviceEditRT = settings->findChild<QLineEdit*>("RT");
    deviceLineEditList.append(deviceEditRT);
top_1
    deviceEditBC->setText(out.readLine());
    deviceEditRT->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new lscMessageObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread, SIGNAL(resultReady(int)), objToThread, SLOT(terminate(int)));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,long)), this, SLOT(statsTestOutput(QString,long)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void LscMessageTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << deviceEditBC->text() << endl;
    in << deviceEditRT->text() << endl;
    settingsFile.close();
}

void LscMessageTest::startTest()
{
    lscMessageObjToThread* curThread = (lscMessageObjToThread*)objToThread;
    curThread->devBC = deviceList.at(0);
    curThread->devRT = deviceList.at(1);

    emit startTestTh();
}

/////////////////////////////////////////////////////////////////////////////////

lscMessageObjToThread::lscMessageObjToThread():
    absObjToThread(),
    devBC(0),
    devRT(0),
    num_messages(0)
{
}

lscMessageObjToThread::~lscMessageObjToThread()
{
}

void lscMessageObjToThread::doWork()
{
    bool autorepeat = false, cycle = true, check = false, speed = true;
    int rtaddr = 3, Count = 100;
    int bus = lsc_BUS_B;   // 0 - А, 1 - Б
    int stackRT = 0;  // 0 - А, 1 - Б
    int stackBC = 0;  // 0 - А, 1 - Б

    int initCommandStackRT = 0;
    int initCommandStackBC = 0x200;//0;
    int initMessageArea = 0;//0x200;

    int errors = 0;
    fixRT = 0x100+stackRT*4;
    fixBC = 0x100+stackBC*4;
    curCommandStackBC = initCommandStackBC;
    curCommandStackRT = initCommandStackRT;
    ptrMessagesBC = initMessageArea;

    qDebug() << "lscMessageObjToThread::doWork() started";
    try
    {
        if (cycle)
        {
            /*
            time_t timer = time(NULL);
            char buftime[128];
            ctime_s(buftime, 128, &timer);
            emit outputReady(QString("Время начала теста: ")+QString(buftime));
            */
            QDateTime local(QDateTime::currentDateTime());
            emit outputReady(QString("Время начала цикла: ") + local.toString(tr("dd.MM.yyyy hh:mm:ss")));
        }

        emit resultReady((int)AbstractTest::Running);

        if (connectBC() != AbstractTest::Running)
            return;
        if (connectRT() != AbstractTest::Running)
            return;

        // конфигурация ОУ НСК
        bufRT.addReg(mr_NUMREG_COMMAND, 1);  // сброс
//        bufRT.addReg(mr_NUMREG_CONFIG_1, 0x8000); // RT
        bufRT.addReg(mr_NUMREG_CONFIG_1, 0x8780 | (mr_REG_CFG1_CURR_AREA_FLAG*stackRT));
        bufRT.addReg(mr_NUMREG_CONFIG_5, 0x2);
        bufRT.addReg(mr_NUMREG_CONFIG_3, 0x8000);
        bufRT.addReg(mr_NUMREG_CONFIG_2, 0x1003);
        bufRT.addReg(0x60, rtaddr);

        bufRT.writeDev(devRT, true);

        fillMemBlkRT(0x100, 0x300, 0);      // 0x100 - 0x400
        fillMemBlkRT(0x140, 0x60, 0x400);   // буферы приёма и передачи для всех подадресов инициализированы значением 0x400
        fillMemBlkRT(0x1C0, 0x60, 0x400);
        fillMemBlkRT(0x400, 1280, 0xCC);

        //writeRegInitRT();
        for (int i=1; i<15; i++)
        {
            bufRT.addRamLSC(0x140+i, 0x400+i*64);
            sa_buf_rec[i] = 0x400+i*64;
        }
        bufRT.addRamLSC(fixRT, initCommandStackRT);
        bufRT.writeDev(devRT, true);

        // конфигурация и запись данных в КШ НСК
        bufBC.addReg(mr_NUMREG_COMMAND, 1);  // сброс
        bufBC.addReg(mr_NUMREG_CONFIG_3, 0x8000);
        bufBC.addReg(mr_NUMREG_CONFIG_5, 0x1);
//        bufBC.addReg(mr_NUMREG_CONFIG_1, 0); // BC
//        bufBC.addReg(mr_NUMREG_CONFIG_2, 0); // должен быть 0 после сброса
        if (autorepeat)
        {
            bufBC.addReg(mr_NUMREG_CONFIG_1, 0x0160 | (mr_REG_CFG1_CURR_AREA_FLAG*stackBC));
            bufBC.addReg(mr_NUMREG_BCFTIME_RTCW_WWMT, 0x0014);
        }
        else
            bufBC.addReg(mr_NUMREG_CONFIG_1, 0x0060 | (mr_REG_CFG1_CURR_AREA_FLAG*stackBC));

        std::vector<C_LSC_Format*> messages;

        messages.push_back(new C_LSC_Format1(rtaddr, 1, 5, 0xAA, true));
        messages.push_back(new C_LSC_Format1(rtaddr, 2, 3, 0xBB, true));
        //messages.push_back(new C_LSC_Format2());
        //messages.push_back(new C_LSC_Format3());
        //messages.push_back(new C_LSC_Format4());
        //messages.push_back(new C_LSC_Format5());
        //messages.push_back(new C_LSC_Format6());
        //messages.push_back(new C_LSC_Format7());
        //messages.push_back(new C_LSC_Format8());
        //messages.push_back(new C_LSC_Format9());
        //messages.push_back(new C_LSC_Format10());

        for (unsigned i=0; i<messages.size(); i++)
            addMessage(messages[i], bus);

/*
        bufBC.addRamLSC(curCommandStackBC, spoiler);
        bufBC.addRamLSC(curCommandStackBC+2, 0x0);
        bufBC.addRamLSC(curCommandStackBC+3, ptrMessagesBC);  // 0, 400
        curCommandStackBC += 4;
        int datalen = 5;
        arrayAddrRW[num_messages] = ptrMessagesBC+datalen+3;
        num_messages++;
        ptrMessagesBC += (datalen+4);
        datalen = 3;
        arrayAddrRW[num_messages] = ptrMessagesBC+datalen+3;
        num_messages++;
*/
#if 0
        bufBC.addRamLSC(0x000, 0x0);  // 0 - шина Б, 0x80 - шина А
        bufBC.addRamLSC(0x001, 0x828);
        bufBC.addRamLSC(0x026, 0x0);  // 0 - шина Б, 0x80 - шина А
        bufBC.addRamLSC(0x027, 0x848);
        bufBC.wtiteDev(devBC, true);
        fillMemBlk(devBC, 0x428, 8, 0xBB);
        fillMemBlk(devBC, 0x402, 8, 0xAA);
#else

        //bufBC1.wtiteDev(devBC, true);
        word32_t data[18] = {0, 0x1845, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0, 0, 0, 0x1883, 0xBB, 0xBB, 0xBB, 0, 0, 0, 0};
        writeMemBlkBC(0, data, 18);
#endif
        if (num_messages == 0)
        {
            stdOutput("Empty frame");
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }

        bufBC.addRamLSC(fixBC, initCommandStackBC);
        bufBC.addRamLSC(fixBC+1, 0xFFFF-num_messages);
        if (autorepeat)
        {
            bufBC.addRamLSC(fixBC+2, initCommandStackBC);
            bufBC.addRamLSC(fixBC+3, 0xFFFF-num_messages);
        }

        // Старт КШ
        //writeRegInitBC();
        bufBC.addReg(mr_NUMREG_COMMAND, 2);
        bufBC.writeDev(devBC, true);
        stdOutput("After StartBC()");
        if (!autorepeat)
        {
            for (int i=0; i<num_messages; i++)
            {
                word32_t bcsw = readStatusWordBC(initCommandStackBC+i*4);
                bool bcswok = (bcsw == 0xA000);
                if (check || !bcswok)
                {
                    stdOutput(tr("BC SW%1 = %2  %3").arg(i).arg(bcsw, 4, 16, QLatin1Char('0')).arg(bcswok ? "OK" : "ERROR !!!"));
                }
                if (check)
                {
                    word32_t rw = readResponseWord(arrayAddrRW[i]);
                    stdOutput(tr("RW%1 = %2     %3").arg(i).arg(rw, 4, 16, QLatin1Char('0')).arg((rw == (rtaddr << 11)) ? "OK" : "ERROR !!!"));
                }
                if (!messages[i]->check())
                {
                    stdOutput(tr("Errors in message 1").arg(i));
                    errors++;
                }
                else if(!bcswok)
                    errors++;
            }

            word32_t buf1[32];
            readMemBlkRT(0x140, buf1, 32);
            word32_t buf2[32];
            readMemBlkRT(buf1[2], buf2, 32);
            stdOutput(tr("BufRT addr=%1: %2 %3 %4 %5 %6 %7 %8 %9").arg(buf1[2], 4, 16, QLatin1Char('0')).arg(buf2[0], 4, 16, QLatin1Char('0')).
                    arg(buf2[1], 4, 16, QLatin1Char('0')).arg(buf2[2], 4, 16, QLatin1Char('0')).arg(buf2[3], 4, 16, QLatin1Char('0')).arg(buf2[4], 4, 16, QLatin1Char('0'))
                    .arg(buf2[5], 4, 16, QLatin1Char('0')).arg(buf2[6], 4, 16, QLatin1Char('0')).arg(buf2[7], 4, 16, QLatin1Char('0')));
            readMemBlkRT(buf1[4], buf2, 32);
            stdOutput(tr("BufRT addr=%1: %2 %3 %4 %5 %6 %7 %8 %9").arg(buf1[4], 4, 16, QLatin1Char('0')).arg(buf2[0], 4, 16, QLatin1Char('0')).
                    arg(buf2[1], 4, 16, QLatin1Char('0')).arg(buf2[2], 4, 16, QLatin1Char('0')).arg(buf2[3], 4, 16, QLatin1Char('0')).arg(buf2[4], 4, 16, QLatin1Char('0'))
                    .arg(buf2[5], 4, 16, QLatin1Char('0')).arg(buf2[6], 4, 16, QLatin1Char('0')).arg(buf2[7], 4, 16, QLatin1Char('0')));

            if (cycle)
            {
                for (int count=0; count< Count; count++)
                {
                    if (pause_stop() == -1)
                    {
                        stdOutput(tr("Interrupted on step %1.").arg(count));
                        /*
                        time_t timer = time(NULL);
                        char buftime[128];
                        ctime_s(buftime, 128, &timer);
                        emit outputReady(QString("Время окончания теста: ")+QString(buftime));
                        */
                        QDateTime local(QDateTime::currentDateTime());
                        emit outputReady(QString("Время начала теста: ") + local.toString(tr("dd.MM.yyyy hh:mm:ss")));
                        emit resultReady((int)AbstractTest::Stopped);
                        return;
                    }
                    if (count%100 == 0 && count!=0)
                        qDebug() << "count=" << count;
                    CWriteDeviceMemory bufBC1;
                    CWriteDeviceMemory bufRT1;
                    for (int i=0; i<num_messages; i++)
                    {
                        bufRT1.addRamLSC(initCommandStackRT+i*4, spoiler);
                        bufBC1.addRamLSC(initCommandStackBC+i*4, spoiler);
                        messages[i]->spoil(speed);
                    }
                    bufRT1.writeDev(devRT, true);
                    bufBC1.addRamLSC(fixBC, initCommandStackBC);
                    bufBC1.addRamLSC(fixBC+1, 0xFFFF-num_messages);
                    bufBC1.addReg(mr_NUMREG_COMMAND, 2);
                    bufBC1.writeDev(devBC, true);

                    for (int i=0; i<num_messages; i++)
                    {
                        word32_t bcsw = readStatusWordBC(initCommandStackBC+i*4);
                        bool bcswok = (bcsw == 0xA000);
                        if (check || !bcswok)
                            stdOutput(tr("BC SW%1 = %2  %3").arg(i).arg(bcsw, 4, 16, QLatin1Char('0')).arg(bcswok ? "OK" : "ERROR !!!"));
                        if (check)
                        {
                            word32_t rw = readResponseWord(arrayAddrRW[i]);
                            stdOutput(tr("RW%1 = %2     %3").arg(i).arg(rw, 4, 16, QLatin1Char('0')).arg((rw == (rtaddr << 11)) ? "OK" : "ERROR !!!"));
                        }
                        if (!messages[i]->check())
                        {
                            stdOutput(tr("Message #%1: ERROR !!!").arg(i));
                            errors++;
                        }
                        else if(!bcswok)
                            errors++;
                    }

                }
            }
        }
        else
        {
            for (int i=0; i< 1000; i++)
            {
                if (pause_stop() == -1)
                {
                    writeRegSingleBC(mr_NUMREG_COMMAND, 0x20);
                    emit resultReady((int)AbstractTest::Stopped);
                    return;
                }
                thread()->msleep(100);
            }
            writeRegSingleBC(mr_NUMREG_COMMAND, 0x20);
        }

        if (cycle)
        {
            /*
            time_t timer = time(NULL);
            char buftime[128];
            ctime_s(buftime, 128, &timer);
            emit outputReady(QString("Время окончания теста: ")+QString(buftime));
            */
            QDateTime local(QDateTime::currentDateTime());
            emit outputReady(QString("Время начала теста: ") + local.toString(tr("dd.MM.yyyy hh:mm:ss")));
        }
        if (errors)
        {
            stdOutput(tr("%1 errors").arg(errors));
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        }
        else
        {
            stdOutput("Success");
            emit resultReady((int)AbstractTest::Completed);
        }


    }
    catch(const QString& exception)
    {
        stdOutput(tr("Error: ") + exception);
        emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}

AbstractTest::RunningState lscMessageObjToThread::connectBC()
{
    try
    {
        devBC->tryToConnect();
        qDebug() << "BC connected";
        return AbstractTest::Running;
    }
    catch(const QString&)
    {
        stdOutput("BC: no connection");

        if (pause_stop() == -1)
        {
            emit resultReady((int)AbstractTest::Stopped);
            return AbstractTest::Stopped;
        }

        emit resultReady((int)AbstractTest::ErrorIsOccured);
        return AbstractTest::ErrorIsOccured;
    }
}

AbstractTest::RunningState lscMessageObjToThread::connectRT()
{
    try
    {
        devRT->tryToConnect();
        qDebug() << "RT connected";
        return AbstractTest::Running;
    }
    catch(const QString&)
    {
        stdOutput("RT: no connection");

        if (pause_stop() == -1)
        {
            emit resultReady((int)AbstractTest::Stopped);
            return AbstractTest::Stopped;
        }

        emit resultReady((int)AbstractTest::ErrorIsOccured);
        return AbstractTest::ErrorIsOccured;
    }
}

bool lscMessageObjToThread::isTestRunning(int st)
{
    return st == AbstractTest::Running || st == AbstractTest::Paused;
}

void lscMessageObjToThread::terminate(int st)
{
    if (!isTestRunning(st))
    {
        devBC->tryToDisconnect();
        devRT->tryToDisconnect();
    }
}
/*
void lscMessageObjToThread::writeRegInitBC()
{
    regBC.clear();
}

void lscMessageObjToThread::writeRegAddBC(word32_t num, word32_t val)
{
    int addr = num*4;
    regBC.append((char*)&addr, 4);
    regBC.append((char*)&val, 4);
}

void lscMessageObjToThread::writeMemAddBC(word32_t addr, word32_t val)
{
    word32_t real_addr = 0x4000 + addr*4;
    regBC.append((char*)&real_addr, 4);
    regBC.append((char*)&val, 4);
}

void lscMessageObjToThread::writeRegistersBC()
{
    devBC->write_F1(regBC.data(), regBC.size());
}

void lscMessageObjToThread::writeRegInitRT()
{
    regRT.clear();
}

void lscMessageObjToThread::writeRegAddRT(word32_t num, word32_t val)
{
    word32_t addr = num*4;
    regRT.append((char*)&addr, 4);
    regRT.append((char*)&val, 4);
    //char* data = regRT.data();
}

void lscMessageObjToThread::writeRegistersRT()
{
    devRT->write_F1(regRT.data(), regRT.size());
}

void lscMessageObjToThread::writeMemAddRT(word32_t addr, word32_t val) {word32_t real_addr = 0x4000 + addr*4; regRT.append((char*)&real_addr, 4); regRT.append((char*)&val, 4); }
*/

void lscMessageObjToThread::writeMemBlkBC(int dst_addr, word32_t *buf, int num_words)
{
    devBC->write_F2(0x4000 + dst_addr*4, (char*)buf, num_words * 4);
}

bool lscMessageObjToThread::fillMemBlk(Device* dev, int dst_addr, int num_words16, word16_t value)
{
    char* buf = (char*)malloc(num_words16 * 4);
    if (buf == 0)
        return false;
    word32_t* tmp = (word32_t*)buf;
    for (int i=0; i<num_words16; i++)
        tmp[i] = value;
    dev->write_F2(0x4000 + dst_addr*4, buf, num_words16 * 4);
    free (buf);
    return true;
}

word32_t lscMessageObjToThread::readResponseWord(word32_t addr)
{
    word32_t answer;
    devBC->read_F2(0x4000+addr*4, sizeof(word32_t), (char*)(&answer));
    return answer;
}

word32_t lscMessageObjToThread::readStatusWordBC(word32_t addr)
{
    word32_t answer;
    devBC->read_F2(0x4000+addr*4, sizeof(word32_t), (char*)(&answer));
    return answer;
}

void lscMessageObjToThread::writeRegSingleBC(word32_t num, word32_t value)
{
    devBC->write_F2(num*4, (char*)(&value), 4);
}

void lscMessageObjToThread::readMemBlkRT(int addr, word32_t* buf, int num_words)
{
    devRT->read_F2(0x4000+addr*4, num_words*4, (char*)(buf));
}

bool lscMessageObjToThread::addMessage(C_LSC_Format* format, int bus)
{
    if (num_messages >= max_num_messages || format == 0)
        return false;

    bufBC.addRamLSC(curCommandStackBC, spoiler);
    bufBC.addRamLSC(curCommandStackBC+2, 0x0);
    bufBC.addRamLSC(curCommandStackBC+3, ptrMessagesBC);
    curCommandStackBC += 4;
    format->setDevices(devBC, devRT);
    format->setSABufs(sa_buf_rec, sa_buf_trm, sa_buf_brd);
    format->setBus(bus);
    format->addMessage(curCommandStackRT, ptrMessagesBC);
    num_messages++;

    return true;
}
