#ifndef BULBTEST_H
#define BULBTEST_H

#include "abstracttest.h"

class BulbTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit BulbTest(QWidget *parent = 0) : AbstractTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    void save();
protected:
    void startTest();
private:
    QLineEdit *lineEditIterate, *lineEditInterval, *lineEditDevice;
    QCheckBox * checkBoxOut;
};

class bulbObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    void doWork();
public:
    void setIter(QLineEdit* ed) { iterCount = ed->text().toInt(); }
    void setPause(QLineEdit* ed) { iterTime = ed->text().toInt(); }
    void setOutEnabled(bool b) { outEnable = b; }
    QTcpSocket tcpSocket;
    Device *dev;
private slots:
    void switchOff(int);
private:
    uint iterCount, iterTime;
    bool outEnable;
};

#endif // BULBTEST_H
