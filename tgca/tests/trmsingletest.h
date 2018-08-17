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

    unsigned int mnb;
    unsigned int maxNumByte();
    void recalc();

private slots:
    void onRadio();
    void onCodec();
    void onManType();
    void done1(int);
};

class trmSingleObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    uint command, cfgBC, cfgRT, amplBC, amplRT, time, rta;
    bool broad, useInt, output;
    char* data;
    long inCycle;
    QString mode;
    QTcpSocket tcpSocketBC, tcpSocketRT;
    int socketDescBC, socketDescRT;
    Device *devBC, *devRT;
};

#endif // TRMSINGLETEST_H
