#include "regtest.h"

void RegTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);
    disableStat();

    mode = settings->findChild<QComboBox*>("mode");
    startAddr = settings->findChild<QLineEdit *>("startAddress");
    endAddr = settings->findChild<QLineEdit *>("endAddress");
    incAddr = settings->findChild<QSpinBox *>("incAddress");
    startData = settings->findChild<QLineEdit *>("startData");
    incData = settings->findChild<QSpinBox *>("incData");
    inversion = settings->findChild<QComboBox*>("inverse");
    iteration = settings->findChild<QSpinBox *>("iteration");
    oput = settings->findChild<QComboBox*>("output");
    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1
    mode->setCurrentText(out.readLine());
    startAddr->setText(out.readLine());
    endAddr->setText(out.readLine());
    incAddr->setValue(out.readLine().toInt());
    startData->setText(out.readLine());
    incData->setValue(out.readLine().toInt());
    inversion->setCurrentText(out.readLine());
    iteration->setValue(out.readLine().toInt());
    oput->setCurrentText(out.readLine());
    deviceEdit->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new regObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void RegTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << mode->currentText() << endl;
    in << startAddr->text() << endl;
    in << endAddr->text() << endl;
    in << incAddr->text() << endl;
    in << startData->text() << endl;
    in << incData->text() << endl;
    in << inversion->currentText() << endl;
    in << iteration->text() << endl;
    in << oput->currentText() << endl;
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void RegTest::startTest()
{
    regObjToThread* curThread = (regObjToThread*)objToThread;
    curThread->mode = mode->currentText();
    curThread->addr = startAddr->text().toUInt(NULL, 16);
    curThread->range = endAddr->text().toUInt(NULL, 16);
    curThread->addrinc = incAddr->value();
    curThread->data = startData->text().toUInt(NULL, 16);
    curThread->datainc = incData->value();
    curThread->inverse = inversion->currentText().toUInt();
    curThread->inCycle = iteration->value();
    curThread->output = oput->currentText().toUInt();
    curThread->dev = deviceList.at(0);

    emit startTestTh();
}

void regObjToThread::doWork()
{
    try {
        emit resultReady((int)AbstractTest::Running);
        dev->tryToConnect();

        long it = 0, decrement = 0;
        if (inCycle == 0) { it=-1;  decrement=-1;   }

        int dsz;

        uint temp = (((range - addr + 1) / addrinc) * addrinc) + addr - 1 + 4;
        if (temp <= range)   dsz = ((range - addr + 1) / addrinc + 1)*4;
        else    dsz = ((range - addr + 1) / addrinc)*4;
        qDebug() << "dsz: " << dsz;

        QByteArray writeArray, answer;
        QByteArray readArray;

        if (mode == "w") {
            uint final;
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                if (inverse) final = ~j;    else    final = j;
                writeArray.append((char*)&i, 4);
                writeArray.append((char*)&final, 4);
            }
            for (; it<inCycle; it=it+1+decrement) {
                qDebug() << "writeArray size: " << writeArray.size();
                dev->write_F1(writeArray.data(), dsz*2);
                if (pause_stop() == -1) throw QString("stopped");
            }
            throw QString("finish");
        } else if (mode == "r") {
            for (uint i=addr; i+3<=range; i+=addrinc)
                readArray.append((char*)&i, 4);
            answer.resize(readArray.size());

            for (; it<inCycle; it=it+1+decrement) {
                dev->read_F1(readArray.data(), answer.data(), readArray.size());
                for (int i=0; i<answer.size(); i+=4) {
                    if (output) outputReady("Read: " + QString::number((int)*(int*)(answer.data()+i), 16));
                }
                if (pause_stop() == -1) throw QString("stopped");
            }
            throw QString("finish");
        } else if (mode == "wr") {
            uint final;
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                if (inverse) final = ~j;    else    final = j;
                writeArray.append((char*)&i, 4);
                writeArray.append((char*)&final, 4);
                readArray.append((char*)&i, 4);
            }
            answer.resize(readArray.size());

            ulong same=0, diff=0;
            for (; it<inCycle; it=it+1+decrement) {
                dev->write_F1(writeArray.data(), writeArray.size());

                same=0; diff=0;
                uint w,r;
                dev->read_F1(readArray.data(), answer.data(), readArray.size());
                for (int i=0, j=12; i<answer.size(); i+=4, j+=8) {
                    w = *(int*)(writeArray.data()+j);
                    r = *(int*)(answer.data()+i);
                    if (output) emit outputReady(tr("Write: %1; Read: %2").arg(QString::number(w, 16)).arg(QString::number(r, 16)));
                    if (w != r) diff++; else same++;
                }
                emit outputReady(tr("Write!=Read: %1;    Write==Read: %2").arg(QString::number(diff)).arg(QString::number(same)));
                if (pause_stop() == -1) throw QString("stopped");
            }
            if (!diff)
                throw QString("finish");
            else
                throw QString("finishWithErrors");
        }
        throw QString("finish");
    } catch (const QString& exception) {
        if (exception == "connection") {
            if (pause_stop() == -1) return;
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
        } else if (exception == "finishWithErrors") {
            dev->tryToDisconnect();
            emit resultReady(AbstractTest::TestFault);
        }
    }
}
