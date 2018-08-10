#ifndef GENDATA_H
#define GENDATA_H

void *createRandomData(int length, int init, int minval, int maxval, int numchar = 1);
void *createRegulatData(int length, int init, int step, int iter, int numchar = 1);
void destroyData(void* data);

#endif // GENDATA_H