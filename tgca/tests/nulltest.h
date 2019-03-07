#ifndef NULLTEST_H
#define NULLTEST_H

#include "commontest.h"

class NullTest : public CommonTest
{
    Q_OBJECT
public:
    explicit NullTest(QWidget *parent = 0) : CommonTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    void startTest();
    void setEnabledSpecial(bool );

private:
    // зацикливание
    QSpinBox  *spinBoxCycle;            // число итераций
    QRadioButton *radioButtonUnlimited; // длина цикла не ограничена
    QRadioButton *radioButtonEnter;     // длина цикла задаётся параметром spinBoxCycle

    QLineEdit *lineEditSleepTime;    // пауза, которую должна определить функция измерения времени
    QLineEdit *lineEditErrBit;       // число внесенных ошибочных бит
    QComboBox *comboBoxManType;
    QCheckBox *checkBoxCodec;

    // пауза между итерациями
    QLineEdit *lineEditPause;

    // соединения
    QCheckBox *checkBoxConnRT;
    QCheckBox *checkBoxConnBC;

    // вывод
    QComboBox *comboBoxOut;

    void defineFields();
    void load(QString fName);

private slots:
    void onRadioCycle();
};

class nullObjToThread : public commonObjToThread
{
    Q_OBJECT

public:
    nullObjToThread() : commonObjToThread(), testData(0) {}

    uint iterCount, iterTime, outMode;
    bool bConnectBC, bConnectRT, bDebugOut;
    void *testData;

    bool timeTest;
    int timeSleep;
    bool compTest;
    int wrongBit;
    int manType;
    bool codec;

protected:
    void stdOutput(QString message_rus, QString message_eng);
    void perform();
};

#endif // NULLTEST_H
