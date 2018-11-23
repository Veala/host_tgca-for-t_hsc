
#include "ctestbc.h"
#include "extern_hsc.h"
#include <string.h>
#include "command.h"

//#include <QString>
#include <QDebug>

CTestBC::CTestBC():
    loaded(false)
{
}

bool CTestBC::setConfigFlds(int man, bool enacodec)
{
    mt = man;
    codec = enacodec;
    nw = calcNumWordInSymbol(mt, codec);
    loaded = true;

    return true;
}

int CTestBC::NumSymOFDM(int num_byte)
{
    if (num_byte < 0)
        return -1;

    unsigned short len = numWordInSymbol()*sizeof(word32_t);
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
/// Размеры буфера назначения и входных данных задаются в байтах. Входной буфер mem_dst заранее заполнен нулями.
bool CTestBC::createCommandPack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src, int addr, int tr, unsigned int code)
{
    //qDebug() << size_dst << " " << tr;
    word32_t command;
    char* ptr_dst = (char*)mem_dst;
    char* ptr_src = (char*)mem_src;

    if (!loaded)
        return false;   // регистр конфигурации не установлен

    int num_s =  NumSymOFDM(size_src);   // это значение будет при tr == tgca_tr_REC

    if (num_s < 0)
        return false;   // ошибка метода NumSymOFDM()

    if (tr == tgca_tr_REC)  // передача КШ -> ОУ
    {
        if (mem_src == NULL && size_src > 0)
            return false;   // отсутствуют данные

        if (size_src > maxNumByte())
            return false;   // размер данных не поместится в один пакет

        if ((num_s + 1) * NUMWORDINOFDMSYM * sizeof(word32_t) > (int)size_dst)
            return false;   // длина выходного массива недостаточна
    }


    /// Преамбулы будут созданы автоматически. Записываем в память только OFDM символы.

    createCommandWord(&command, addr, num_s, tr, code);
    if (tr == tgca_tr_TRM)  // передача ОУ -> КШ
    {
        num_s = 0;
        if ((num_s + 1) * NUMWORDINOFDMSYM * sizeof(word32_t) > (int)size_dst)
            return false;   // длина выходного массива недостаточна
    }
    memcpy((void*)ptr_dst, (void*)(&command), sizeof(word32_t));

    unsigned nb = nw * sizeof(word32_t);
    unsigned nc = nb - sizeof(word32_t);
    if (tr != tgca_tr_REC)
        return true;

    /// Копирование первого символа
    if (nc > size_src)
        nc = size_src;
    size_src -= nc;
    //qDebug() << "nc " << nc << " nw " << nw << " nb " << nb << " size_src " << size_src << " size_dst " << size_dst;
    if (nc > 0)
    {
        memcpy((void*)(ptr_dst + sizeof(word32_t)), (void*)ptr_src, nc);
        ptr_src += nc;
    }

    /// Копирование остальных символов
    while(size_src > 0)
    {
        ptr_dst += (NUMWORDINOFDMSYM * sizeof(word32_t));
        nc = nb > size_src ? size_src : nb;
        size_src -= nc;
        memcpy((void*)ptr_dst, (void*)ptr_src, nc);
        ptr_src += nc;
    };
    return true;
}

/// Создает образ данных для записи в буфер передатчика ОУ для команды приёма от ОУ.
/// Входной буфер mem_dst заранее заполнен нулями.
/// Нулевое слово нулевого символа (отсутствующее командное слово) остаётся заполненным нулями.
/// Размеры буфера назначения и входных данных задаются в байтах.
bool CTestBC::createTestPack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src)
{
    char* ptr_dst = (char*)mem_dst;
    char* ptr_src = (char*)mem_src;

    if (!loaded)
        return false;   // регистр конфигурации не установлен

    if (mem_src == NULL)
        return false;   // отсутствуют данные

    if (size_src == 0)
        return false;   // запрещаем передачу 0 байт

    if (size_src > maxNumByte())
        return false;   // размер данных не поместится в один пакет

    int num_s = NumSymOFDM(size_src);
    if (num_s < 0)
            return false;   // ошибка метода NumSymOFDM()

    if ((num_s + 1) * NUMWORDINOFDMSYM * sizeof(word32_t) > (int)size_dst)
        return false;   // длина выходного массива недостаточна

    unsigned nb = nw * sizeof(word32_t);
    unsigned nc = nb - sizeof(word32_t);

    /// Копирование первого символа
    if (nc > size_src)
        nc = size_src;
    size_src -= nc;
    //qDebug() << "nc " << nc << " nw " << nw << " nb " << nb << " size_src " << size_src << " size_dst " << size_dst;
    if (nc > 0)
    {
        memcpy((void*)(ptr_dst + sizeof(word32_t)), (void*)ptr_src, nc);
        ptr_src += nc;
    }

    /// Копирование остальных символов
    while(size_src > 0)
    {
        ptr_dst += (NUMWORDINOFDMSYM * sizeof(word32_t));
        nc = nb > size_src ? size_src : nb;
        size_src -= nc;
        memcpy((void*)ptr_dst, (void*)ptr_src, nc);
        ptr_src += nc;
    };
    return true;
}
