#ifndef TESTUTIL_H
#define TESTUTIL_H

#include "extern_hsc.h"
#include "gendata.h"
#include "device.h"

int waitForInterruption(Device* dev, bool useInt, int waitTime, int *status);
int readRegVal(Device* dev, BaseReg *reg);
int getStatusReg(Device * dev);

bool splitOddEven(const QByteArray all, QByteArray& even, QByteArray& odd);

QString formattedLongInteger(long num);

#endif // TESTUTIL_H
