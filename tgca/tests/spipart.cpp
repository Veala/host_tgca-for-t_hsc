#include "spipart.h"

void SpiPart::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    table = settings->findChild<QTableWidget*>("table");
    linesEdit = settings->findChild<QLineEdit*>("lines");
    cycleChBox = settings->findChild<QCheckBox*>("cycle");
    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1
    int rows = 32, columns = 2;
    for (int i=0; i<rows; i++)
    {
        table->insertRow(i);
        for (int j=0; j<columns; j++) {
            QLineEdit* line = new QLineEdit(out.readLine());
            line->setInputMask("HHHH;_");
            table->setCellWidget(i,j,line);
//            table->setItem(i,j,new QTableWidgetItem(out.readLine()));
        }
    }
    linesEdit->setText(out.readLine());
    cycleChBox->setChecked(out.readLine().toInt());
    deviceEdit->setText(out.readLine());
    settingsFile.close();
    checkDeviceAvailability(1111);

    objToThread = new spiObjToThread();
    objToThread->moveToThread(&testThread);
    connect(&testThread,SIGNAL(finished()), objToThread,SLOT(deleteLater()));
    connect(objToThread,SIGNAL(resultReady(int)), this, SLOT(setRunningState(int)));
    connect(objToThread,SIGNAL(outputReady(QString)), this, SLOT(testOutout(QString)));
    connect(this,SIGNAL(startTestTh()), objToThread, SLOT(doWork()));
    testThread.start();
}

void SpiPart::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    int rows = 32, columns = 2;
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<columns; j++)
//            in << table->item(i,j)->data(Qt::DisplayRole).toString() << endl;
            in << ((QLineEdit*)table->cellWidget(i,j))->text() << endl;
    }
    in << linesEdit->text() << endl;
    in << cycleChBox->isChecked() << endl;
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void SpiPart::startTest()
{
    spiObjToThread* curThread = (spiObjToThread*)objToThread;
    for (int i=0; i<32; i++)
    {
//        curThread->addr.append(table->item(i,0)->data(Qt::DisplayRole).toString().toUInt(nullptr, 16));
//        curThread->data.append(table->item(i,1)->data(Qt::DisplayRole).toString().toUInt(nullptr, 16));
        curThread->addr.append(((QLineEdit*)table->cellWidget(i,0))->text().toUInt(nullptr, 16));
        curThread->data.append(((QLineEdit*)table->cellWidget(i,1))->text().toUInt(nullptr, 16));
    }

    curThread->dev = deviceList.at(0);
    curThread->lines = linesEdit->text().toInt();
    curThread->cycle = cycleChBox->isChecked();

    emit startTestTh();
}

void spiObjToThread::doWork()
{
    try {
        QString ip = dev->connection.getServerIP();
        ushort port = dev->connection.getServerPORT().toUShort();

        tcpSocket.connectToHost(QHostAddress(ip), port);
        if (!tcpSocket.waitForConnected(5000)) {
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            tcpSocket.abort();
            return;
        }
        dev->setSocket(&tcpSocket);

        emit resultReady((int)AbstractTest::Running);

        QVector<BaseReg*> regs;
        regs.append(&dev->reg_hsc_dr_spi_lsw);
        regs.append(&dev->reg_hsc_cr_spi);

        do {
            for (int i=0; i<lines; i+=2)
            {
                dev->reg_hsc_dr_spi_lsw.data=data.at(i);
                *(((quint32*)&dev->reg_hsc_cr_spi)+1) = data.at(i+1);
                dev->writeRegs(regs);
                int n = 0;
                while (1) {
                    dev->readReg(&dev->reg_hsc_cr_spi);
                    if (dev->reg_hsc_cr_spi.spif == 1) {
                        emit outputReady(tr("reg_cr_spi.spif == %1").arg(dev->reg_hsc_cr_spi.spif,0,16));
                        break;
                    } else {
                        n++;
                        emit outputReady(tr("reg_cr_spi.spif != %1").arg(dev->reg_hsc_cr_spi.spif,0,16));
                        if (n>1000) {
                            tcpSocket.abort();
                            emit resultReady((int)AbstractTest::ErrorIsOccured);
                            return;
                        }
                    }
                }

                //threadState = AbstractTest::Paused;
                if (pause_stop() == -1) {
                    tcpSocket.abort();
                    return;
                }
            }
        } while (cycle);

        tcpSocket.abort();
        emit outputReady(tr("SPI загружено"));
        emit resultReady(AbstractTest::Completed);
    } catch(const QString& exception) {
        if (exception == "socket")
            emit resultReady((int)AbstractTest::ErrorIsOccured);
    }
}
