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

    bool setConfigFlds(int man, bool enacodec);
    unsigned short numWordInSymbol() const { return loaded ? nw : 0; }
    int NumSymOFDM(int num_byte);
    unsigned int maxNumByte();
    bool createCommandPack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src, int addr, int tr, unsigned int code);
    bool array2Pack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src);
    bool pack2Array(char* mem_dst, unsigned int size_dst, char* buf_rec, unsigned int num_sym, bool without_cw = true);
    bool cmpPack(void* vsk_buf1, void* vsk_buf2, int num_sym, bool without_cw);
    bool cmpPackData(void* vsk_buf, void* raw_data, int num_b, bool without_cw);
    bool cmpPackDataBit(void* vsk_buf, void* raw_data, int num_b, int *wrong_bit, int max_wrong_bit);

private:
    void array2PackQPSK(char* ptr_dst, char* ptr_src, unsigned int size_src);
    void array2PackQAM16(char* ptr_dst, char* ptr_src, unsigned int size_src);
    void array2PackQAM64(char* ptr_dst, char* ptr_src, unsigned int size_src);
    void pack2ArrayQPSK(char* ptr_dst, char* ptr_src, unsigned int num_sym, bool without_cw);
    void pack2ArrayQAM16(char* ptr_dst, char* ptr_src, unsigned int num_sym, bool without_cw);
    void pack2ArrayQAM64(char* ptr_dst, char* ptr_src, unsigned int num_sym, bool without_cw);
};

#endif // CTESTBC_H
