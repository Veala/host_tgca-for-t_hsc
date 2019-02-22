#ifndef COMMAND_H
#define COMMAND_H

#include "extern_hsc.h"

bool createCommandWord(word32_t* command_word, addr_t addr, unsigned int num_s, int tr, unsigned int code);
bool parseCommandWord(void* pComm, addr_t *rta, short* num_s, short* tr, short* code);
int getRtaFromCommand(void* pComm);
int getTrbitFromCommand(void* pComm);
int getPackLenFromCommand(void* pComm);

#endif // COMMAND_H
