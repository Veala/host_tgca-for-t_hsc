#include "echotest.h"

void EchoTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,pB,tB);
    echo = settings->findChild<QLineEdit *>("echo");
    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1
    echo->setText(out.readLine());
    deviceEdit->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new echoObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void EchoTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << echo->text() << endl;
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void EchoTest::startTest()
{
    echoObjToThread* curThread = (echoObjToThread*)objToThread;
    curThread->echoText = echo->text();
    curThread->dev = deviceList.at(0);

    emit startTestTh();
}

void echoObjToThread::doWork()
{
    QString ip = dev->connection.getServerIP();
    ushort port = dev->connection.getServerPORT().toUShort();

    tcpSocket.connectToHost(QHostAddress(ip), port);
    if (!tcpSocket.waitForConnected(5000)) {
        emit resultReady((int)AbstractTest::ErrorIsOccured);
        tcpSocket.abort();
        return;
    }

    emit resultReady((int)AbstractTest::Running);

    QByteArray writeArray = cmdHead(5, echoText.size()+1);
    writeArray.append(echoText.toStdString().c_str(), echoText.size()+1);
    if (write_Echo(&tcpSocket, writeArray) == -1) return;
    tcpSocket.abort();
    emit resultReady(AbstractTest::Completed);
}
