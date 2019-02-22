#include "bulbtest.h"
#include "../registers.h"

void BulbTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);
    disableStat();

    lineEditIterate = settings->findChild<QLineEdit *>("lineEditIterate");
    lineEditInterval = settings->findChild<QLineEdit *>("lineEditInterval");
    lineEditDevice = settings->findChild<QLineEdit *>("lineEditDevice");
    checkBoxOut = settings->findChild<QCheckBox *>("checkBoxOut");
    deviceLineEditList.append(lineEditDevice);

    top_1
        lineEditIterate->setText(out.readLine());
        lineEditInterval->setText(out.readLine());
        lineEditDevice->setText(out.readLine());
        checkBoxOut->setChecked(!out.readLine().isEmpty());
        settingsFile.close();
        checkDeviceAvailability(1111);

        objToThread = new bulbObjToThread();
        objToThread->moveToThread(&testThread);
        connect(&testThread, SIGNAL(finished()), objToThread, SLOT(deleteLater()));
        connect(objToThread, SIGNAL(resultReady(int)), objToThread, SLOT(switchOff(int)));
        connect(objToThread, SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
        connect(objToThread, SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
        connect(this, SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
        testThread.start();
}

void BulbTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
     in << lineEditIterate->text() << endl;
     in << lineEditInterval->text() << endl;
     in << lineEditDevice->text() << endl;
     in << (checkBoxOut->isChecked() ? "1" : "") << endl;
     settingsFile.close();
}

void BulbTest::startTest()
{
    bulbObjToThread* curThread = (bulbObjToThread*)objToThread;

    curThread->setPause(lineEditInterval);
    curThread->setIter(lineEditIterate);
    curThread->dev = deviceList.at(0);
    curThread->setOutEnabled(checkBoxOut->isChecked());

    emit startTestTh();
}

void bulbObjToThread::switchOff(int rs)
{
    if (rs == AbstractTest::Stopped && tcpSocket.state() == QAbstractSocket::ConnectedState)
    {
        dev->reg_aux_bulb.first=0; dev->reg_aux_bulb.second=0;
        dev->reg_aux_bulb.third=0; dev->reg_aux_bulb.fourth=0;
        dev->writeReg(&dev->reg_aux_bulb);
//        int addr=REG_AUX_bulb, val=0;
//        QByteArray array = cmdHead(1, 8);
//        array.append((char*)&addr, 4);
//        array.append((char*)&val, 4);
//        dev->write_F1(array);
        tcpSocket.abort();
    }
}

void bulbObjToThread::doWork()
{
    try
    {
        QString ip = dev->connection.getServerIP();
        ushort port = dev->connection.getServerPORT().toUShort();

        emit resultReady((int)AbstractTest::Running);
        tcpSocket.connectToHost(QHostAddress(ip), port);
        if (!tcpSocket.waitForConnected(5000)) {
            if (pause_stop() == -1) {
                tcpSocket.abort();
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            if (outEnable)
            {
                QString ms = QString(tr("Нет соединения: ip = ")) + ip;
                emit outputReady(ms);
            }
            tcpSocket.abort();
            return;
        }
        dev->setSocket(&tcpSocket);

        if (outEnable)
        {
            QString ms = QString(tr("Соединение установлено: ip = ")) + ip;
            emit outputReady(ms);
        }

        int vals[24] = { 1, 2, 4, 8,   0, 1, 3, 7,   15, 14, 13, 11,
                         7, 3, 12, 6,  9, 6, 9, 0,   15, 0, 15, 0 };

        for (uint i=0; i<iterCount; i++)
        {
            for (int j=0; j<24; j++)
            {
                dev->reg_aux_bulb.first=1;
                dev->reg_aux_bulb.second=1;
                dev->reg_aux_bulb.third=1;
                dev->reg_aux_bulb.fourth=1;
                *(((quint32*)&dev->reg_aux_bulb)+1) &= vals[j];
                dev->writeReg(&dev->reg_aux_bulb);
                if (pause_stop() == -1) {
                    tcpSocket.abort();
                    return;
                }
                thread()->msleep(iterTime);
            }
        }
        tcpSocket.abort();
        emit resultReady((int)AbstractTest::Completed);
    }
    catch(const QString& exception)
    {
        if (outEnable)
            emit outputReady(QString(tr("Ошибка: ")) + exception);
/*        if (tcpSocket.state() == QAbstractSocket::ConnectedState)
            tcpSocket.abort(); */
        emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}
