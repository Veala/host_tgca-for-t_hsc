#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QIcon>

#include "extern_hsc.h"
#include "registers.h"

namespace Ui {
class Configuration;
}

enum CONFIG_DEFAULT_TYPE
{
    cdt_Default,
    cdt_Default_S,
    cdt_Error
};

enum REG_COL_NUM
{
    rcn_Addr,
    rcn_Name_Check,
    rcn_Descr,
    rcn_Val,
    rcn_End
};

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = 0);
    ~Configuration();

/// Функции, заполняющие значения регистров в форме.
/// Они пересчитывают заполнение всех зависимых полей формы.

    /// Заполнение всех регистров.
    /// Входной параметр regVSK - указатель на буфер, содержащий все регистры ВСК.
    bool update(void *regAll);

private:
    Ui::Configuration *ui;

//    char imprintRegVSK[NUMOFREGVSK * SIZEOFWORD];
//    char imprintRegNSK[NUMOFREGNSK * SIZEOFWORD];
    int currentTab;
    bool stopSign;

    bool regVSKUse[NUMOFREGVSK];
    bool regVSKRO[NUMOFREGVSK];
    //bool regVSKChecked[NUMOFREGVSK];
    bool regVSKRes[NUMOFREGVSK];

    void initVSK();
    void initNSK();
    void updateRegVSK(int num_reg, word16_t val);
    bool initDefault(CONFIG_DEFAULT_TYPE key, word16_t param);
    bool initFromFile(QString name);
    bool setConfigDefault0(word16_t reg_config, bool all);
    /// Функции получения значения регистра из формы
    /// и записи значения регистра в форму с пересчетом всех затронутых полей.
    /// Входной параметр addr - адрес регистра ВСК или регистра НСК, как в файле registers.h.
    /// Пока реализовано только для ВСК.
    word16_t getRegVal(addr_t addr) const;
    bool setRegVal(addr_t addr, word16_t val, bool force = false);
    void adaptRegVSK(int num_reg, word16_t val, QString strval);
    void chgCfgFlag(word16_t flag, bool b);

    bool registerNSKInfo_read_only(int num_reg) const;
    bool registerVSKInfo_read_only(int num_reg) const;
    bool registerNSKInfo_view_always(int num_reg) const;
    bool registerVSKInfo_view_always(int num_reg) const;

    void switchRegisterAsgmt(int num_reg, QLabel *labelHeader, QLabel *labelTextL, QLabel *labelTextH,
             QLineEdit *lineL, QLineEdit *lineH, QLabel *labelInd, QString* description, QString* name, int num);

    //QIcon iconStartEna, iconStartDis;

private slots:
    void onEditRegVSK(QPoint);
    void onEditRegNSK(QPoint);
    void onChangeTab(int);
    bool onPushSave();
    void onPushChoose();
    void onPushWrite();
    void onPushRead();
    void onExpandVSK(int);
    void onExpandNSK(int);
    void onCheckSelectVSK(int);
    void onCheckSelectNSK(int);
    void onExit(int);

    void onManTypeChanged(int index);
    void onCodecChanged(bool checked);
    void onEnaAruChanged(bool checked);
    void onEnaMemVskChanged(int index);
    void on_comboBox_BC_RT_currentIndexChanged(int index);
    void onEnaIntChanged(bool checked);
    void onEnaRtaVskChanged(bool checked);
    void onRTA_Choose(int index);

    void onCellChangedNSK(int, int);
    void onCellChangedVSK(int, int);
    void  on_lineEditQ16_editingFinished();
    void on_lineEditQ64l_textChanged(const QString &arg1);
    void on_lineEditQ64m_textEdited(const QString &arg1);
    void on_lineEditQ64h_textChanged(const QString &arg1);

public slots:
    /// Задание конфигурации.
    bool initFrom(QString name, int* err);
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
