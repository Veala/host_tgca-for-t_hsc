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
    void *createData(long numbyte, int numcopy);
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
    explicit CommonTest(QWidget *parent = 0);

    //virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
    QDialog *getSettings() { return settings; }
    void codesToLabel(QString codes, QLineEdit* line);

protected slots:
    void updateSettings();
    void statsToZero();
    void statsTestOutput(QString, long);

    void onMenuRTA(QPoint);
    void applyRTAddr();
    void onMenuDevBC(QPoint);
    void applyDevBC();
    void onMenuDevRT(QPoint);
    void applyDevRT();
    void onMenuTime(QPoint);
    void applyWaitTime();
    void onMenuPause(QPoint);
    void applyPause();
    void onMenuResPause(QPoint);
    void applyResPause();
    void onMenuOver(QPoint);
    void applyOverTime();
    void onMenuEnaInt(QPoint);
    void applyEnaInt();
    void onMenuUseInt(QPoint);
    void applyUseInt();
    void onMenuEnaAddr(QPoint);
    void applyEnaAddr();
    void onMenuOut(QPoint);
    void applyOutputMode();

protected:
    void addDevicesTolist();
    virtual int updateDeviceList();
    void setTestConnections();
    void changeConnections();
    void connectStatisticSlots();
    void connectThread();
    void statsAddlabel(QString str);

    dataGeneration dataGen;
    // общие поля для разных типов тестов
    QComboBox *comboBoxRTA;             // адрес ОУ
    QLineEdit *lineEditTime;            // максимальное время ожидания завершения цикла обмена, в т.ч. прерывания КШ и завершения обмена по SPI
    QLineEdit *lineEditPause;           // пауза между итерациями
    QLineEdit *lineEditReservePause;    // задержка для оконного режима
    QLineEdit *lineEditOver;            // значение, на которое надо уменьшить измереное время передачи данных: расходы на ethernet
    QCheckBox *checkBoxEnaInt;          // КШ и ОУ, проверяется пока только КШ
    QCheckBox *checkBoxEnaAddr;         // ОУ
    QCheckBox *checkBoxUseInt;          // КШ и ОУ, проверяется пока только КШ
    QLabel    *labelUseInt;
    QCheckBox *checkBoxOut;
    // устройства
    QLineEdit *lineEditDevBC;           // имя устройства, назначенного КШ
    QLineEdit *lineEditDevRT;           // имя устройства, назначенного ОУ
    QCheckBox *checkBoxDevBC;           // использовать КШ
    QCheckBox *checkBoxDevRT;           // использовать ОУ

signals:
    void settingsClosed(int);
private:
    void applyCurrent(QWidget* wid);
};

class commonObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    void doWork();
protected slots:
    virtual void terminate(int);
public:
    explicit commonObjToThread() : absObjToThread(), devBC(0), devRT(0) {}
    void setOutEnabled(bool b) { outEnable = b; }
    virtual void destroyData() {}
    Device *devBC, *devRT;
    int onPauseStop();
protected:
    bool outEnable;
    static const quint32 delayTime = 5;
    virtual void stdOutput(QString message_rus, QString message_eng);
    virtual void perform() = 0;
    void switchWindow(int n);
    void softReset(Device* dev);
    AbstractTest::RunningState connectBC();
    AbstractTest::RunningState connectRT();
    bool isRunning();
    void initStartBC();
signals:
    void statsOutputReady(QString,long);
};

inline void setRegWritten(Device *dev, BaseReg &reg)
{
    dev->configuration.setWritten(reg.address, reg.getData());
}

QString commmonTest_labelToCodes(QLineEdit* line);
void commmonTest_applyCodesToLabel(QLineEdit* line, QString str);

#endif // COMMONTEST_H
