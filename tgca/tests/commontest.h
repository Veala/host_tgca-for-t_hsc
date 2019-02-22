#ifndef COMMONTEST_H
#define COMMONTEST_H

#include "abstracttest.h"
#include <QRadioButton>

class dataGeneration : public QObject
{
    Q_OBJECT
public:
    void settings();
    void setParentTest(AbstractTest *t) { test = t; }
    QDialog *parentTestSettings();
    void init(QString genDataType, QString genDataBegin, QString genDataStep,
              QString genDataNumStep, QString genDataUnit, QString genDataLen);
    void enable(bool b);
    void *createData(long numbyte);
    long getDataLen() const;

    QString genType() const;
    QString begin() const;
    QString step() const;
    QString numStep() const;
    QString unit() const;
    QString dataLen() const;

private:
    AbstractTest *test;
    // настройки генерации данных
    QRadioButton *radioButtonLin;       // выбор способа генерации: линейное со сбросом ("пила")
    QLineEdit *lineEditBegin;           // начальнон значение для генерации данных                           // не меняет маску (по сигналу от comboBoxUnit)
    QLineEdit *lineEditStep;            // приращение для линейных данных или минимум для случайных  // не меняет маску (по сигналу от comboBoxUnit)
    QLineEdit *lineEditNumStep;         // число шагов до сброса или максимум для случайных чисел  // не меняет маску (по сигналу от comboBoxUnit)
    QComboBox *comboBoxUnit;            // длина единицы данных в байтах: 1, 2 или 4
    QLineEdit *lineEditBufLen;          // размер массива данных в байтах: для генерации данных потребуется выделение памяти вдвое большего разменра
    QLabel *labelBegin;                 // меняется при переключении radioButtonRand-radioButtonLin
    QLabel *labelStep;                  // меняется при переключении radioButtonRand-radioButtonLin
    QLabel *labelNumStep;               // меняется при переключении radioButtonRand-radioButtonLin
    QRadioButton *radioButtonRand;      // выбор генерации данных: случайные

private slots:
    void onRadioData();
};

class CommonTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit CommonTest(QWidget *parent = 0) : AbstractTest(parent) { dataGen.setParentTest(this);}
    //virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
    QDialog *getSettings() { return settings; }
protected slots:
    void updateSettings();
    void statsToZero();
    void statsErrOutput(QString, long);
protected:
    void addDevicesTolist();
    virtual int updateDeviceList();
    void changeConnections();
    void assignDevices();
    void connectStats();
    void connectThread();
    void statsAddlabel(QString str);

    dataGeneration dataGen;
    // устройства
    QLineEdit *lineEditDevBC;           // имя устройства, назначенного КШ
    QLineEdit *lineEditDevRT;           // имя устройства, назначенного ОУ
    QCheckBox *checkBoxDevBC;           // использовать КШ
    QCheckBox *checkBoxDevRT;           // использовать ОУ

signals:
    void settingsClosed(int);
private:
};

class commonObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    void doWork();
protected slots:
//    void testTerminate(int);
    virtual void terminate(int) = 0;
public:
    explicit commonObjToThread() : absObjToThread(), devBC(0), devRT(0) {}
    void setIter(int num) { iterCount = num; }
    void setOutEnabled(bool b) { outEnable = b; }
    QTcpSocket tcpSocketBC, tcpSocketRT;
    Device *devBC, *devRT;
protected:
    uint iterCount;
    bool outEnable;
    virtual void stdOutput(QString message_rus, QString message_eng);
    virtual void perform() = 0;
    AbstractTest::RunningState connectBC();
    AbstractTest::RunningState connectRT();
//    virtual void terminate(int) = 0;
    bool isRunning();
signals:
    void statsOutputReadySimple(QString,long);
};

#endif // COMMONTEST_H
