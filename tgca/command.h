#ifndef COMMAND_H
#define COMMAND_H

#include "extern_hsc.h"

bool createCommandWord(word32_t* command_word, addr_t addr, unsigned int num_s, int tr, unsigned int code);

#endif // COMMAND_H
