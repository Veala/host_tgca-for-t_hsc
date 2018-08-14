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
private:
    QLineEdit *lineEditIterate, *lineEditInterval, *lineEditDevice;
};

class bulbObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
public:
    void setIter(QLineEdit* ed) { iter = ed->text().toInt(); }
    void setPause(QLineEdit* ed) { pause = ed->text().toInt(); }
    int socketDescriptor;
    QTcpSocket tcpSocket;
    Device *dev;
private:
    uint iter, pause;
};

#endif // BULBTEST_H
