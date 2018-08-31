#include "monitortest.h"
#include "../monitor.h"

void MonitorTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,pB,tB);
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
    if (deviceList.count() != 2)
    {
        message(tr("Two devices are required"));
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
        qDebug() << "No valid device assigned";
        //return;
    }

    Monitor monit;
    monit.setDevice(curThread->devBC, curThread->devRT);
    connect(&monit, SIGNAL(finished(int)), &monit, SLOT(forceExit()));
    connect(curThread, SIGNAL(doneWriteBC()), &monit, SLOT(doneWriteBC()));
    monit.writeBC = &curThread->write_size_BC;
    monit.writeRT = &curThread->write_size_RT;
    monit.readBC = &curThread->read_size_BC;
    monit.readRT = &curThread->read_size_RT;
    monit.arr = &curThread->array;
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
}

void MonitorTest::updateSettings()
{
    updateDeviceList();
    int done = (checkBoxDevBC->isChecked() ? 1 : 0) + (checkBoxDevRT->isChecked() ? 2 : 0);
    emit settingsClosed(done);
}

void monitorObjToThread::doWork()
{/*
    if (devBC)
    {
        QString ip = devBC->connection.getServerIP();
        ushort port = devBC->connection.getServerPORT().toUShort();
        tcpSocketBC.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketBC.waitForConnected(5000)) {
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            qDebug() << "return";
            return;
        }
        qDebug() << "BC connected";
    }
    if (devRT)
    {
        QString ip = devRT->connection.getServerIP();
        ushort port = devRT->connection.getServerPORT().toUShort();
        tcpSocketRT.connectToHost(QHostAddress(ip), port);
        if (!tcpSocketRT.waitForConnected(5000)) {
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            devRT = 0;
            terminate();
            return;
        }
        qDebug() << "RT connected";
    }*/
    emit resultReady((int)AbstractTest::Running);

//        //Monitor monit;
//        monit.setDevice(devBC, devRT);
//        //connect(&monit, SIGNAL(finished(int)), &monit, SLOT(forceExit()));
//        connect(this, SIGNAL(doneWriteBC()), &monit, SLOT(doneWriteBC()));
//        monit.writeBC = &write_size_BC;
//        monit.writeRT = &write_size_RT;
//        monit.readBC = &read_size_BC;
//        monit.readRT = &read_size_RT;
//        monit.arr = &array;
//        monit.force_exit = &force_exit;

        qDebug() << "monitorObjToThread is working";

    int write_size_BC = 0;
    QByteArray array;
    int c=0;
    while (1)
    {
        if (c==0 )
        {
            c=1;
            qDebug() << "in cycle";
        }
        if (write_size_BC)
        {
            if (write_F1(&tcpSocketBC, array) == -1)
            {
                terminate();
                return;
            }
            write_size_BC = 0;
            emit doneWriteBC();
        }
        if (write_size_RT)
        {
            if (write_F1(&tcpSocketRT, array) == -1)
            {
                terminate();
                return;
            }
        }
        if (pause_stop() == -1)
        {
            terminate();
            return;
        }
        //qDebug() << force_exit;
        if (force_exit)
        {
            qDebug() << force_exit;
            emit resultReady((int)AbstractTest::Completed);
            terminate();
            return;
        }
        this->thread()->msleep(50);
    }
    emit resultReady((int)AbstractTest::Completed);
}

void monitorObjToThread::terminate()
{
    if (devBC)
        tcpSocketBC.abort();
    if (devRT)
        tcpSocketRT.abort();
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
    absObjToThread(),
    //devBC(0),
    //devRT(0),
    write_size_BC(0),
    write_size_RT(0),
    read_size_BC(0),
    read_size_RT(0),
    force_exit(false)
{
}

monitorObjToThread::~monitorObjToThread()
{
}
