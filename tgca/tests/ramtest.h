#ifndef RAMTEST_H
#define RAMTEST_H

#include "commontest.h"

class RamTest : public CommonTest
{
    Q_OBJECT
public:
    explicit RamTest(QWidget *parent = 0) : CommonTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    void save();
protected:
    void startTest();
    int updateDeviceList();
    void setEnabledSpecial(bool) {}

private:
    QLineEdit *lineEditShift;
    QComboBox *comboBoxNBufBC;
    QComboBox *comboBoxBufDirBC;
    QComboBox *comboBoxBufDirRT;
    QComboBox *comboBoxNBufRT;
    QLabel *labelBegAddrBC;
    QLabel *labelEndAddrBC;
    QLabel *labelBegAddrRT;
    QLabel *labelEndAddrRT;
    QLineEdit *lineEditNum;
    QComboBox *comboBoxDev;

    uint begAddrBC();
    uint begAddrRT();

private slots:
    void resetFlds();
};

class ramObjToThread : public commonObjToThread
{
    Q_OBJECT

public:
    ramObjToThread();
    uint addrBC, addrRT, rangeBC, rangeRT;

protected:
    void perform();
};

#endif // RAMTEST_H
