#ifndef ECHOTEST_H
#define ECHOTEST_H

#include "abstracttest.h"

class EchoTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit EchoTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QLineEdit *echo, *deviceEdit;
};

class echoObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    QString echoText;
    QTcpSocket tcpSocket;
    Device *dev;
};

#endif // ECHOTEST_H
