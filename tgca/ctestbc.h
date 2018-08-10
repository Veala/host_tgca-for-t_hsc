#ifndef CTESTBC_H
#define CTESTBC_H

#include "registers.h"

class CTestBC
{
    /// настройки конфигурации
    bool loaded;
    int mt;
    bool codec;
    unsigned short nw;

public:
    CTestBC();

    bool setConfiguration(reg16_t cfg);
    unsigned short numWordInSymbol() const { return loaded ? nw : 0; }
    bool NumSymOFDM(int num_byte, int* num_sym, int* sym_len);
    unsigned int maxNumByte();
    bool createCommandPack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src, int addr, int tr, unsigned int code);

};

#endif // CTESTBC_H
