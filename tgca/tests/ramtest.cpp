#include "ramtest.h"

void RamTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);
    //disconnect((QAction*)menu.findChild<QAction*>("stats"), SIGNAL(triggered(bool)), stats, SLOT(show()));
    changeConnections();

    lineEditShift = settings->findChild<QLineEdit*>("lineEditShift");
    comboBoxNBufBC = settings->findChild<QComboBox*>("comboBoxNBufBC");
    comboBoxNBufRT = settings->findChild<QComboBox*>("comboBoxNBufRT");

    labelBegAddrBC = settings->findChild<QLabel*>("labelBegAddrBC");
    labelEndAddrBC = settings->findChild<QLabel*>("labelEndAddrBC");
    labelBegAddrRT = settings->findChild<QLabel*>("labelBegAddrRT");
    labelEndAddrRT = settings->findChild<QLabel*>("labelEndAddrRT");

    lineEditNum = settings->findChild<QLineEdit*>("lineEditNum");
    comboBoxBufDirBC = settings->findChild<QComboBox*>("comboBoxBufDirBC");
    comboBoxBufDirRT = settings->findChild<QComboBox*>("comboBoxBufDirRT");
    comboBoxDev = settings->findChild<QComboBox*>("comboBoxDev");
    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");

top_1
    comboBoxDev->setCurrentText(out.readLine());
    comboBoxNBufBC->setCurrentText(out.readLine());
    comboBoxBufDirBC->setCurrentText(out.readLine());
    comboBoxNBufRT->setCurrentText(out.readLine());
    comboBoxBufDirRT->setCurrentText(out.readLine());
    lineEditShift->setText(out.readLine());
    lineEditNum->setText(out.readLine());
    lineEditDevBC->setText(out.readLine());
    lineEditDevRT->setText(out.readLine());

    lineEditShift->setInputMask("HHHHH;_");
    lineEditNum->setInputMask("HHHHH;_");

    settingsFile.close();

    addDevicesTolist();

    connect(comboBoxNBufBC, SIGNAL(activated(int)), this, SLOT(resetFlds()));
    connect(comboBoxBufDirBC, SIGNAL(activated(int)), this, SLOT(resetFlds()));
    connect(comboBoxNBufRT, SIGNAL(activated(int)), this, SLOT(resetFlds()));
    connect(comboBoxBufDirRT, SIGNAL(activated(int)), this, SLOT(resetFlds()));
    connect(comboBoxDev, SIGNAL(activated(int)), this, SLOT(resetFlds()));
    connect(lineEditShift, SIGNAL(textChanged(QString)), this, SLOT(resetFlds()));
    connect(lineEditNum, SIGNAL(textChanged(QString)), this, SLOT(resetFlds()));

    resetFlds();

    objToThread = new ramObjToThread();
    connectThread();

    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("errComp", stats->findChild<QLabel*>("errComp"));
    connectStatisticSlots();

    testThread.start();
}

int RamTest::updateDeviceList()
{
    int ind = comboBoxDev->currentIndex(); // 0 - только КШ, 1 - только ОУ, 2 - сравнение КШ и ОУ
    if (ind != 1)
        deviceLineEditList.append(lineEditDevBC);
    if (ind != 0)
        deviceLineEditList.append(lineEditDevRT);
    return ind+1;
}

void RamTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << comboBoxDev->currentText() << endl;
    in << comboBoxNBufBC->currentText() << endl;
    in << comboBoxBufDirBC->currentText() << endl;
    in << comboBoxNBufRT->currentText() << endl;
    in << comboBoxBufDirRT->currentText() << endl;
    in << lineEditShift->text() << endl;
    in << lineEditNum->text() << endl;
    in << lineEditDevBC->text() << endl;
    in << lineEditDevRT->text() << endl;
    settingsFile.close();
}

void RamTest::startTest()
{
    ramObjToThread* curThread = (ramObjToThread*)objToThread;
    curThread->addrBC = begAddrBC();
    curThread->rangeBC = curThread->addrBC + 4*lineEditNum->text().toInt(0,16);
    curThread->addrRT = begAddrRT();
    curThread->rangeRT = curThread->addrRT + 4*lineEditNum->text().toInt(0,16);
    switch (comboBoxDev->currentIndex())
    {
    case 0:
        curThread->devBC = deviceList.at(0);
        curThread->devRT = 0;
        break;
    case 1:
        curThread->devRT = deviceList.at(0);
        curThread->devBC = 0;
        break;
    case 2:
        curThread->devBC = deviceList.at(0);
        curThread->devRT = deviceList.at(1);
    }

    statsToZero();
    emit startTestTh();
}

uint RamTest::begAddrBC()
{
    return 0x40000 + 0x20000*comboBoxBufDirBC->currentIndex() + 0x40000*comboBoxNBufBC->currentIndex() + 4*lineEditShift->text().toInt(0,16);
}

uint RamTest::begAddrRT()
{
    return 0x40000 + 0x20000*comboBoxBufDirRT->currentIndex() + 0x40000*comboBoxNBufRT->currentIndex() + 4*lineEditShift->text().toInt(0,16);
}

void RamTest::resetFlds()
{
    int add = 4*lineEditNum->text().toInt(0,16);
    switch (comboBoxDev->currentIndex())
    {
    case 0:  // КШ
        labelBegAddrBC->setText(tr("%1h ").arg(begAddrBC(), 5, 16, QLatin1Char('0')));
        labelEndAddrBC->setText(tr("%1h ").arg(begAddrBC()+add, 5, 16, QLatin1Char('0')));
        labelBegAddrRT->setText(tr(""));
        labelEndAddrRT->setText(tr(""));
        break;
    case 1:  // ОУ
        labelBegAddrBC->setText(tr(""));
        labelEndAddrBC->setText(tr(""));
        labelBegAddrRT->setText(tr("%1h ").arg(begAddrRT(), 5, 16, QLatin1Char('0')));
        labelEndAddrRT->setText(tr("%1h ").arg(begAddrRT()+add, 5, 16, QLatin1Char('0')));
        break;
    case 2:  // оба
        labelBegAddrBC->setText(tr("%1h ").arg(begAddrBC(), 5, 16, QLatin1Char('0')));
        labelEndAddrBC->setText(tr("%1h ").arg(begAddrBC()+add, 5, 16, QLatin1Char('0')));
        labelBegAddrRT->setText(tr("%1h ").arg(begAddrRT(), 5, 16, QLatin1Char('0')));
        labelEndAddrRT->setText(tr("%1h ").arg(begAddrRT()+add, 5, 16, QLatin1Char('0')));
    }
}

void ramObjToThread::perform()
{
    Device *dev = 0;
    QTcpSocket *tcpSocket;

    if (devBC)
    {
        if (connectBC() != AbstractTest::Running)
            return;
        if (devRT == 0)
        {
            dev = devBC;
            tcpSocket = &tcpSocketBC;
        }
        qDebug() << "BC connected";
    }
    if (devRT)
    {
        if (connectRT() != AbstractTest::Running)
            return;
        if (devBC == 0)
        {
            dev = devRT;
            tcpSocket = &tcpSocketRT;
        }
        qDebug() << "RT connected";
    }

    int addrinc = sizeof(word32_t);
    if (dev)
    {
        QByteArray readArray, answer;

        uint addr = devBC ? addrBC : addrRT;
        uint range = devBC ? rangeBC : rangeRT;
        for (uint i=addr; i<range; i+=addrinc)
            readArray.append((char*)&i, addrinc);

        answer.resize(readArray.size());
        dev->read_F1(readArray.data(), answer.data(), readArray.size());

        for (int i=0; i<answer.size(); i+=addrinc)
        {
    //        stdOutput(QString("Read: (%1)  %2").arg(addr+i1, 8, 16, QLatin1Char('0')).arg((uint)*(uint*)(answer.data()+i), 8, 16, QLatin1Char('0')), "");
            stdOutput(QString("Read: (%1)  %2").arg((uint)*(int*)(readArray.data()+i), 8, 16, QLatin1Char('0')).arg((uint)*(int*)(answer.data()+i), 8, 16, QLatin1Char('0')), "");
        }
        //tcpSocket->abort();
    }
    else
    {
        QByteArray readArrayBC, answerBC;
        QByteArray readArrayRT, answerRT;

        for (uint i1=addrBC, i2=addrRT; i1 < rangeBC; i1+=addrinc, i2+=addrinc)
        {
            readArrayBC.append((char*)&i1, addrinc);
            readArrayRT.append((char*)&i2, addrinc);
        }

        answerBC.resize(readArrayBC.size());
        answerRT.resize(readArrayRT.size());
        devBC->read_F1(readArrayBC.data(), answerBC.data(), readArrayBC.size());
        devRT->read_F1(readArrayRT.data(), answerRT.data(), readArrayRT.size());

        uint sz = answerBC.size();
        if (sz != answerRT.size())
        {
            stdOutput(tr("Ошибка чтения памяти: длина массивов не совпадает"), tr("Error memory reading: different size"));
            emit resultReady(AbstractTest::ErrorIsOccured);
            return;
        }
        emit statsOutputReady("totalIter", sz/4);
        for (uint addr1=addrBC, addr2=addrRT, i=0; i<sz; i+=addrinc, addr1+=addrinc, addr2+=addrinc)
        {
    //        stdOutput(QString("Read: (%1)  %2").arg(addr+i1, 8, 16, QLatin1Char('0')).arg((uint)*(uint*)(answer.data()+i), 8, 16, QLatin1Char('0')), "");
            uint ans1 = (uint)*(int*)(answerBC.data()+i);
            uint ans2 = (uint)*(int*)(answerRT.data()+i);
            QString msg =  tr("(%1) %2   (%3) %4").arg(addr1, 8, 16, QLatin1Char('0')).arg(ans1, 8, 16, QLatin1Char('0')).
                                                      arg(addr2, 8, 16, QLatin1Char('0')).arg(ans2, 8, 16, QLatin1Char('0'));
            if (ans1 != ans2)
            {
                msg += tr("  Не совпадает!");
                emit statsOutputReady("errComp", 1);
            }
            stdOutput(msg, msg);
        }
        //tcpSocket->abort();
    }
    emit resultReady(AbstractTest::Completed);
}

ramObjToThread::ramObjToThread()
{
    outEnable = true;
}

void ramObjToThread::terminate(int )
{
    if (!isRunning())  // (fl == AbstractTest::ErrorIsOccured || fl == AbstractTest::Completed || fl == AbstractTest::Stopped)
    {
        if (devBC != 0 && tcpSocketBC.state() == QAbstractSocket::ConnectedState)
            tcpSocketBC.abort();
        if (devRT != 0 && tcpSocketRT.state() == QAbstractSocket::ConnectedState)
            tcpSocketRT.abort();
    }
}
