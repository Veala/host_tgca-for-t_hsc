#ifndef TESTUTIL_H
#define TESTUTIL_H

#include "extern_hsc.h"
#include "gendata.h"
#include "device.h"

int waitForInterruption(Device* dev, bool useInt, int waitTime, int *status);
int readRegVal(Device* dev, BaseReg *reg);
int getStatusReg(Device * dev);

#endif // TESTUTIL_H
