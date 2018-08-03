#include "abstracttest.h"

void MemTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,tB);

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
}

void MemTest::save(bool b)
{
    AbstractTest::save(b);
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

void MemTest::startTest(bool b)
{
    if (getState() != AbstractTest::ItIsOk) {
        message(tr("Ошибка: проблема с устройствами теста - %1").arg(fileName->text()));
        return;
    }

    deviceList.at(0);
    Device *dev = deviceList.at(0);

    if (dev==NULL) {
        qDebug() << "dev NULL";
        return;
    }

    uint addr = startAddr->text().toUInt(NULL, 16);
    uint range = endAddr->text().toUInt(NULL, 16);
    uint addrinc = incAddr->value();
    uint data = startData->text().toUInt(NULL, 16);
    uint datainc = incData->value();
    uint inverse = inversion->currentText().toUInt();
    long inCycle = iteration->value();
    uint output = oput->currentText().toUInt();

    long it = 0, decrement = 0;
    if (inCycle == 0) { it=-1;  decrement=-1;   }

    int cmd, dsz;

    uint temp = (((range - addr + 1) / addrinc) * addrinc) + addr - 1 + 4;
    if (temp <= range)   dsz = ((range - addr + 1) / addrinc + 1)*4;
    else    dsz = ((range - addr + 1) / addrinc)*4;
    qDebug() << "dsz: " << dsz;

    QByteArray array;

    if (mode->currentText() == "w") {
        cmd = 1; dsz=dsz*2;
        array.append((char*)&cmd, 4);
        array.append((char*)&dsz, 4);
        for (; it<inCycle; it=it+1+decrement) {
            uint final;
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                if (inverse) final = ~j;    else    final = j;
                array.append((char*)&i, 4);
                array.append((char*)&final, 4);
            }
        }
        qDebug() << "array size: " << array.size();
        qDebug() << "wrote: " << dev->rw_socket.write(array, array.size());
    } else if (mode->currentText() == "r") {
        cmd = 3;
        for (; it<inCycle; it=it+1+decrement) {
            dev->rw_socket.write((char*)&cmd, 4);
            dev->rw_socket.write((char*)&dsz, 4);
            for (uint i=addr; i+3<=range; i+=addrinc) //{
                dev->rw_socket.write((char*)&i, 4);
            for (uint i=addr; i+3<=range; i+=addrinc) {
                //dev->rw_socket.waitForReadyRead();
                dev->rw_socket.read((char*)&data, 4);
                if (output) textBrowser->append("Read: " + QString::number(data, 16));
            }
        }
    } else if (mode->currentText() == "wr") {
        cmd = 1; dsz=dsz*2;
        array.append((char*)&cmd, 4);
        array.append((char*)&dsz, 4);
        for (; it<inCycle; it=it+1+decrement) {
            uint final;
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                if (inverse) final = ~j;    else    final = j;
                array.append((char*)&i, 4);
                array.append((char*)&final, 4);
            }
        }
        qDebug() << "array size: " << array.size();
        qDebug() << "wrote: " << dev->rw_socket.write(array, array.size());

        it = 0, decrement = 0;
        if (inCycle == 0) { it=-1;  decrement=-1;   }

        cmd = 3; dsz=dsz/2;
        for (; it<inCycle; it=it+1+decrement) {
            ulong same=0, diff=0;
            uint final;
            dev->rw_socket.write((char*)&cmd, 4);
            dev->rw_socket.write((char*)&dsz, 4);
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc)
                dev->rw_socket.write((char*)&i, 4);
            //dev->rw_socket.waitForReadyRead();
            for (uint i=addr, j=data; i+3<=range; i+=addrinc, j+=datainc) {
                //dev->rw_socket.waitForReadyRead();
                dev->rw_socket.read((char*)&data, 4);
                if (inverse) final = ~j;    else    final = j;
                if (output) textBrowser->append(tr("Write: %1; Read: %2").arg(QString::number(final, 16)).arg(QString::number(data, 16)));
                if (data != final) diff++; else same++;
            }
            textBrowser->append(tr("Write!=Read: %1;    Write==Read: %2").arg(QString::number(diff)).arg(QString::number(same)));
        }
    }
}
