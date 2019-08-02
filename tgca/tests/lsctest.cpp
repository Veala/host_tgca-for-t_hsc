#include "lsctest.h"

void LscTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
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

    objToThread = new lscObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,long)), this, SLOT(statsTestOutput(QString,long)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void LscTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << deviceEditBC->text() << endl;
    in << deviceEditRT->text() << endl;
    settingsFile.close();
}

void LscTest::startTest()
{
    lscObjToThread* curThread = (lscObjToThread*)objToThread;
    curThread->devices.clear();
    curThread->isStopped.clear();
    for (int i=0; i<deviceList.count(); i++) {
        curThread->devices.append((Device*)deviceList.at(i));
        curThread->isStopped.append(QString("no"));
        curThread->devices.at(i)->lscGatewayThread.state = "nothing";
    }
    emit startTestTh();
}

void lscObjToThread::doWork()
{
    int n = devices.count(), n2 = 0;
    int p = 0;
    try {
        emit resultReady((int)AbstractTest::Running);
        for  (int i=0; i<n; i++) devices.at(i)->tryToConnect();
        for  (int i=0; i<n; i++) devices.at(i)->lscGatewayThread.start();
        while(1) {
            this->thread()->msleep(100);
            if (p == 0 && pause_stop() == -1) {
                for  (int i=0; i<n; i++) devices.at(i)->setLSCGatewayEnabled(0);
                p=1;
            }
            for (int i=0; i<n; i++) {
                if (devices.at(i)->lscGatewayThread.state != "nothing" && isStopped.at(i) == "no") {
                    isStopped[i] = "yes";
                    n2++;
                    emit outputReady(tr("НСК шлюз %1 не работает, исключение: %2").arg(devices.at(i)->getName()).arg(devices.at(i)->lscGatewayThread.state));
                }
                if (n2==n) {
                    emit outputReady(tr("Все НСК шлюзы не работают"));
                    throw QString(devices.at(i)->lscGatewayThread.state);
                }
            }
        }
    } catch (const QString& exception) {
        for  (int i=0; i<n; i++) devices.at(i)->tryToDisconnect();
        if (exception == "connection") {
            if (pause_stop() == -1) {
                emit resultReady((int)AbstractTest::Stopped);
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else if (exception == "socket") {
            if (pause_stop() == -1) {
                emit resultReady((int)AbstractTest::Stopped);
                return;
            }
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        } else if (exception == "stopped") {
            emit resultReady(AbstractTest::Stopped);
        } else if (exception == "memory") {
            emit resultReady(AbstractTest::ErrorIsOccured);
        }
    }
}
