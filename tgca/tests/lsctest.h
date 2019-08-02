#ifndef LSCTEST_H
#define LSCTEST_H

#include "abstracttest.h"

class LscTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit LscTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QLineEdit *deviceEditBC, *deviceEditRT;

};

class lscObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    QList<Device*> devices;
    QList<QString> isStopped;
};

#endif // LSCTEST_H
