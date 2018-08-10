#ifndef EXTERN_H
#define EXTERN_H

/// Типы.   !!! LLL Эти типы определены в hscl.h
typedef unsigned short  addr_t;
typedef unsigned short  word16_t;
typedef unsigned int    word32_t;
//typedef unsigned int    t_size_t;
typedef unsigned int*   p_mem_t;



#define SIZEOFWORD 4

/// Число слов, которое всегда отводится в пакете на 1 символ, независимо от реальной длины символа,
//   которая определяется кодеком и типом манипуляции.
#define NUMWORDINOFDMSYM   128
/// Максимальное число символов в пакете.   !!! Сейчас под вопросом, остается ли тоже 254 символа в режиме ena_aru
#define MAXNUMSYMBINPACK   254
#define MAXNUMSYM   256
/// Размер памяти в байтах, достаточный для максимального пакета данных (256 символов по 128 32-разрядных слов).
#define MAXPACKAGESIZE  (MAXNUMSYM * NUMWORDINOFDMSYM * sizeof(word32_t))   /// 0x8000
/// Размер одного OFDM символа в байтах - 0x200 (128 32-разрядных слов)
#define NUMBYTEINOFDMSYM (NUMWORDINOFDMSYM * sizeof(word32_t))

//word16_t memToWord16(char *buf, int addr);
//void word16ToMem(char *buf, int num_reg, word16_t val);

void ReadFormat1(char *mem_dst, addr_t mem_src, int num_byte);
void WriteFormat1(char *mem_src, addr_t *mem_dst, int num_byte);

/// Адрес ОУ для групповой команды.
#define BRD_RT_ADDR  31
/// Максимальный рарешенный адрес ОУ - 5 бит
#define MAX_RT_ADDR  31
/// Максимальный рарешенный код команды - 7 бит
#define MAX_COMMAND_CODE  127

/// Биты ответного слова.
#define  tgca_fl_SW_NOTRDY   1
#define  tgca_fl_SW_DEVERR   2
#define  tgca_fl_SW_DATERR   4

/// Признак trbit: направление передачи данных.
#define  tgca_tr_REC  0   // OY принимает
#define  tgca_tr_TRM  1   // ОУ передает
//   бит tr в командном слове
#define  tgca_fl_CW_tr_bit  (1 << 7)
//   значения бита tr в командном слове
#define  tgca_val_CW_tr_REC  (hscl_fl_CW_tr_bit * hscl_tr_REC) // ОУ принимает
#define  tgca_val_CW_tr_TRM  (hscl_fl_CW_tr_bit * hscl_tr_TRM) // ОУ передает
//   извлечение признака tr из командного слова
//#define  hscl_tr_bit_VAL(cw) (((cw) & hscl_CW_tr_bit) >> 7)


#endif // EXTERN_H
