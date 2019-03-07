
#include "monitortest.h"

void MonitorTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);
    changeConnections();

    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");
    checkBoxDevBC = settings->findChild<QCheckBox *>("checkBoxDevBC");
    checkBoxDevRT = settings->findChild<QCheckBox *>("checkBoxDevRT");

top_1
    lineEditDevBC->setText(out.readLine());
    checkBoxDevBC->setChecked(!out.readLine().isEmpty());
    lineEditDevRT->setText(out.readLine());
    checkBoxDevRT->setChecked(!out.readLine().isEmpty());
    settingsFile.close();

    addDevicesTolist();

    objToThread = new monitorObjToThread();
    connectThread();
    connect(&testThread,SIGNAL(finished()), this, SLOT(printFin()));
    disableStat();

    testThread.start();
}

void MonitorTest::printFin()
{
    qDebug() << "testThread MonitorTest finished";
}

void MonitorTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << lineEditDevBC->text() << endl;
    in << (checkBoxDevBC->isChecked() ? "1" : "") << endl;
    in << lineEditDevRT->text() << endl;
    in << (checkBoxDevRT->isChecked() ? "1" : "") << endl;
    settingsFile.close();
}

void MonitorTest::startTest()
{
    monitorObjToThread* curThread = (monitorObjToThread*)objToThread;
    curThread->devBC = curThread->devRT = 0;
    if (deviceList.count() == 0)
    {
        message(tr("Valid devices are required"));
        return;
    }

    int index = 0;
    if (checkBoxDevBC->isChecked())
    {
        curThread->devBC = deviceList.at(0);
        index ++;
    }
    if (checkBoxDevRT->isChecked())
    {
        curThread->devRT = deviceList.at(index);
        index ++;
    }
    if (index == 0)
    {
        message(tr("No valid device assigned"));
        return;
    }

    Monitor monit;
    monit.setDevices(curThread->devBC, curThread->devRT);
    connect(&monit, SIGNAL(finished(int)), &monit, SLOT(forceExit()));
    connect(curThread, SIGNAL(doneWriteBC()), &monit, SLOT(doneWriteBC()));
    connect(curThread, SIGNAL(doneWriteRT()), &monit, SLOT(doneWriteRT()));
    connect(curThread, SIGNAL(doneReadBC(QByteArray)), &monit, SLOT(doneReadBC(QByteArray)));
    connect(curThread, SIGNAL(doneReadRT(QByteArray)), &monit, SLOT(doneReadRT(QByteArray)));
    connect(curThread, SIGNAL(connected(int)), &monit, SLOT(onConnect(int)));
    connect(curThread, SIGNAL(terminated()), &monit, SLOT(onPushExit()));
    monit.signalToMonitorTest = &curThread->signalFromMonitor;
    monit.arr = &curThread->writeArray;
    monit.force_exit = &curThread->force_exit;

    emit startTestTh();
    monit.exec();
}

void monitorObjToThread::doWork()
{
    signalFromMonitor = Monitor::wait,
    force_exit = false;
    emit resultReady((int)AbstractTest::Running);
    if (devBC && (connectBC() == AbstractTest::Running))
    {
        qDebug() << "BC connected";
        emit connected(0);
    }
    if (devRT && (connectRT() == AbstractTest::Running))
    {
        qDebug() << "RT connected";
        emit connected(1);
    }

//        //Monitor monit;
//        monit.setDevices(devBC, devRT);
//        //connect(&monit, SIGNAL(finished(int)), &monit, SLOT(forceExit()));
//        connect(this, SIGNAL(doneWriteBC()), &monit, SLOT(doneWriteBC()));
//        monit.writeBC = &write_size_BC;
//        monit.writeRT = &write_size_RT;
//        monit.readBC = &read_size_BC;
//        monit.readRT = &read_size_RT;
//        monit.arr = &array;
//        monit.force_exit = &force_exit;

        qDebug() << "monitorObjToThread is working";

    QByteArray readArray;
    while (1)
    {
        if (signalFromMonitor)
        {
            switch (signalFromMonitor)
            {
            case Monitor::writeBC:
                devBC->write_F1(writeArray.data(), writeArray.size());
                signalFromMonitor = Monitor::wait;
                emit doneWriteBC();
                break;

            case Monitor::writeRT:
                devRT->write_F1(writeArray.data(), writeArray.size());
                signalFromMonitor = Monitor::wait;
                emit doneWriteRT();
                break;

            case Monitor::readBC:
                readArray.clear();
                readArray.resize(writeArray.size());
                devBC->read_F1(writeArray.data(), readArray.data(), writeArray.size());
                signalFromMonitor = Monitor::wait;
                emit doneReadBC(readArray);
                break;

            case Monitor::readRT:
                readArray.clear();
                readArray.resize(writeArray.size());
                devRT->read_F1(writeArray.data(), readArray.data(), writeArray.size());
                signalFromMonitor = Monitor::wait;
                emit doneReadRT(readArray);
                break;

            default:
                qDebug() << "Wrong signal from monitor";
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                prepTerminate();
                return;
            }
        }
        if (force_exit)
        {
            qDebug() << "force_exit";
            emit resultReady((int)AbstractTest::Completed);
            prepTerminate();
            return;
        }
        if (threadState != AbstractTest::Running)
        {
            qDebug() << "threadState = " << threadState;
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            prepTerminate();
            return;
        }
        if (pause_stop() == -1)
        {
            qDebug() << "pause_stop";
            prepTerminate();
            return;
        }
        thread()->msleep(50);
    }
    emit resultReady((int)AbstractTest::ErrorIsOccured);
}

void monitorObjToThread::prepTerminate()
{
    qDebug() << "monitortest terminates";
    if (devBC)
        devBC->tryToDisconnect();
    if (devRT)
        devRT->tryToDisconnect();
    emit terminated();
    //emit resultReady((int)AbstractTest::Completed);
}
/*
void monitorObjToThread::forceExit()
{
    qDebug() << "forceExit";
    prepTerminate();
    emit resultReady((int)AbstractTest::Completed);
}
*/
monitorObjToThread::monitorObjToThread()
{
    outEnable = false;
}
