#ifndef TRASHTEST_H
#define TRASHTEST_H

#include "abstracttest.h"

class TrashTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit TrashTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QComboBox *mode;
    QLineEdit *startAddr, *count, *endAddr, *startData, *deviceEdit;
    QSpinBox *incData;
};

class trashObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    QByteArray writeArray;
    QByteArray readArray;
    QString mode;
    uint startAddr, count, endAddr, startData, incData;
    Device *dev;
};

#endif // TRASHTEST_H
