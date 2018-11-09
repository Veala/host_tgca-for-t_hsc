#ifndef TRMSINGLETEST_H
#define TRMSINGLETEST_H

#include "abstracttest.h"
#include <QRadioButton>

class TrmSingleTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit TrmSingleTest(QWidget *parent = 0) : AbstractTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();

private:
    QLineEdit *lineEditLen;
    QComboBox *comboBoxRTA;
    QComboBox *comboBoxCode;
    QCheckBox *checkBoxBroad;

    QComboBox *comboBoxManType;
    QCheckBox *checkBoxCodec;
    QCheckBox *checkBoxEnaInt;
    QCheckBox *checkBoxEnaAddr;
    QComboBox *comboBoxAmplBC;
    QComboBox *comboBoxAmplRT;

    QRadioButton *radioButtonLin;
    QLineEdit *lineEditBegin;
    QLineEdit *lineEditStep;
    QLineEdit *lineEditNumStep;
    QComboBox *comboBoxUnit;

    QLineEdit *lineEditTime;
    QLineEdit *lineEditPause;
    QCheckBox *checkBoxUseInt;
    QCheckBox *checkBoxOut;

    QLineEdit *lineEditDevBC;
    QLineEdit *lineEditDevRT;
    QCheckBox *checkBoxDevBC;
    QCheckBox *checkBoxDevRT;

    QLabel *labelLen;
    QLabel *labelBegin;
    QLabel *labelStep;
    QLabel *labelNumStep;
    QRadioButton *radioButtonRand;

    QCheckBox *checkBoxCompare;
    QCheckBox *checkBoxInit;
    QSpinBox  *spinBoxCycle;

    QRadioButton *radioButtonSingle;
    QRadioButton *radioButtonUnlimited;
    QRadioButton *radioButtonEnter;

    QLineEdit *lineEditSPIData;
    QCheckBox *checkBoxConfRegLoad;
    QCheckBox *checkBoxRTALoad;
    QCheckBox *checkBoxSPILoad;
    QCheckBox *checkBoxCounters;
    QComboBox *comboBoxSPIdr;
    QComboBox *comboBoxSPIen;

    QCheckBox *checkBoxBCOut;
    QCheckBox *checkBoxRTOut;
//    QLabel    *labelBCStat;
//    QLabel    *labelRTStat;
//    QLabel    *labelCounters;
    QComboBox *comboBoxCountPref;
    QComboBox *comboBoxBCOutPref;
    QComboBox *comboBoxRTOutPref;
    QLineEdit *lineEditCountOut;
    QLineEdit *lineEditBCOut;
    QLineEdit *lineEditRTOut;

    QLabel *labelMarkFatalErr;
    QLabel *labelMarkStatBCErr;
    QLabel *labelMarkStatRTErr;
    QLabel *labelMarkCompErr;
    QCheckBox *checkBoxFatalErr;
    QCheckBox *checkBoxStatBCErr;
    QCheckBox *checkBoxStatRTErr;
    QCheckBox *checkBoxCompErr;
    QCheckBox *checkBoxWinMode;
    QLineEdit *lineEditWinModePause;

    unsigned int mnb;
    unsigned int maxNumByte();
    void recalc();
    void updateDeviceList();
    void setStatSettings();

private slots:
    void onRadioData();
    void onRadioCycle();
    void onCheckInit();
    void recalcMax();
    void updateSettings();

signals:
    void settingsClosed(int);
};

class trmSingleObjToThread : public absObjToThread
{
    Q_OBJECT
    //void checkStatusReg(Device *dev, int *status);
    int checkStatusRegBC();
    int checkStatusRegRT();
    void checkCounters(Device *dev);
    void stdOutput(QString);
    int readReg(Device* dev, int addr);

public slots:
    virtual void doWork();
    void terminate(int);
public:
    trmSingleObjToThread();

    uint cfgBC, cfgRT, amplBC, amplRT, waitTime, pauseTime, delayTime, data_size;
    int iterCycle;
    addr_t rtaddr;
    bool broad, useInt, outEnable, compEnable, initEnable, writeCfg, checkCountersEnable;
    bool statusBCOut, statusRTOut, exitOnNoFin, windowMode;
    int dataSPI, modeSPI;
    void* data;
    //char data[MAXPACKAGESIZE];
    QTcpSocket tcpSocketBC, tcpSocketRT;
    Device *devBC, *devRT;
};

#endif // TRMSINGLETEST_H
