#include "echotest.h"

void EchoTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);
    disableStat();
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
    try
    {
        emit resultReady((int)AbstractTest::Running);
        dev->tryToConnect();
        dev->write_Echo(echoText);
        throw QString("finish");
    }
    catch (const QString& exception) {
           if (exception == "connection") {
               if (pause_stop() == -1) { emit resultReady(AbstractTest::Stopped); return; }
               emit resultReady((int)AbstractTest::ErrorIsOccured);
           } else if (exception == "socket") {
               //if (pause_stop() == -1) return;
               emit resultReady((int)AbstractTest::ErrorIsOccured);
           } else if (exception == "stopped") {
               dev->tryToDisconnect();
               emit resultReady(AbstractTest::Stopped);
           } else if (exception == "finish") {
               dev->tryToDisconnect();
               emit resultReady(AbstractTest::Completed);
           }
       }
}
