
#include "gendata.h"
#include <stdlib.h>    // malloc, rand

inline int randomValue(int minval, int maxval)
{
    return rand() % maxval + minval;
}

void fillRandomDataByte(void *data, int length, int minval, int maxval)
{
    for (int i=0; i<length; i++)
    {
        ((char*)data)[i] = randomValue(minval, maxval) & 0xFF;
    }
}
void fillRandomDataW16(void *data, int length, int minval, int maxval)
{
    for (int i=0; i<length; i++)
    {
        ((short*)data)[i] = randomValue(minval, maxval) & 0xFFFF;
    }
}
void fillRandomDataW32(void *data, int length, int minval, int maxval)
{
    for (int i=0; i<length; i++)
    {
        ((int*)data)[i] = randomValue(minval, maxval) & 0xFFFFFFFF;
    }
}
void fillRandomDataDefault(void *data, int length, int minval, int maxval)
{
    for (int i=0; i<length; i++)
    {
        ((char*)data)[i] = randomValue(minval, maxval);
    }
}

void *createRandomData(int length, int init, int minval, int maxval, int numchar)
{
    if (length <=0)
        return 0;
    if (numchar == 2)
        length += length % numchar;
    else if (numchar == 4 && length%numchar != 0)
        length += (4 - length%numchar);

    void *data = malloc(length);
    if (data)
    {
        srand(init);
        int scaleParam = maxval - minval + 1;
        if (scaleParam <= 0)
        {
            scaleParam = 255;
            minval = 0;
        }
        switch (numchar)
        {
        case 1:
            fillRandomDataByte(data, length, minval, scaleParam);
        case 2:
            fillRandomDataW16(data, length/2, minval, scaleParam);
        case 4:
            fillRandomDataW32(data, length/4, minval, scaleParam);
        default:
            fillRandomDataDefault(data, length, minval, scaleParam);
        }
    }
    return data;
}

void fillRegularDataByte(void *data, int length, int init, int step, int itermax)
{
    if (itermax > 0)
        for (int i=0; i<length; i++)
            ((char*)data)[i] = (init+step*(i%itermax)) & 0xFF;
    else
        for (int i=0; i<length; i++)
            ((char*)data)[i] = (init+step*i) & 0xFF;
}
void fillRegularDataDefault(void *data, int length, int init, int step, int itermax)
{
    if (itermax > 0)
        for (int i=0; i<length; i++)
            ((char*)data)[i] = init+step*(i%itermax);
    else
        for (int i=0; i<length; i++)
            ((char*)data)[i] = init+step*i;
}
void fillRegularDataW16(void *data, int length, int init, int step, int itermax)
{
    if (itermax > 0)
        for (int i=0; i<length; i++)
            ((short*)data)[i] = (init+step*(i%itermax)) & 0xFFFF;
    else
        for (int i=0; i<length; i++)
            ((short*)data)[i] = (init+step*i) & 0xFFFF;
}
void fillRegularDataW32(void *data, int length, int init, int step, int itermax)
{
    if (itermax > 0)
        for (int i=0; i<length; i++)
            ((int*)data)[i] = (init+step*(i%itermax)) & 0xFFFFFFFF;
    else
        for (int i=0; i<length; i++)
            ((int*)data)[i] = (init+step*i) & 0xFFFFFFFF;
}
void *createRegularData(int length, int init, int step, int itermax, int numchar)
{
    if (length <=0)
        return 0;
    if (numchar == 2)
        length += length % numchar;
    else if (numchar == 4 && length%numchar != 0)
        length += (4 - length%numchar);

    void *data = malloc(length);
    if (data)
    {
        switch (numchar)
        {
        case 1:
            fillRegularDataByte(data, length, init, step, itermax);
        case 2:
            fillRegularDataW16(data, length/2, init, step, itermax);
        case 4:
            fillRegularDataW32(data, length/4, init, step, itermax);
        default:
            fillRegularDataDefault(data, length, init, step, itermax);
        }
    }
    return data;
}

void destroyData(void* data)
{
    if (data)
        free (data);
}
