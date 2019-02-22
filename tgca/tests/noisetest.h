#ifndef NOISETEST_H
#define NOISETEST_H

#include "commontest.h"

class NoiseTest : public CommonTest
{
    Q_OBJECT

public:
    explicit NoiseTest(QWidget *parent = 0) : CommonTest(parent) {}
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    void save();
    void statsToZero();
protected:
    void startTest();
    void setEnabledSpecial(bool b);

private:
    void defineFields();
    void load(QString fName);
    void disableUnused();

    // основные
    QComboBox *comboBoxTestType;  // тип команды
    QLineEdit *lineEditNumS;      // длина пакета
    QComboBox *comboBoxExitRule;  // условие выхода
    // зацикливание
    QRadioButton *radioButtonSingle;
    QRadioButton *radioButtonUnlimited;
    QRadioButton *radioButtonEnter;
    QSpinBox *spinBoxCycle;
    // вывод
    QCheckBox *checkBoxParamView;
    QLabel *labelParamView;
    QComboBox *comboBoxShow;
    QCheckBox *checkBoxOut;
    // скорость
    QComboBox *comboBoxSpeed;
    QLineEdit *lineEditOver;
    QLabel *labelOver;
    // регистры
    QCheckBox *checkBoxInit;
    QCheckBox *checkBoxCodec;
    QCheckBox *checkBoxEnaAddr;
    QCheckBox *checkBoxEnaInt;
    QCheckBox *checkBoxRTALoad;
    QComboBox *comboBoxManType;
    QComboBox *comboBoxRTA;
    QCheckBox *checkBoxConfRegLoad;
    // проверки
    QComboBox *comboBoxWrongCfgReg;
    QComboBox *comboBoxErrRegComp;
    QComboBox *comboBoxErrStatusRT;
    QComboBox *comboBoxErrStatusBC;
    QComboBox *comboBoxBCIntErr;
    QComboBox *comboBoxCheckRTA;
    // паузы
    QLineEdit *lineEditWinModePause;
    QLineEdit *lineEditTime;
    QLineEdit *lineEditPause;
    // прерывание
    QCheckBox *checkBoxUseInt;
    QLabel *labelUseInt;
};


class noiseObjToThread : public commonObjToThread
{
    Q_OBJECT

public:
    noiseObjToThread() {}

    int trbit;
    void *testData;
    long dataSize;
    quint32 waitTime, pauseTime, delayTime;
    int num_s;
    int rtaddr;
    bool useInt, initEnable, writeCfg;
    int iterCycle;
    bool checkLoadCfg;
    int timeMeasure;
    int timeOverhead;
    bool checkStatusErrBC, checkStatusErrRT;
    bool checkRTA;
    /*
    int nwrd;
    bool statusBCOut, statusRTOut, windowMode;
    bool noIntFatalBC;
    int noIntFatalRT;
    int compEnableReg, compEnableData;
    int perOutBC, modeOutBC, perOutRT, modeOutRT;
    QString manipulation;
    bool codec;
*/
protected:
    void perform();
    void terminate(int);
};

#endif // NOISETEST_H
