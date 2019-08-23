#ifndef LSCMESSAGETEST_H
#define LSCMESSAGETEST_H

#include "abstracttest.h"
#include "../lscformat.h"
#include "../writedevicememory.h"

class LscMessageTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit LscMessageTest(QWidget *parent): AbstractTest(parent) {}
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();
private:
    QLineEdit *deviceEditBC, *deviceEditRT;

};

class lscMessageObjToThread : public absObjToThread
{
    Q_OBJECT
public slots:
    virtual void doWork();
    void terminate(int);
public:
    explicit lscMessageObjToThread();
    ~lscMessageObjToThread();

    Device *devBC, *devRT;
private:
    AbstractTest::RunningState connectBC();
    AbstractTest::RunningState connectRT();
    bool isRunning();
//    void writeRegInitBC();
    void writeRegSingleBC(word32_t num, word32_t value);
//    void writeRegAddBC(word32_t num, word32_t value);
//    void writeMemAddBC(word32_t addr, word32_t val);
//    void writeRegistersBC();
    void writeMemBlkBC(int dst_addr, word32_t* buf, int num_words);
//    void writeRegInitRT();
//    void writeRegAddRT(word32_t num, word32_t value);
//    void writeMemAddRT(word32_t addr, word32_t val);
//    void writeRegistersRT();
    bool fillMemBlk(Device* dev, int dst_addr, int num_words16, word16_t value);
    bool fillMemBlkRT(int dst_addr, int num_words16, word16_t value) { return fillMemBlk(devRT, dst_addr, num_words16, value); }
    void readMemBlkRT(int addr, word32_t* buf, int num_words);
    void stdOutput(QString str) { emit outputReady(str); /*qDebug() << str;*/ }
    word32_t readResponseWord(word32_t addr);
    word32_t readStatusWordBC(word32_t addr);
    bool addMessage(C_LSC_Format* format, int bus);

    //QByteArray regRT, regBC;
    int num_messages;
    const int max_num_messages = 64;
    int fixRT, fixBC;
    word32_t arrayAddrRW[64];
    int curCommandStackBC;
    int curCommandStackRT;
    int ptrMessagesBC;
    const word32_t spoiler = 0xFFFF;

    word32_t sa_buf_rec[32];
    word32_t sa_buf_trm[32];
    word32_t sa_buf_brd[32];

    CWriteDeviceMemory bufBC;
    CWriteDeviceMemory bufRT;
};

#endif // LSCMESSAGETEST_H
