
#include "ctestbc.h"
#include "extern_hsc.h"
#include <string.h>
#include "command.h"

#include <QString>

CTestBC::CTestBC():
    loaded(false)
{
}

bool CTestBC::setConfiguration(reg16_t cfg)
{
    mt = manType(cfg);
    codec = (cfg & fl_REG_CFG_ena_codec) != 0;
    nw = calcNumWordInSymbol(mt, codec);
    loaded = true;

    return true;
}

int CTestBC::NumSymOFDM(int num_byte)
{
    if (num_byte < 0)
        return -1;

    unsigned short len = numWordInSymbol();
    if (len == 0)
        return -1;

    return (num_byte + sizeof(word32_t) -1) / len;  // учтено командное слово
}

unsigned int CTestBC::maxNumByte()
{
    unsigned short len = numWordInSymbol();
    if (len == 0)
        return 0;

    return MAXNUMSYMBINPACK*len*sizeof(word32_t) - sizeof(word32_t);
}


/// Создает командное слово и копирует его в mem_dst вместе с соответствующим количеством слов данных из mem_src.
/// Размеры буфера назначения и входных данных задаются в байтах.
bool CTestBC::createCommandPack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src, int addr, int tr, unsigned int code)
{
    word32_t command;
    char* ptr_dst = (char*)mem_dst;
    char* ptr_src = (char*)mem_src;
    int num_s = 0;   // это значение будет при tr != tgca_tr_REC

    if (!loaded)
        return false;   // регистр конфигурации не установлен

    if (tr == tgca_tr_REC)  // передача КШ -> ОУ
    {
        if (mem_src == NULL && size_src > 0)
            return false;   // отсутствуют данные

        if (size_src > maxNumByte())
            return false;   // размер данных не поместится в один пакет

        if (NumSymOFDM(size_src) < 0)
            return false;   // ошибка метода NumSymOFDM()
    }

    if ((num_s + 1) * NUMWORDINOFDMSYM * sizeof(word32_t) > (int)size_dst)
        return false;   // длина выходного массива недостаточна

    /// Преамбулы будут созданы автоматически. Записываем в память только OFDM символы.

    createCommandWord(&command, addr, num_s, tr, code);
    memcpy((void*)ptr_dst, (void*)(&command), sizeof(word32_t));
    ptr_dst += sizeof(word32_t);

    unsigned nc = nw - sizeof(word32_t);
    if (tr != tgca_tr_REC)
        size_src = 0;

    /// Копирование первого символа
    if (nc > size_src)
        nc = size_src;
    size_src -= nc;
    int rest = (NUMWORDINOFDMSYM-1)*sizeof(word32_t) - nc;
    if (nc > 0)
    {
        memcpy((void*)ptr_dst, (void*)ptr_src, nc);
        ptr_dst += nc;
        ptr_src += nc;
    }
    memset((void*)ptr_dst, 0, rest);
    ptr_dst += rest;
    ptr_src += rest;

    /// Копирование остальных символов
    while(size_src > 0)
    {
        nc = nw > size_src ? size_src : nw;
        size_src -= nc;
        rest = NUMWORDINOFDMSYM * sizeof(word32_t) - nc;
        memcpy((void*)ptr_dst, (void*)ptr_src, nc);
        ptr_dst += nc;
        ptr_src += nc;
        memset((void*)ptr_dst, 0, rest);
        ptr_dst += rest;
        ptr_src += rest;
    };
    return true;
}


//hscl_SingleExchange
//Test_BC_TRM
//CatchStatusChange
