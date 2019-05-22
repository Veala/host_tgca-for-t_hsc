
#include "command.h"

#include <QDebug>

bool createCommandWord(word32_t* command_word, addr_t addr, unsigned int num_s, int tr, unsigned int code)
{
    bool bRet = true;
    if (addr > MAX_RT_ADDR)
    {
        addr &= MAX_RT_ADDR;
        bRet = false;
    }
    if (num_s >= MAXNUMSYM)
    {
        num_s &= 0xFF;
        bRet = false;
    }
    if (code > MAX_COMMAND_CODE)
    {
        code &= MAX_COMMAND_CODE;
        bRet = false;
    }
    if ( tr != 0 && tr != 1 && tr != tgca_fl_CW_tr_bit)
        bRet = false;
    if (tr != 0)
        tr = 1;

    *command_word = (addr << 16) | (num_s << 8) | (tgca_fl_CW_tr_bit * tr) | code;
    return bRet;
}

/// Старт обмена
int startVSK()
{
    //return hscl_WriteReg(hscl_fl_REG_CREG_start_bc, hscl_NUMREG_creg);
    return 0;
}

bool parseCommandWord(void* pComm, addr_t *rta, short* num_s, short* tr, short* code)
{
    word32_t* ptr = (word32_t*)pComm;
    word32_t command = *ptr;
    *rta = (command >> 16) & MAX_RT_ADDR;
    *num_s = (command >> 8) & 0xFF;
    *tr = (command & tgca_fl_CW_tr_bit) ? 1 : 0;
    *code = command & MAX_COMMAND_CODE;
    return true;
}

word32_t getRtaFromCommand(void* pComm)
{
    word32_t* ptr = (word32_t*)pComm;
    word32_t command = *ptr;
    return (command >> 16) & MAX_RT_ADDR;
}

word32_t getTrbitFromCommand(void* pComm)
{
    word32_t* ptr = (word32_t*)pComm;
    word32_t command = *ptr;
    return (command & tgca_fl_CW_tr_bit) ? 1 : 0;;
}

word32_t getPackLenFromCommand(void* pComm)
{
    word32_t* ptr = (word32_t*)pComm;
    word32_t command = *ptr;
    return (command >> 8) & 0xFF;
}

word32_t getRtaFromResponse(void* pData)
{
    word32_t* ptr = (word32_t*)pData;
    word32_t rw = *ptr;
    return (rw >> 16) & MAX_RT_ADDR;
}

word32_t getStatusFromResponse(void* pData)
{
    word32_t* ptr = (word32_t*)pData;
    word32_t rw = *ptr;
    return (rw & 0xFFFF);
}
