#ifndef BULBTEST_H
#define BULBTEST_H

#include "abstracttest.h"

class BulbTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit BulbTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QLineEdit *lineEditIterate, *lineEditInterval, *lineEditDevice;
};

class bulbObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    void setIter(QLineEdit* ed) { iterCount = ed->text().toInt(); }
    void setPause(QLineEdit* ed) { iterTime = ed->text().toInt(); }
    QTcpSocket tcpSocket;
    Device *dev;
private slots:
    void switchOff(int);
private:
    uint iterCount, iterTime;
};

#endif // BULBTEST_H
