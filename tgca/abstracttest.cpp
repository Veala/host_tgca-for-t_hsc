#include "abstracttest.h"

AbstractTest::AbstractTest(QWidget *parent) : QFrame(parent)
{
    QAction *act;
    act = menu.addAction(tr("Старт"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(startTest(bool)));
    act = menu.addAction(tr("Настройки..."));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showSettings(bool)));
    act = menu.addAction(tr("Сохранить")); act->setObjectName(tr("saveObj"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(save(bool)));
//    act = menu.addAction(tr("Создать копию...")); act->setObjectName(tr("saveAsObj"));
//    connect(act, SIGNAL(triggered(bool)), this, SLOT(save(bool)));
    act = menu.addAction(tr("Удалить"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteProc(bool)));

    layout = new QHBoxLayout(this);
    name_enabled = new QCheckBox(tr("Тест"));
    //name_enabled->setLayoutDirection(Qt::RightToLeft);
    layout->addWidget(name_enabled);
    fileName = new QLabel(tr(""));
    layout->addWidget(fileName);
    layout->addSpacerItem(new QSpacerItem(10,10));
    status = new QLabel(tr("Статус"));
    status->setAlignment(Qt::AlignRight);
    layout->addWidget(status);

    setFrameStyle(QFrame::Box);
}

AbstractTest::~AbstractTest()
{
    delete settings;
}

void AbstractTest::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        menu.exec(QCursor::pos());
    } else if (event->buttons() == Qt::LeftButton) {

    }
}

void AbstractTest::setSettings(QVBoxLayout *b, QDialog *d, bool ched, QString tType, QString fName)
{
    devices=b;  settings=d;
    settings->setWindowTitle(QObject::tr("Настройки"));
    name_enabled->setChecked(ched);
    name_enabled->setText(tType);
    fileName->setText(fName);
}

void AbstractTest::showSettings(bool)
{
    settings->exec();
}

void AbstractTest::deleteProc(bool)
{
    deleteLater();
}

void AbstractTest::save(bool)
{
    QString saveSndrName = sender()->objectName();
    if (saveSndrName == "saveObj") {
        saveFileNameStr = fileName->text();
    } else if (saveSndrName == "saveAsObj") {
        saveFileNameStr = QFileDialog::getSaveFileName(this, QObject::tr("Введите имя файла"), QObject::tr(""));
        if (saveFileNameStr.isEmpty()) {
            return;
        }
    }
}

void EchoTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName)
{
    AbstractTest::setSettings(b,d,ch,tType,fName);
    echo = settings->findChild<QLineEdit *>("echo");
    device = settings->findChild<QComboBox*>("device");
top_1
    echo->setText(out.readLine());
    device->setCurrentText(out.readLine());
    settingsFile.close();
}

void EchoTest::save(bool b)
{
    AbstractTest::save(b);
top_2(saveFileNameStr)
    in << echo->text() << endl;
    in << device->currentText() << endl;
    settingsFile.close();
}

void EchoTest::startTest(bool b)
{
    QList <Device*> devList;
    for (int i=0; i<devices->count(); i++)
        devList << (Device*)devices->itemAt(i)->widget();
    Device* dev = devList.at(0);

    int cmd = 5;  int dsz = echo->text().size()+1;
    QByteArray data;
    data.append((char*)&cmd,4);
    data.append((char*)&dsz,4);
    data.append(echo->text().toStdString().c_str(), dsz);
    dev->rw_socket.write(data, dsz+8);
}

void MemTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName)
{
    AbstractTest::setSettings(b,d,ch,tType,fName);

    mode = settings->findChild<QComboBox*>("mode");
    startAddr = settings->findChild<QLineEdit *>("startAddress");
    endAddr = settings->findChild<QLineEdit *>("endAddress");
    incAddr = settings->findChild<QSpinBox *>("incAddress");
    startData = settings->findChild<QLineEdit *>("startData");
    incData = settings->findChild<QSpinBox *>("incData");
    inversion = settings->findChild<QComboBox*>("inverse");
    iteration = settings->findChild<QSpinBox *>("iteration");
    oput = settings->findChild<QComboBox*>("output");
    device = settings->findChild<QComboBox*>("device");
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
    device->setCurrentText(out.readLine());
    settingsFile.close();
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
    in << device->currentText() << endl;
    settingsFile.close();
}

void MemTest::startTest(bool b)
{
    qDebug() << "strat test";
    //QList<Device*> allDevWidgets = devices->findChildren<Device*>();
    //qDebug() << allDevWidgets.count();
    //Device *dev = allDevWidgets.at(0);
    Device *dev = (Device*)devices->itemAt(0)->widget();

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
    QString devName = device->currentText();

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
            for (uint i=addr; i+3<=range; i+=addrinc)
                dev->rw_socket.write((char*)&i, 4);
            for (uint i=addr; i+3<=range; i+=addrinc) {
                dev->rw_socket.read((char*)&data, 4);
                if (output) qDebug() << "Read: " << data;
            }
        }
    } else if (mode->currentText() == "wr") {
        //write_read();
    }
}

void RegTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName)
{

}

void RegTest::save(bool)
{

}

void RegTest::startTest(bool b)
{

}

AbstractTest *testLib::createTest(QVBoxLayout *devices)
{
    QStringList allTests;
    allTests << QObject::tr("Тест \"Эхо\"") << QObject::tr("Тест памяти") << QObject::tr("Тест регистров");

    bool ok;
    QString testType = QInputDialog::getItem(0, QObject::tr("Создать тест"), QObject::tr("Тесты:"), allTests, 0, false, &ok);
    if (!(ok && !testType.isEmpty()))
        return NULL;

    QString newFileStr = QFileDialog::getSaveFileName(0, QObject::tr("Введите имя файла"), QObject::tr(""));
    if (newFileStr.isEmpty()) {
        return NULL;
    }
    QString defFileStr;

    if (testType == QObject::tr("Тест памяти")) {
        defFileStr = QObject::tr("../default/mem_test");
    } else if (testType == QObject::tr("Тест регистров")) {
        defFileStr = QObject::tr("../default/reg_test");
    } else if (testType == QObject::tr("Тест \"Эхо\"")) {
        defFileStr = QObject::tr("../default/echo_test");
    }

    if(!QFile::copy(defFileStr,newFileStr))
        return NULL;

    return loadTest(newFileStr, devices);
}

AbstractTest *testLib::loadTest(QString settingsFileStr, QVBoxLayout *devices)
{
    QFile settingsFile(settingsFileStr);
    if (!settingsFile.open(QFile::ReadOnly))
        return NULL;

    QTextStream out(&settingsFile);
    QString testType = out.readLine();
    bool checked = out.readLine().toInt();
    settingsFile.close();

    QString uiFileStr;
    AbstractTest* test;
    if (testType == QObject::tr("Тест памяти")) {
        uiFileStr = QObject::tr("../default/settings_mem_test.ui");
        test = new MemTest(0);
    } else if (testType == QObject::tr("Тест регистров")) {
        uiFileStr = QObject::tr("../default/settings_reg_test.ui");
        test =  new RegTest(0);
    } else if (testType == QObject::tr("Тест \"Эхо\"")) {
        uiFileStr = QObject::tr("../default/settings_echo_test.ui");
        test = new EchoTest(0);
    } else {
        qDebug() << "unknown test";
        return NULL;
    }

    QUiLoader loader;
    QFile uiFile(uiFileStr);
    if (!uiFile.open(QFile::ReadOnly))
        return NULL;
    QDialog* settingsForm = (QDialog*)loader.load(&uiFile);
    uiFile.close();

    test->setSettings(devices, settingsForm, checked, testType, settingsFileStr);
    return test;
}
