#ifndef LSCFORMAT_H
#define LSCFORMAT_H

#include "extern_hsc.h"
#include "device.h"
#include <QByteArray>

/// Шина
#define lsc_BUS_A 0
#define lsc_BUS_B 1
#define lsc_CW_BUS_FLAG 0x80

class C_LSC_Format
{
public:
    explicit C_LSC_Format() {}
    virtual ~C_LSC_Format() {}

    void addMessage(int &addrStackRT, int &addrBC);
    bool check();
    void spoil(bool fast) { spoilRW(); if (!fast) spoilData(); }
    void setDevices(Device* dev1, Device* dev2) { devBC=dev1; devRT=dev2; }
    void setBus(int bus) { bus_flag = (bus == lsc_BUS_A ? lsc_CW_BUS_FLAG : 0); }
    void setSABufs(word32_t* rec, word32_t* trm, word32_t* brd_rec) { sa_buf_rec = rec, sa_buf_trm=trm, sa_buf_brd = brd_rec; }

protected:
    word32_t bus_flag;
    Device* devBC;
    Device* devRT;
    //word32_t recCommand, pData, statusRT;
    word32_t curCommandStackRT;

    word32_t* sa_buf_rec;
    word32_t* sa_buf_trm;
    word32_t* sa_buf_brd;

    word32_t readRW(word32_t addr);
    void writeRam1Word(word32_t addr, word32_t val);
    word32_t getSABufRec(int sa) { return sa_buf_rec[sa]; }
    void readMemBlkRT(int addr, word32_t* buf, int num_words);

    virtual void addControlWord(QByteArray &arr) = 0;
    virtual void addCommandWord(QByteArray &arr) = 0;
    virtual void addDataBC(QByteArray &arr, int &addrBC) = 0;
    virtual void addDataRT(int &addrStackRT) = 0;
    virtual bool broad() = 0;
    virtual bool checkRW() = 0;
    virtual bool checkData() = 0;
    virtual int length() = 0;
    virtual void spoilRW() = 0;
    virtual void spoilData() = 0;

    // признаки слова управления сообщением
static const word32_t bc_a_b_flag = 0x80; //	выбор канала: 0x80 - А, 0 - Б
static const word32_t modecode_flag = 4;  // формат КУ
static const word32_t broad_flag    = 2;  // групповое сообщение
static const word32_t rt_rt_flag    = 1;  // сообщение «ОУ-ОУ»

static const word32_t initVal = 0xFF00;   // значение, которым инициализируется память: оно должно быть затёрто ответным словом или данными
};

class C_LSC_Format1: public C_LSC_Format  // КШ-ОУ
{
public:
    explicit C_LSC_Format1(int rtaddr, int saddr, int num, word32_t val, bool my);
    bool broad() { return false; }
    int length() {return nw + 4; }
    bool checkRW() { return readRW(addrRW) == (own ? (rta<<11) : initVal); }
    virtual bool checkData();
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag; arr.append((char*)CW, 4); }
    void addCommandWord(QByteArray &arr) { command = (rta<<11)|(sa<<5)|nw; arr.append((char*)command, 4); }
    void addDataBC(QByteArray &arr, int &addrBC);
    void addDataRT(int &addrStackRT);
    void spoilRW();
    void spoilData();
private:
    bool own;
    int rta, sa, nw;
    word32_t addrRW, dataVal, command; //, expectedRW;
};

class C_LSC_Format2: public C_LSC_Format  // ОУ-КШ
{
public:
    bool broad() { return false; }
    int length() {return nw + 4; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag; arr.append((char*)CW, 4); }
    void addCommandWord(QByteArray &arr) { command = (rta<<11)|(1<<10)|(sa<<5)|nw; arr.append((char*)command, 4); }
    //void addDataBC(QByteArray &arr, int &addrBC);
    //void addDataRT(int &addrStackRT);
    //void spoilRW();
    //void spoilData();
private:
    bool own;
    int rta, sa, nw;
    word32_t addrRW, dataVal, command;
};

class C_LSC_Format3: public C_LSC_Format  // ОУ-ОУ
{
public:
    bool broad() { return false; }
    int length() {return nw + 6; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + rt_rt_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int type; // 0 - передача от rta другому, 1 - передача от другого к rta, 2 - оба адреса отличны от rta
    int rta, sa1, sa2, nw;
    word32_t addrRW1, addrRW2, dataVal;
};

class C_LSC_Format4: public C_LSC_Format  // КУ без СД
{
public:
    bool broad() { return false; }
    int length() {return 3; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + modecode_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, mc;
    word32_t addrRW;
};

class C_LSC_Format5: public C_LSC_Format  // КУ и приём СД от ОУ
{
public:
    bool broad() { return false; }
    int length() {return 4; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + modecode_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, mc;
    word32_t addrRW;
};

class C_LSC_Format6: public C_LSC_Format  // КУ со СД
{
public:
    bool broad() { return false; }
    int length() {return 4; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + modecode_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, mc, cd;
    word32_t addrRW;
};

class C_LSC_Format7: public C_LSC_Format  // групповая КШ-всем ОУ
{
public:
    bool broad() { return true; }
    int length() {return nw + 3; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int sa, nw;
    word32_t dataVal;
};

class C_LSC_Format8: public C_LSC_Format  // групповая передача от ОУ другим ОУ
{
public:
    bool broad() { return true; }
    int length() {return nw + 5; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag + rt_rt_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    bool own;
    int rta, sa1, sa2, nw;
    word32_t addrRW, dataVal;
};

class C_LSC_Format9: public C_LSC_Format  // групповая КУ
{
public:
    bool broad() { return true; }
    int length() {return 3; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag + modecode_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int mc;
};

class C_LSC_Format10: public C_LSC_Format  // групповая КУ со СД (синхронизация с СД)
{
public:
    bool broad() { return true; }
    int length() {return 4; }
    bool checkRW() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool checkData() {return false;} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
    void addControlWord(QByteArray &arr) { word32_t CW = bus_flag + broad_flag + modecode_flag; arr.append((char*)CW, 4); }
    void spoilRW()   {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void spoilData() {} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
private:
    int mc, cd;
};

#endif // LSCFORMAT_H
