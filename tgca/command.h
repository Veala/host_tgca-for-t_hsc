#ifndef COMMAND_H
#define COMMAND_H

#include "extern_hsc.h"

bool  createCommandWord(word32_t* command_word, addr_t addr, unsigned int num_s, int tr, unsigned int code);
bool  parseCommandWord(void* pComm, addr_t *rta, short* num_s, short* tr, short* code);
word32_t  getRtaFromCommand(void* pComm);
word32_t getTrbitFromCommand(void* pComm);
word32_t getPackLenFromCommand(void* pComm);

word32_t getRtaFromResponse(void* pData);
word32_t getStatusFromResponse(void* pData);

#endif // COMMAND_H
