
#include "command.h"

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

