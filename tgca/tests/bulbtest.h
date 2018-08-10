#ifndef BULBTEST_H
#define BULBTEST_H

#include "abstracttest.h"

class BulbTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit BulbTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *pB, QTextBrowser *tB);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
};

#endif // BULBTEST_H
