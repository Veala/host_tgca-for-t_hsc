#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <QLabel>

#include "extern_hsc.h"

#define NUMOFREGVSK    32
#define REGVSKADDRFROM 0x80

#define NUMOFREGNSK    32
#define REGNSKADDRFROM 0x00


namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = 0);
    ~Configuration();

    /// Функцию надо вызывать перед каждым show(); Она пересчитывает заполнение формы.
    /// Входной параметр regVSK - указатель на буфер, содержащий все регистры ВСК.
    bool update(void *regAll);

    /// Функции получения значения регистра из формы
    /// и записи значения регистра в форму с пересчетом всех затронутых полей.
    /// Входной параметр addr - адрес регистра ВСК или регистра НСК, как в файле registers.h.
    /// Пока реализовано только для ВСК.
    word16_t getRegVal(addr_t addr) const;
    bool setRegVal(addr_t addr, word16_t val, bool force = true);

private:
    Ui::Configuration *ui;

//    char imprintRegVSK[NUMOFREGVSK * SIZEOFWORD];
//    char imprintRegNSK[NUMOFREGNSK * SIZEOFWORD];

    bool regVSKUse[NUMOFREGVSK] ;
    bool regVSKEd[NUMOFREGVSK];
    bool regVSKRO[NUMOFREGVSK];

    void initVSK();
    void initNSK();
    void updateRegVSK(int num_reg, char *values);

    bool registerNSKInfo_read_only(int num_reg) const;
    bool registerVSKInfo_read_only(int num_reg) const;
    bool registerNSKInfo_view_always(int num_reg) const;
    bool registerVSKInfo_view_always(int num_reg) const;

    void switchReg(int num_reg, QLabel *labelText, QLabel *labelInd, char* description[2], char* name[2], int num);

private slots:
    void onEditRegVSK(int,int);
    void onEditRegNSK(int,int);
    void onEditRegVSK(QPoint);
    void onEditRegNSK(QPoint);
    void onChangeTab(int);
    void onPushSave();
    void onPushChoose();
    void onPushWriteReg();
    void onExpand(int);

};
/*
class registerInfo
{
private:
//    virtual int addr(int) const = 0;
//    virtual QString name(int) const = 0;
//    virtual QString description(int) const = 0;
//    virtual word16_t value(int num_reg) const = 0;
//    virtual void setValue(int num_reg, word16_t val) = 0;
//    virtual bool read_only(int) const = 0;
//    virtual bool view_always(int) const = 0;
public:
};

class registerNSKInfo : public registerInfo
{
private:
    int addr(int) const;
    QString name(int) const;
    QString description(int) const;
    word16_t value(int num_reg) const { return memToWord16(imprintRegNSK, num_reg*4); }
    void setValue(int num_reg, word16_t val) { word16ToMem(imprintRegNSK, num_reg, val); }
    bool read_only(int) const;
    bool view_always(int) const;
};

class registerVSKInfo : public registerInfo
{
private:
    int addr(int) const = 0;
    QString name(int) const = 0;
    QString description(int) const = 0;
    word16_t value(int num_reg) const = 0;
    void setValue(int num_reg, word16_t val) { word16ToMem(imprintRegVSK, num_reg, val); }
    bool read_only(int) const = 0;
    bool view_always(int) const = 0;
};
*/
#endif // CONFIGURATION_H
