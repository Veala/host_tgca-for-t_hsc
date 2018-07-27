#include "abstracttest.h"

AbstractTest::AbstractTest(QWidget *parent) : QFrame(parent)
{
    QAction *act;
    act = menu.addAction(tr("Старт"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(startTest(bool)));
    act = menu.addAction(tr("Настройки..."));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showSettings(bool)));
    act = menu.addAction(tr("Сохранить"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(save(bool)));
    act = menu.addAction(tr("Создать копию..."));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(createCopy(bool)));
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

void AbstractTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName)
{
    devices=b;
    settings=d;
    settings->setWindowTitle(QObject::tr("Настройки"));
    name_enabled->setChecked(ch);
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

void AbstractTest::createCopy(bool)
{

}

EchoTest::EchoTest(QWidget *parent) : AbstractTest(parent)
{

}

void EchoTest::save(bool)
{


}

void EchoTest::startTest(bool b)
{
    QList <Device*> devList;
    for (int i=0; i<devices->count(); i++)
        devList << (Device*)devices->itemAt(i)->widget();
    Device* dev = devList.at(0);
    QLineEdit *echo = settings->findChild<QLineEdit *>("echo");

    int cmd = 5;  int dsz = echo->text().size()+1;
    QByteArray data;
    data.append((char*)&cmd,4);
    data.append((char*)&dsz,4);
    data.append(echo->text().toStdString().c_str(), dsz);
    dev->rw_socket.write(data, dsz+8);

    //qDebug() << "echo wrote: " << dev->rw_socket.write(data, dsz+8);

}

MemTest::MemTest(QWidget *parent) : AbstractTest(parent)
{

}

void MemTest::save(bool)
{

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

    QComboBox *mode = settings->findChild<QComboBox*>("mode");
    QLineEdit *startAddr = settings->findChild<QLineEdit *>("startAddress");
    QLineEdit *endAddr = settings->findChild<QLineEdit *>("endAddress");
    QSpinBox *incAddr = settings->findChild<QSpinBox *>("incAddress");
    QLineEdit *startData = settings->findChild<QLineEdit *>("startData");
    QSpinBox *incData = settings->findChild<QSpinBox *>("incData");
    QComboBox *inversion = settings->findChild<QComboBox*>("inverse");
    QSpinBox *iteration = settings->findChild<QSpinBox *>("iteration");
    QComboBox *oput = settings->findChild<QComboBox*>("output");
    QComboBox *device = settings->findChild<QComboBox*>("device");

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

RegTest::RegTest(QWidget *parent) : AbstractTest(parent)
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

    QString newFileStr = QFileDialog::getSaveFileName(0, tr("Введите имя файла"), tr(""), tr("tgca_c file (*.tgca_c)"));
    if (newFileStr.isEmpty())
        return NULL;

    QString defFileStr;
    if (testType == QObject::tr("Тест памяти")) {
        defFileStr = tr("../default/mem_test.tgca_t");
    } else if (testType == QObject::tr("Тест регистров")) {
        defFileStr = tr("../default/reg_test.tgca_t");
    } else if (testType == QObject::tr("Тест \"Эхо\"")) {
        defFileStr = tr("../default/echo_test.tgca_t");
    }

    if(!QFile::copy(defFileStr,newFileStr))
        return NULL;

    return loadTest(newFileStr, devices);
}

AbstractTest *testLib::loadTest(QString file, QVBoxLayout *devices)
{
    QFile txtFile(file);
    if (!txtFile.open(QFile::ReadOnly))
        return NULL;

    QTextStream out(txtFile);
    QString testType = out.readLine();
    bool ch = out.readLine().toUShort();

    QUiLoader loader;
    QDialog* settingsForm = (QDialog*)loader.load(&uiFile);
    QString test = settingsForm->property("TypeTest").toString();

    txtFile.close();
    if (test == QObject::tr("Тест памяти")) {
        return new MemTest(0, settingsForm, devices, file);
    } else if (test == QObject::tr("Тест регистров")) {
        return new RegTest(0, settingsForm, devices, file);
    } else if (test == QObject::tr("Тест \"Эхо\"")) {
        return new EchoTest(0, settingsForm, devices, file);
    } else {
        qDebug() << "unknown test";
    }




//    QString uiFileStr;
//    QString defFileStr;
//    AbstractTest* test;
//    if (testType == QObject::tr("Тест памяти")) {
//        defFileStr = tr("../default/mem_test.tgca_t");
//        uiFileStr = tr("../default/settings_mem_test.ui");
//        test = new MemTest(0);
//    } else if (testType == QObject::tr("Тест регистров")) {
//        defFileStr = tr("../default/reg_test.tgca_t");
//        uiFileStr = tr("../default/settings_reg_test.ui");
//        test = new RegTest(0);
//    } else if (testType == QObject::tr("Тест \"Эхо\"")) {
//        defFileStr = tr("../default/echo_test.tgca_t");
//        uiFileStr = tr("../default/settings_echo_test.ui");
//        test = new EchoTest(0);
//    }

//    if(!QFile::copy(defFileStr,newFileStr))
//        return NULL;

//    QUiLoader loader;
//    QFile uiFile(uiFileStr);
//    if (!uiFile.open(QFile::ReadOnly))
//        return NULL;
//    QDialog* settingsForm = (QDialog*)loader.load(&uiFile);
//    uiFile.close();

//    test->setSettings(devices, settingsForm, );
//    return test;

}
