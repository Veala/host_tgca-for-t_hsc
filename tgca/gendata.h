#ifndef GENDATA_H
#define GENDATA_H

void *createRandomData(long length, int init, int minval, int maxval, int numchar = 1, int numcopy = 1);
void *createRegularData(long length, int init, int step, int itermax, int numchar = 1, int numcopy = 1);
void destroyData(void* data);

#endif // GENDATA_H
