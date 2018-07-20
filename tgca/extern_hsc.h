#ifndef EXTERN_H
#define EXTERN_H

/// Типы.   !!! LLL Эти типы определены в hscl.h
typedef unsigned short  addr_t;
typedef unsigned short  word16_t;
typedef unsigned int    word32_t;
typedef unsigned int*   p_mem_t;

#define SIZEOFWORD 4

word16_t memToWord16(char *buf, int addr);
void word16ToMem(char *buf, int num_reg, word16_t val);

void ReadFormat1(char *mem_dst, addr_t mem_src, int num_byte);
void WriteFormat1(char *mem_src, addr_t *mem_dst, int num_byte);

#endif // EXTERN_H
