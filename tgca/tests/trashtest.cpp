#include "trashtest.h"

void TrashTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    mode = settings->findChild<QComboBox *>("mode");
    startAddr = settings->findChild<QLineEdit *>("startAddr");
    count = settings->findChild<QLineEdit *>("count");
    endAddr = settings->findChild<QLineEdit *>("endAddr");
    startData = settings->findChild<QLineEdit *>("startData");
    incData = settings->findChild<QSpinBox *>("incData");
    //oput = settings->findChild<QComboBox*>("output");
    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1

    mode->setCurrentText(out.readLine());
    startAddr->setText(out.readLine());
    count->setText(out.readLine());
    endAddr->setText(out.readLine());
    startData->setText(out.readLine());
    incData->setValue(out.readLine().toInt());
    deviceEdit->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new trashObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(objToThread,SIGNAL(statsOutputReady(QString,long)), this, SLOT(statsTestOutput(QString,long)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void TrashTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << mode->currentText() << endl;
    in << startAddr->text() << endl;
    in << count->text() << endl;
    in << endAddr->text() << endl;
    in << startData->text() << endl;
    in << incData->text() << endl;
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void TrashTest::startTest()
{
    trashObjToThread* curThread = (trashObjToThread*)objToThread;
    curThread->mode = mode->currentText();
    curThread->startAddr = startAddr->text().toUInt(NULL, 16);
    curThread->endAddr = endAddr->text().toUInt(NULL, 16);
    curThread->count = count->text().toUInt(NULL, 10);
    curThread->startData = startData->text().toUInt(NULL, 10);
    curThread->incData = incData->value();
    //curThread->output = oput->currentText().toUInt();
    curThread->dev = deviceList.at(0);

    emit startTestTh();
}

void trashObjToThread::doWork()
{
    try {
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
            tcpSocket.abort();
            return;
        }
        dev->setSocket(&tcpSocket);


//------------------------------------------------------
        qDebug() << mode;
        qDebug() << startAddr;
        qDebug() << count;
        qDebug() << endAddr;
        qDebug() << startData;
        qDebug() << incData;

        if (mode == "w") {
            writeArray.clear();
            for (uint i=0; i<count; i+=4, startData+=incData)
                writeArray.append((char*)&startData, 4);
            dev->write_F2(startAddr, writeArray.data(), count);
            qDebug() << "wrote size: " << writeArray.size();
            qDebug() << "left one: " << *(uint*)writeArray.left(4).data();
            qDebug() << "right one: " << *(uint*)writeArray.right(4).data();
        } else if (mode == "r") {
            readArray.clear();
            readArray.resize(count);
            dev->read_F2(startAddr,count,readArray.data());
            qDebug() << "read size: " << readArray.size();
            int n = memcmp((void*)readArray.data(), (void*)writeArray.data(), (size_t)count);
            if (n==0) qDebug() << "writeArray and readArray are EQUAL ";
            else qDebug() << "writeArray and readArray are NOT EQUAL ";
            qDebug() << "left one: " << *(uint*)readArray.left(4).data();
            qDebug() << "right one: " << *(uint*)readArray.right(4).data();
        } else if (mode == "t") {
            dev->cpyOnHard(startAddr,count,endAddr);
            qDebug() << "cpyOnHard";
            readArray.clear();
            readArray.resize(count);
            dev->read_F2(endAddr,count,readArray.data());
            qDebug() << "read size: " << readArray.size();
            int n = memcmp((void*)readArray.data(), (void*)writeArray.data(), (size_t)count);
            if (n==0) qDebug() << "writeArray and readArray are EQUAL ";
            else qDebug() << "writeArray and readArray are NOT EQUAL ";
            qDebug() << "left one: " << *(uint*)readArray.left(4).data();
            qDebug() << "right one: " << *(uint*)readArray.right(4).data();
        }
        tcpSocket.abort();
        emit resultReady(AbstractTest::Completed);
    } catch (const QString& exception) {
        //if (exception == "socket")
            emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}
