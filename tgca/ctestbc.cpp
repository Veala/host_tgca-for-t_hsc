
#include "ctestbc.h"
#include "extern_hsc.h"
#include <string.h>
#include "testutil.h"
#include "command.h"

#include <QDebug>

CTestBC::CTestBC():
    loaded(false)
{
}

static inline bool isValidMT(int mt)
{
    return (mt == val_REG_CFG_type_man_QPSK || mt == val_REG_CFG_type_man_QAM16 || mt == val_REG_CFG_type_man_QAM64);
}

bool CTestBC::setConfigFlds(int man, bool enacodec)
{
    loaded = isValidMT(man);
    codec = enacodec;
    if (loaded)
    {
        mt = man;
        nw = calcNumWordInSymbol(mt, codec);
    }
    else
        mt = val_REG_CFG_type_man_ERROR;

    return loaded;
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

    return MAXNUMSYM*len*sizeof(word32_t) - sizeof(word32_t);
}

/// Создает командное слово и копирует его в mem_dst вместе с соответствующим количеством слов данных из mem_src.
/// Размеры буфера назначения и входных данных задаются в байтах.
/// Свободная от данных часть буфера mem_dst заполняется нулями.
bool CTestBC::createCommandPack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src, int addr, int tr, unsigned int code)
{
    word32_t command;

    if (!loaded)
        return false;   // регистр конфигурации не установлен

    int num_s =  NumSymOFDM(size_src);   // это значение будет при tr == tgca_tr_REC

    if (num_s < 0)
        return false;   // ошибка метода NumSymOFDM()

    if (tr == tgca_tr_REC)  // передача КШ-ОУ
    {
        if (mem_src == NULL && size_src > 0)
            return false;   // отсутствуют данные

        if (size_src > maxNumByte())
            return false;   // размер данных не поместится в один пакет
    }

    int size_required = (tr == tgca_tr_REC) ? num_s+1 : 1;
    size_required *= NUMBYTEINOFDMSYM;
    if (size_required > (int)size_dst)
        return false;   // длина выходного массива недостаточна

    char * ptr_dst = (char*)mem_dst;
    ptr_dst += sizeof(word32_t);
    memset((void*)ptr_dst, 0, size_required - sizeof(word32_t));

    /// Преамбулы будут созданы автоматически. В память будут записаны только OFDM символы.
    /// Создаём и записываем командное слово
    createCommandWord(&command, addr, num_s, tr, code);
    memcpy(mem_dst, (void*)(&command), sizeof(word32_t));

    if (tr != tgca_tr_REC)
        return true;

    if (mt == val_REG_CFG_type_man_QPSK)
    {
        array2PackQPSK((char*)mem_dst, (char*)mem_src, size_src);
    }
    else if (mt == val_REG_CFG_type_man_QAM16)
        array2PackQAM16((char*)mem_dst, (char*)mem_src, size_src);
    else
        array2PackQAM64((char*)mem_dst, (char*)mem_src, size_src);

    return true;
}

/// Создает образ данных для записи в буфер передатчика ОУ для команды передачи ОУ-КШ.
/// Незначащие байты входного буфера mem_dst заполняются нулями.
/// Нулевое слово нулевого символа (ответное слово) не изменяется.
/// Размеры буфера назначения и входных данных задаются в байтах.
bool CTestBC::array2Pack(void* mem_dst, unsigned int size_dst, void* mem_src, unsigned int size_src)
{
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

    if ((num_s + 1) * NUMBYTEINOFDMSYM > (int)size_dst)
        return false;   // длина выходного массива недостаточна

    char * ptr_dst = (char*)mem_dst;
    ptr_dst += sizeof(word32_t);
    memset((void*)ptr_dst, 0, size_dst - sizeof(word32_t));

    if (mt == val_REG_CFG_type_man_QPSK)
        array2PackQPSK((char*)mem_dst, (char*)mem_src, size_src);
    else if (mt == val_REG_CFG_type_man_QAM16)
        array2PackQAM16((char*)mem_dst, (char*)mem_src, size_src);
    else
        array2PackQAM64((char*)mem_dst, (char*)mem_src, size_src);

    return true;
}

/// Собирает данные из буфера приёмника в массив mem_dst.
/// Ответное слово не считается данными и не копируется в выходной массив.
/// Размер буфера назначения задаётся в байтах.
/// Размер данных в буфере приёмника задаётся в символах OFDM (0-255).
bool CTestBC::pack2Array(char* mem_dst, unsigned int size_dst, char* buf_rec, unsigned int num_sym, bool without_cw)
{
    if (!loaded)
        return false;   // регистр конфигурации не установлен

    if (mem_dst == NULL || buf_rec == NULL)
        return false;

    // (nw * (num_sym+1) - 1) * sizeof(word32_t) - это количество полезных байт данных в пакете
    if ((nw * (num_sym+1) - 1) * sizeof(word32_t) > size_dst)
        return false;   // длина выходного буфера недостаточно

    if (mt == val_REG_CFG_type_man_QPSK)
        pack2ArrayQPSK(mem_dst, buf_rec, num_sym, without_cw);
    else if (mt == val_REG_CFG_type_man_QAM16)
        pack2ArrayQAM16(mem_dst, buf_rec, num_sym, without_cw);
    else
        pack2ArrayQAM64(mem_dst, buf_rec, num_sym, without_cw);

    return true;
}

/// Сравнение двух буферов данных в формате ВСК, игнорируя служебные и неиспользуемые байты
/// n_sym - число ОФДМ символов для сравнения
bool CTestBC::cmpPack(void* vsk_buf1, void* vsk_buf2, int n_sym, bool without_cw)
{
    char cmp_data1[MAXPACKAGESIZE];
    char cmp_data2[MAXPACKAGESIZE];
    if (!loaded)
        return false;

    if (n_sym < 0 || n_sym >= MAXNUMSYM)
        return false;

    if (!pack2Array(cmp_data1, MAXPACKAGESIZE, (char*)vsk_buf1, n_sym, without_cw))
        return false;

    if (!pack2Array(cmp_data2, MAXPACKAGESIZE, (char*)vsk_buf2, n_sym, without_cw))
        return false;

    int nb = nw*sizeof(word32_t)*(n_sym+1);  // число "полезных" байт
    if (without_cw)
        nb -= sizeof(word32_t);

    int i = memcmp((void*)cmp_data1, (void*)cmp_data2, nb);

    if (i)
    {
        char *ptr1 = cmp_data1;
        char *ptr2 = cmp_data2;
        for (int i=0; i<nb; i++)
        {
            if ( (*(ptr1+i)) != (*(ptr2+i)) )
            {
                qDebug() << "Difference: i = " << i << "  " << (uint)(*(ptr1+i)) << "  " << (uint)(*(ptr2+i));
                return false;
            }
        }

        qDebug() << "Difference not found";

        return false;
    }

    return true;
}

/// Сравнение данных в буфере в формате ВСК с исходными неформатированными данными.
/// num_b - размер данных для сравнения в байтах
/// если !without_cw, то командное слово участвует в сравнении наравне с данными и размер num_b включает и командное слово тоже.
bool CTestBC::cmpPackData(void* vsk_buf, void* raw_data, int num_b, bool without_cw)
{
    char cmp_data[MAXPACKAGESIZE];
    if (!loaded)
        return false;

    int n_sym;
    if (without_cw)
        n_sym = (num_b - 1 + sizeof(word32_t)) / NUMBYTEINOFDMSYM;
    else
        n_sym = (num_b - 1) / NUMBYTEINOFDMSYM;
    if (n_sym < 0 || n_sym >= MAXNUMSYM)
        return false;

    if (!pack2Array(cmp_data, MAXPACKAGESIZE, (char*)vsk_buf, n_sym, without_cw))
        return false;

    int i = memcmp((void*)cmp_data, raw_data, num_b);

    if (i)
    {
        char * ptr1 = cmp_data;
        char* ptr2 = (char*) raw_data;
        for (int i=0; i<num_b; i++)
        {
            if ( (*(ptr1+i)) != (*(ptr2+i)) )
            {
                qDebug() << "Difference: i = " << i << "  " << (uint)(*(ptr1+i)) << "  " << (uint)(*(ptr2+i));
                return false;
            }
        }

        qDebug() << "Difference not found";

        return false;
    }

    return true;
}


// private

void CTestBC::pack2ArrayQPSK(char* ptr_dst, char* ptr_src, unsigned int num_sym, bool without_cw)
{
    unsigned ncw = without_cw ? sizeof(word32_t) : 0;  // сдвиг на длину командного слова
    unsigned nb = nw * sizeof(word32_t);
    unsigned nc = nb - ncw;

    /// Копирование первого символа
    memcpy((void*)ptr_dst, (void*)(ptr_src + ncw), nc);
    ptr_dst += nc;

    /// Копирование остальных символов
    for (unsigned num=0; num<num_sym; num++)
    {
        ptr_src += NUMBYTEINOFDMSYM;
        memcpy((void*)ptr_dst, (void*)ptr_src, nb);
        ptr_dst += nb;
    };
}

void CTestBC::pack2ArrayQAM16(char* ptr_dst, char* ptr_src, unsigned int num_sym, bool without_cw)
{
    // В каждом символе два отрезка полезных данных
    unsigned nb1 = codec ? 96 : 112;  // число байт в первом отрезке
    unsigned nb2 = codec ? 80 : 112;  // число байт во втором отрезке
    unsigned shift1 = 128;            // число байт между началом OFDM символа и началом второго отрезка
    unsigned shift2 = NUMBYTEINOFDMSYM - shift1;
    unsigned ncw = without_cw ? sizeof(word32_t) : 0;  // сдвиг на длину командного слова

    /// Копирование первого символа
    memcpy((void*)ptr_dst, (void*)(ptr_src + ncw), nb1-ncw);
    ptr_dst += (nb1-ncw);
    ptr_src += shift1;
    memcpy((void*)ptr_dst, (void*)ptr_src, nb2);

    /// Копирование остальных символов
    for (unsigned num=0; num < num_sym; num++)
    {
        ptr_dst += nb2;
        ptr_src += shift2;
        memcpy((void*)ptr_dst, (void*)ptr_src, nb1);

        ptr_dst += nb1;
        ptr_src += shift1;
        memcpy((void*)ptr_dst, (void*)ptr_src, nb2);
    };
}

void CTestBC::pack2ArrayQAM64(char* ptr_dst, char* ptr_src, unsigned int num_sym, bool without_cw)
{
    // В каждом OFDM символе 3 отрезка полезных данных
    unsigned nb1 = codec ? 96 : 112;  // число байт в первом отрезке
    unsigned nb2 = codec ? 80 : 112;  // число байт во втором отрезке
    unsigned nb3 = codec ? 48 : 112;  // число байт во втором отрезке
    unsigned shift1 = 128;  // число байт между началом OFDM символа и началом второго отрезка
    unsigned shift2 = 128;  // число байт между началом первого и началом второго отрезка
    unsigned shift3 = NUMBYTEINOFDMSYM - shift1 - shift2;
    unsigned ncw = without_cw ? sizeof(word32_t) : 0;  // сдвиг на длину командного слова

    for (unsigned num=0; num <= num_sym; num++)
    {
        memcpy((void*)ptr_dst, (void*)(ptr_src+ncw), nb1-ncw);
        ptr_dst += (nb1-ncw);
        ptr_src += shift1;

        memcpy((void*)ptr_dst, (void*)ptr_src, nb2);
        ptr_dst += nb2;
        ptr_src += shift2;

        memcpy((void*)ptr_dst, (void*)ptr_src, nb3);
        ptr_dst += nb3;
        ptr_src += shift3;
        ncw = 0;
    };
}

/// Копирование непрерывного массива данных в буфер устройства ВСК в соответствии с форматом QPSK.
/// Свободная от данных часть ОФДМ символов не заполняется.
/// Параметры:
/// ptr_dst - указатель на начало буфера,
/// ptr_src - указатель на данные,
/// size_src - размер данных в байтах.
void CTestBC::array2PackQPSK(char* ptr_dst, char* ptr_src, unsigned int size_src)
{
    unsigned nb = nw * sizeof(word32_t);  // число байт в одном символе
    unsigned nc = nb - sizeof(word32_t);  // число байт в первом символе
    /// Копирование первого символа
    if (nc > size_src)
        nc = size_src;
    size_src -= nc;
    //qDebug() << "nc " << nc << " nw " << nw << " nb " << nb << " size_src " << size_src;
    if (nc > 0)
        memcpy((void*)(ptr_dst + sizeof(word32_t)), (void*)ptr_src, nc);

    /// Копирование остальных символов
    while(size_src > 0)
    {
        ptr_dst += (NUMBYTEINOFDMSYM);
        ptr_src += nc;
        nc = nb > size_src ? size_src : nb;
        size_src -= nc;
        memcpy((void*)ptr_dst, (void*)ptr_src, nc);
    };
}

void CTestBC::array2PackQAM16(char* ptr_dst, char* ptr_src, unsigned int size_src)
{
    // В каждом символе два отрезка полезных данных
    unsigned nb1 = codec ? 96 : 112;  // число байт в первом отрезке
    unsigned nb2 = codec ? 80 : 112;  // число байт во втором отрезке
    unsigned shift1 = 128;            // число байт между началом OFDM символа и началом второго отрезка
    unsigned shift2 = NUMBYTEINOFDMSYM - shift1;
    unsigned ncw = sizeof(word32_t);  // длина командного слова

    unsigned nc = nb1 - ncw;    // число байт в первом отрезке первого символа
    if (nc > size_src)
        nc = size_src;
    size_src -= nc;
    if (nc > 0)
    {
        /// Копирование первого отрезка
        memcpy((void*)(ptr_dst + ncw), (void*)ptr_src, nc);

        /// Копирование остальных данных
        while(size_src > 0)
        {
            ptr_src += nc;
            ptr_dst += shift1;
            nc = nb2 > size_src ? size_src : nb2;
            size_src -= nc;
            memcpy((void*)ptr_dst, (void*)ptr_src, nc);
            if (size_src == 0)
                break;

            ptr_src += nc;
            ptr_dst += shift2;
            nc = nb1 > size_src ? size_src : nb1;
            size_src -= nc;
            memcpy((void*)ptr_dst, (void*)ptr_src, nc);
        };
    }
}

void CTestBC::array2PackQAM64(char* ptr_dst, char* ptr_src, unsigned int size_src)
{
    // В каждом OFDM символе 3 отрезка полезных данных
    unsigned nb1 = codec ? 96 : 112;  // число байт в первом отрезке
    unsigned nb2 = codec ? 80 : 112;  // число байт во втором отрезке
    unsigned nb3 = codec ? 48 : 112;  // число байт во втором отрезке
    unsigned shift1 = 128;  // число байт между началом OFDM символа и началом второго отрезка
    unsigned shift2 = 128;  // число байт между началом первого и началом второго отрезка
    unsigned shift3 = NUMBYTEINOFDMSYM - shift1 - shift2;
    unsigned ncw = sizeof(word32_t);  // длина командного слова

    unsigned nc = nb1 - ncw;    // число байт в первом отрезке первого символа
    if (nc > size_src)
        nc = size_src;
    size_src -= nc;
    if (nc > 0)
    {
        /// Копирование первого отрезка
        memcpy((void*)(ptr_dst + ncw), (void*)ptr_src, nc);

        /// Копирование остальных данных
        while(size_src > 0)
        {
            ptr_src += nc;
            ptr_dst += shift1;
            nc = nb2 > size_src ? size_src : nb2;
            size_src -= nc;
            memcpy((void*)ptr_dst, (void*)ptr_src, nc);
            if (size_src == 0)
                break;

            ptr_src += nc;
            ptr_dst += shift2;
            nc = nb3 > size_src ? size_src : nb3;
            size_src -= nc;
            memcpy((void*)ptr_dst, (void*)ptr_src, nc);
            if (size_src == 0)
                break;

            ptr_src += nc;
            ptr_dst += shift3;
            nc = nb1 > size_src ? size_src : nb1;
            size_src -= nc;
            memcpy((void*)ptr_dst, (void*)ptr_src, nc);
        };
    }
}

/// Подсчет количества несовпадающих битов в байтах
static bool cmpBytes(char char1, char char2, int *num_wrong_bit, int max_wrong_bit)
{
    if (char1 != char2)
    {
        if ((*num_wrong_bit) >= max_wrong_bit)
            ++(*num_wrong_bit);
        else
        {
            char diff = char1 ^ char2;
            do
            {
                ++(*num_wrong_bit);
                diff &= (diff-1);  // Забираем младшую единичку.
            }
            while (diff);
        }
    }
    return ((*num_wrong_bit) <= max_wrong_bit);
}

/// Рекурсивная функция сравнения двух сассивов длины num_b.
/// Число несовпавших бит добавляется к значению переменной num_wrong_bit.
/// Если число несовпавших бит не превышает max_wrong_bit, функция возвращает true.
/// Иначе функция возвращает false и значение *num_wrong_bit при этом может быть меньше
/// действительного количества различий (но больше max_wrong_bit).
bool cmpArrays(void* data_1, void* data_2, int num_b, int *num_wrong_bit, int max_wrong_bit)
{
    if (num_b == 1)
        return cmpBytes(0xFF & *((char*)data_1), 0xFF & *((char*)data_2), num_wrong_bit, max_wrong_bit);

    if (memcmp(data_1, data_2, num_b) == 0)
        return ((*num_wrong_bit) <= max_wrong_bit);

    if ((*num_wrong_bit) >= max_wrong_bit)
    {
        ++(*num_wrong_bit);
        return false;
    }

    int num_h = num_b/2;

    if (!cmpArrays(data_1, data_2, num_h, num_wrong_bit, max_wrong_bit))
        return false;

    char* ptr1 = ((char*)data_1) + num_h;
    char* ptr2 = ((char*)data_2) + num_h;

    return cmpArrays((void*)ptr1, (void*)ptr2, num_b-num_h, num_wrong_bit, max_wrong_bit);
}

/// Сравнение данных в буфере в формате ВСК с исходными неформатированными данными.
/// Командное слово не участвует в сравнении.
/// Параметры:
/// vsk_buf, raw_data - указатели на начало данных,
/// num_b - размер данных для сравнения в байтах,
/// wrong_bit - число несовпавших бит,
/// max_wrong_bit - максимальное разрешенное число несовпавших бит.
/// Функция прекращает дальнейшее сравнение при превышении max_wrong_bit.
/// Если число несовпавших бит меньше или равно разрешенному, то в переменную wrong_bit
/// записывается точное значение и функция возвращает true. Иначе - wrong_bit содержит
/// уже найденное количество и функция возвращает false.
bool CTestBC::cmpPackDataBit(void* vsk_buf, void* raw_data, int num_b, int *wrong_bit, int max_wrong_bit)
{
    char cmp_data[MAXPACKAGESIZE];

    *wrong_bit = 0;

    if (!loaded)
        return false;

    int n_sym = (num_b - 1 + sizeof(word32_t)) / NUMBYTEINOFDMSYM;
    if (n_sym < 0 || n_sym >= MAXNUMSYM)
        return false;

    if (!pack2Array(cmp_data, MAXPACKAGESIZE, (char*)vsk_buf, n_sym, true))
        return false;

    return cmpArrays(cmp_data, raw_data, num_b, wrong_bit, max_wrong_bit);
}
