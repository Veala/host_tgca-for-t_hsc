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
    void disableUnimplemented();
    void setFieldConnections();

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
    // QCheckBox *checkBoxOut;   поле определено в CommonTest
    // скорость
    QComboBox *comboBoxSpeed;
    // QLineEdit *lineEditOver;  поле определено в CommonTest
    QLabel *labelOver;
    // регистры
    QCheckBox *checkBoxInit;
    QCheckBox *checkBoxCodec;
    // QCheckBox *checkBoxEnaAddr;  поле определено в CommonTest
    // QCheckBox *checkBoxEnaInt;  поле определено в CommonTest
    QCheckBox *checkBoxRTALoad;
    QComboBox *comboBoxManType;
    // QComboBox *comboBoxRTA;  адрес ОУ поле определено в CommonTest
    QCheckBox *checkBoxConfRegLoad;
    // проверки
    QComboBox *comboBoxWrongCfgReg;
    QComboBox *comboBoxErrRegComp;
    QComboBox *comboBoxErrStatusRT;
    QComboBox *comboBoxErrStatusBC;
    QComboBox *comboBoxBCIntErr;
    QComboBox *comboBoxCheckRTA;
    // паузы  определены в классе CommonTest
    //QLineEdit *lineEditReservePause;
    //QLineEdit *lineEditTime;
    //QLineEdit *lineEditPause;
    // прерывание
    // QCheckBox *checkBoxUseInt;   поле определено в CommonTest
    // QLabel *labelUseInt;   поле определено в CommonTest

private slots:
    void statsLongLongOutput(QString, quint64);
    void onRadioCycle();
};


class noiseObjToThread : public commonObjToThread
{
    Q_OBJECT

public:
    noiseObjToThread() {}

    int trbit;
    void *testData;
    long dataSize;
    quint16 numOFDM;
    int numByteInPackage;
    int rtaddr;
    bool initEnable, writeCfg;
    int iterCycle;
    int timeMeasure;
    int timeOverhead;
    int checkStatusErrBC;
    bool checkStatusErrRT, checkLoadCfg, checkRTA, noIntFatalBC, compEnableReg;
    bool loadRTA;
    int outMode;

    QString manipulation;
    bool codec;

public slots:
    void doWork();

protected:
    void perform();

signals:
    void statsOutputReadyLongLong(QString,quint64);

private:
    int errCounter;
    quint32 wrongBit;

    bool checkStatusRegBC(int status, int interr, int iter, bool *error);
    void checkStatusRT(REG_HSC_status &status, int iter, bool *error);
    void printCurrInfo(int iter, quint64 curbit, quint64 prevbit, bool err);
    bool outInfoEnable(quint64 curbit, quint64 prevbit, bool err) const;
    bool outTimeEnable(quint64 curbit, quint64 prevbit) const;
    void averageSpeed(long time_ms, long wholePackBits, int iter);
};

#endif // NOISETEST_H
