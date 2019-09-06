
#include "testutil.h"

int readRegVal(Device* dev, BaseReg *reg)
{
    dev->readReg(reg);
    return reg->getData();
}

int getStatusReg(Device * dev)
{
    REG_HSC_status reg_status;
    return readRegVal(dev, &reg_status);
}

bool splitOddEven(const QByteArray all, QByteArray& even, QByteArray& odd)
{
    int sz = all.size();
    if (sz%8 != 0)
        return false;
    for (int i=0; i<sz; i+=8)
    {
        int addr = *(int*)(all.data()+i);
        int val = *(int*)(all.data()+i+4);
        if (addr != REG_VSK_creg)
        {
            even.append((char*)&addr, 4);
            odd.append((char*)&val, 4);
        }
    }
    return (even.size() > 0);
}

QString formattedLongInteger(long num)
{
    QString word = QString::number(num, 10);
    int w_size = word.size();
    if (w_size > 3)
    {
        word.insert(word.size() - 3, " ");
        if (w_size > 6)
        {
            word.insert(word.size() - 7, " ");
            if (w_size > 9)
                word.insert(word.size() - 11, " ");
        }
    }
    return word;
}
