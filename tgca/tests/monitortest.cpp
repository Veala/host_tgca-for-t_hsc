#include "monitortest.h"
//#include "../monitor.h"

void MonitorTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);
    disconnect(settings,SIGNAL(finished(int)),this,SLOT(checkDeviceAvailability(int)));
    connect(settings,SIGNAL(finished(int)),this,SLOT(updateSettings()));
    connect(this,SIGNAL(settingsClosed(int)),this,SLOT(checkDeviceAvailability(int)));

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

    if (checkBoxDevBC->isChecked())
        deviceLineEditList.append(lineEditDevBC);
    if (checkBoxDevRT->isChecked())
        deviceLineEditList.append(lineEditDevRT);
    checkDeviceAvailability((checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0));

    objToThread = new monitorObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread, SLOT(deleteLater()));
    connect(&testThread,SIGNAL(finished()), this, SLOT(printFin()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));

    testThread.start();
}

void MonitorTest::printFin()
{
    qDebug() << "testThread finished";
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

void MonitorTest::updateDeviceList()
{
    deviceLineEditList.clear();
    if (checkBoxDevBC->isChecked())
        deviceLineEditList.append(lineEditDevBC);
    if (checkBoxDevRT->isChecked())
        deviceLineEditList.append(lineEditDevRT);
    qDebug() << deviceLineEditList.size() << " devices";
}

void MonitorTest::updateSettings()
{
    updateDeviceList();
    int done = (checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0);
    emit settingsClosed(done);
}

void monitorObjToThread::doWork()
{
    signalFromMonitor = Monitor::wait,
    force_exit = false;
    if (devBC)
    {
        QString ip = devBC->connection.getServerIP();
        ushort port = devBC->connection.getServerPORT().toUShort();
        tcpSocketBC.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketBC.waitForConnected(5000))
        {
            qDebug() << "BC not connected";
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        }
        else
        {
            devBC->setSocket(&tcpSocketBC);
            qDebug() << "BC connected";
            emit connected(0);
        }
    }
    if (devRT)
    {
        QString ip = devRT->connection.getServerIP();
        ushort port = devRT->connection.getServerPORT().toUShort();
        tcpSocketRT.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketRT.waitForConnected(5000))
        {
            qDebug() << "RT not connected";
            emit resultReady((int)AbstractTest::ErrorIsOccured);
        }
        else
        {
            devRT->setSocket(&tcpSocketRT);
            qDebug() << "RT connected";
            emit connected(1);
        }
    }
    emit resultReady((int)AbstractTest::Running);

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
                if (devBC->write_F1(writeArray) == -1)
                {
                    terminate();
                    return;
                }
                signalFromMonitor = Monitor::wait;
                emit doneWriteBC();
                break;

            case Monitor::writeRT:
                if (devRT->write_F1(writeArray) == -1)
                {
                    terminate();
                    return;
                }
                signalFromMonitor = Monitor::wait;
                emit doneWriteRT();
                break;

            case Monitor::readBC:
                readArray.clear();
                if (devBC->read_F1(writeArray, readArray) == -1)
                {
                    terminate();
                    return;
                }
                signalFromMonitor = Monitor::wait;
                emit doneReadBC(readArray);
                break;

            case Monitor::readRT:
                readArray.clear();
                if (devRT->read_F1(writeArray, readArray) == -1)
                {
                    terminate();
                    return;
                }
                signalFromMonitor = Monitor::wait;
                emit doneReadRT(readArray);
                break;

            default:
                qDebug() << "Wrong signal from monitor";
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                terminate();
                return;
            }
        }
        if (force_exit)
        {
            qDebug() << "force_exit";
            emit resultReady((int)AbstractTest::Completed);
            terminate();
            return;
        }
        if (threadState != AbstractTest::Running)
        {
            qDebug() << "threadState = " << threadState;
            terminate();
            return;
        }
        if (pause_stop() == -1)
        {
            qDebug() << "pause_stop";
            terminate();
            return;
        }
        thread()->msleep(50);
    }
    emit resultReady((int)AbstractTest::ErrorIsOccured);
}

void monitorObjToThread::terminate()
{
    qDebug() << "terminates";
    if (devBC && tcpSocketBC.state() == QAbstractSocket::ConnectedState)
        tcpSocketBC.abort();
    if (devRT && tcpSocketRT.state() == QAbstractSocket::ConnectedState)
        tcpSocketRT.abort();
    emit terminated();
    emit resultReady((int)AbstractTest::Completed);
}
/*
void monitorObjToThread::forceExit()
{
    qDebug() << "forceExit";
    terminate();
    emit resultReady((int)AbstractTest::Completed);
}
*/
monitorObjToThread::monitorObjToThread() :
    //devBC(0),
    //devRT(0),
    //signalFromMonitor(Monitor::wait),
    //force_exit(false),
    absObjToThread()
{
}
