#ifndef REGTEST_H
#define REGTEST_H

#include "abstracttest.h"

class RegTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit RegTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QComboBox *mode, *inversion, *oput;
    QLineEdit *startAddr, *endAddr, *startData, *deviceEdit;
    QSpinBox *incAddr, *incData, *iteration;
};

class regObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    uint addr, range, addrinc, data, datainc, inverse, output;
    long inCycle;
    QString mode;
    QTcpSocket tcpSocket;
    Device *dev;
};

#endif // REGTEST_H
