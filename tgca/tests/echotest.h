#ifndef ECHOTEST_H
#define ECHOTEST_H

#include "abstracttest.h"

class EchoTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit EchoTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
    void setEnabledSpecial(bool) {}
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
    Device *dev;
};

#endif // ECHOTEST_H
