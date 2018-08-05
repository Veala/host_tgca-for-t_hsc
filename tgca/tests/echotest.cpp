#include "abstracttest.h"

void EchoTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,tB);
    echo = settings->findChild<QLineEdit *>("echo");
    deviceEdit = settings->findChild<QLineEdit*>("device");
    deviceLineEditList.append(deviceEdit);
top_1
    echo->setText(out.readLine());
    deviceEdit->setText(out.readLine());
    settingsFile.close();

    checkDeviceAvailability(1111);
}

void EchoTest::save(bool b)
{
    AbstractTest::save(b);
top_2(saveFileNameStr)
    in << echo->text() << endl;
    in << deviceEdit->text() << endl;
    settingsFile.close();
}

void EchoTest::startTest()
{
    deviceList.at(0);
    Device *dev = deviceList.at(0);

    if (dev==NULL) {
        qDebug() << "dev NULL";
        return;
    }

    int cmd = 5;  int dsz = echo->text().size()+1;
    QByteArray data;
    data.append((char*)&cmd,4);
    data.append((char*)&dsz,4);
    data.append(echo->text().toStdString().c_str(), dsz);
    dev->rw_socket.write(data, dsz+8);
}
