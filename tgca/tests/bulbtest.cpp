#include "bulbtest.h"

void BulbTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,pB,tB);

    lineEditIterate = settings->findChild<QLineEdit *>("lineEditIterate");
    lineEditInterval = settings->findChild<QLineEdit *>("lineEditInterval");
    lineEditDevice = settings->findChild<QLineEdit *>("lineEditDevice");
    deviceLineEditList.append(lineEditDevice);

    top_1
        lineEditIterate->setText(out.readLine());
        lineEditInterval->setText(out.readLine());
        lineEditDevice->setText(out.readLine());
        settingsFile.close();
        checkDeviceAvailability(1111);

        objToThread = new bulbObjToThread();
        objToThread->moveToThread(&testThread);
        connect(&testThread, SIGNAL(finished()), objToThread, SLOT(deleteLater()));
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
     settingsFile.close();
}

void BulbTest::startTest()
{
    bulbObjToThread* curThread = (bulbObjToThread*)objToThread;

    curThread->setPause(lineEditInterval);
    curThread->setIter(lineEditIterate);

    curThread->dev = deviceList.at(0);
    curThread->socketDescriptor = curThread->dev->rw_socket.socketDescriptor();
    if (curThread->socketDescriptor == -1) {
        message(tr("Socket descriptor error"));
        return;
    }
    emit startTestTh();
}

void bulbObjToThread::doWork()
{
    int vals[24] = { 1, 2, 4, 8,   0, 1, 3, 7,   15, 14, 13, 11,
                     7, 3, 12, 6,  9, 6, 9, 0,   15, 0, 15, 0 };
    int addr = 0x194;
    int cmd  = 1;
    int len  = 8;

    if (tcpSocket.state() != QAbstractSocket::ConnectedState) {
        if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            return;
        }
    }

    emit resultReady((int)AbstractTest::Running);

    qDebug() << "Bulb test started";

    for (int i=0; i<iter; i++)
    {
        for (int j=0; j<24; j++)
        {
            QByteArray array;
            array.append((char*)&cmd, 4);
            array.append((char*)&len, 4);
            array.append((char*)&addr, 4);
            array.append((char*)&(vals[j]), 4);

            int n = tcpSocket.write(array, array.size());
            if (!tcpSocket.waitForBytesWritten(5000)) {
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                return;
            }
            qDebug() << "wrote: " << n;
            if (n == -1) {
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                return;
            }
            this->thread()->msleep(pause);
        }
    }
    resultReady(AbstractTest::Completed);
}
