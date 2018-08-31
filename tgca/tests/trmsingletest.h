#ifndef TRMSINGLETEST_H
#define TRMSINGLETEST_H

#include "abstracttest.h"
#include <QRadioButton>

class TrmSingleTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit TrmSingleTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB);
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

    unsigned int mnb;
    unsigned int maxNumByte();
    void recalc();
    void updateDeviceList();

private slots:
    void onRadio();
    void onCodec();
    void onManType();
    void updateSettings();

signals:
    void settingsClosed(int);
};

class trmSingleObjToThread : public absObjToThread
{
    Q_OBJECT
    bool readReg(QTcpSocket& socket, int addr, int *val);
    bool checkStatusReg(QTcpSocket& socket, int *status);
    bool checkStatusRegBC(int *status);
    bool checkStatusRegRT(int *status);
    bool checkCounters(QTcpSocket* socket);

public slots:
    virtual void doWork();
    void terminate(int);
public:
    trmSingleObjToThread();

    uint cfgBC, cfgRT, amplBC, amplRT, time, data_size, iter;
    addr_t rtaddr;
    bool broad, useInt, outEnable, compEnable, initEnable;
    void* data;
    //char data[MAXPACKAGESIZE];
    QTcpSocket tcpSocketBC, tcpSocketRT;
    Device *devBC, *devRT;
};

#endif // TRMSINGLETEST_H
