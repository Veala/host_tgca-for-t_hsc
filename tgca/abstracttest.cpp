#include "abstracttest.h"

AbstractTest::AbstractTest(QWidget *parent, QDialog *d, QVBoxLayout *b) : QFrame(parent)
{
    QAction *act = menu.addAction(tr("Настройки..."));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showSettings(bool)));
    act = menu.addAction(tr("Удалить"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(deleteProc(bool)));

    layout = new QHBoxLayout(this);
    name_enabled = new QCheckBox(tr("Тест"));
    name_enabled->setLayoutDirection(Qt::RightToLeft);
    layout->addWidget(name_enabled);
    fileName = new QLabel(tr(""));
    layout->addWidget(fileName);
    layout->addSpacerItem(new QSpacerItem(10,10));
    status = new QLabel(tr("Статус"));
    layout->addWidget(status);

    settings=d;
    name_enabled->setChecked(settings->property("enabledTest").toBool());
    name_enabled->setText(settings->property("TypeTest").toString());

    devices=b;

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

}

void AbstractTest::setEnabled(bool en)
{
    //name_enabled->setEnabled(en);
}

MemTest::MemTest(QWidget *parent, QDialog *d, QVBoxLayout *b) : AbstractTest(parent, d, b)
{
//    name_enabled->setText(tr("Тест памяти"));
//    settings->setWindowTitle(tr("Настройки"));
//    QGridLayout* settingsLayout = new QGridLayout();
//    settingsLayout->addWidget(new QLabel(tr("Режим:")), 0, 0);
//    settingsLayout->addWidget(new QLabel(tr("Начальный адрес:")), 1, 0);
//    settingsLayout->addWidget(new QLabel(tr("Конечный адрес:")), 2, 0);
//    settingsLayout->addWidget(new QLabel(tr("Приращение адреса:")), 3, 0);
//    settingsLayout->addWidget(new QLabel(tr("Начальные данные:")), 4, 0);
//    settingsLayout->addWidget(new QLabel(tr("Приращение данных:")), 5, 0);
//    settingsLayout->addWidget(new QLabel(tr("Инверсия:")), 6, 0);
//    settingsLayout->addWidget(new QLabel(tr("Итераций:")), 7, 0);
//    settingsLayout->addWidget(new QLabel(tr("Вывод:")), 8, 0);
//    settingsLayout->addWidget(new QLabel(tr("Устройство:")), 9, 0);

//    modeBox.addItem("r"); modeBox.addItem("w"); modeBox.addItem("wr");
//    addrInc.setMinimum(0); addrInc.setMaximum(10000);
//    dataInc.setMinimum(0); dataInc.setMaximum(10000);
//    inverse.addItem("0"); inverse.addItem("1");
//    iteration.setMinimum(0); iteration.setMaximum(100000);
//    output.addItem("0"); output.addItem("1");

//    settingsLayout->addWidget(&modeBox, 0, 1);
//    settingsLayout->addWidget(&startAddr, 1, 1);
//    settingsLayout->addWidget(&endAddr, 2, 1);
//    settingsLayout->addWidget(&addrInc, 3, 1);
//    settingsLayout->addWidget(&startData, 4, 1);
//    settingsLayout->addWidget(&dataInc, 5, 1);
//    settingsLayout->addWidget(&inverse, 6, 1);
//    settingsLayout->addWidget(&iteration, 7, 1);
//    settingsLayout->addWidget(&output, 8, 1);
//    settingsLayout->addWidget(&deviceName, 9, 1);

//    settings->setLayout(settingsLayout);
}

void MemTest::startTest()
{
    QList<QWidget*> allDevWidgets = devices->findChildren<QWidget*>();
    Device *dev = (Device *)allDevWidgets.at(0);

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

    if (mode->currentText() == "w") {
        ushort cmd = 1;
        //ushort dsz = (range - addr) /
        for (; it<inCycle; it=it+1+decrement) {
            ulong final;
            //dev->rw_socket.write((char*))
            for (ulong i=addr, j=data; i<range; i+=addrinc, j+=datainc) {
                if (inverse) final = ~j;    else    final = j;

                //memcpy(virtual_base + i + delta, (void*)&final, n);
            }

        }
    } else if (mode->currentText() == "r") {
        //also_read();
    } else if (mode->currentText() == "wr") {
        //write_read();
    }
}

RegTest::RegTest(QWidget *parent, QDialog *d, QVBoxLayout *b) : AbstractTest(parent, d, b)
{

}

void RegTest::startTest()
{

}

AbstractTest *testLib::createTest(QVBoxLayout *devices)
{
    QStringList allTests;
    allTests << QObject::tr("Тест \"Эхо\"") << QObject::tr("Тест памяти") << QObject::tr("Тест регистров");

    bool ok;
    QString test = QInputDialog::getItem(0, QObject::tr("Создать тест"), QObject::tr("Тесты:"), allTests, 0, false, &ok);
    if (!(ok && !test.isEmpty())) return NULL;

    if (test == QObject::tr("Тест памяти")) {
        return new MemTest();
    } else if (test == QObject::tr("Тест регистров")) {
        return new RegTest();
    } else if (test == QObject::tr("Тест \"Эхо\"")) {

    }
}

AbstractTest *testLib::loadTest(QString file, QVBoxLayout *devices)
{
    QUiLoader loader;
    QFile uiFile(file);
    if (!uiFile.open(QFile::ReadOnly))
        return NULL;
    QDialog* settingsForm = (QDialog*)loader.load(&uiFile);
    QString test = settingsForm->property("TypeTest").toString();

    if (test == QObject::tr("Тест памяти")) {
        return new MemTest(0, settingsForm, devices);
    } else if (test == QObject::tr("Тест регистров")) {
        return new RegTest(0, settingsForm, devices);
    } else if (test == QObject::tr("Тест \"Эхо\"")) {

    }
}
