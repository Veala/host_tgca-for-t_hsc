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
    QComboBox *comboBoxAmpl;
    QCheckBox *checkBoxEnaInt;

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

    unsigned int maxNumByte();

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
    virtual void doWork() {}
public:
    uint command, cfg, ampl, time, rta;
    bool broad, useInt, output;
    char* data;
    long inCycle;
    QString mode;
    int socketDescBC, socketDescRT;
    Device *devBC, *devRT;
};

#endif // TRMSINGLETEST_H
