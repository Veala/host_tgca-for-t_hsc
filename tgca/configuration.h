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

#define ROW_REG_NSK_2_ADDR(num) ((num)*4)
#define ROW_REG_VSK_2_ADDR(num) (REGVSKADDRFROM + (num)*4)
#define ADDR_2_VSK_ROW(addr) ((addr)/4-32)
#define NUM_REG_2_ADDR(num) ((num)*4)

#define   config_NUMREG_BEGIN_VSK             0x20

#define   config_NUMREG_ram_tx_rx             0x20
//                                         0x21
#define   config_NUMREG_id                    0x22   // только чтение
#define   config_NUMREG_status                0x23   // только чтение
#define   config_NUMREG_cfg                   0x24
#define   config_NUMREG_tx_cntr               0x25
//                                         0x26
#define   config_NUMREG_rx_cntr               0x27
#define   config_NUMREG_creg                  0x28
#define   config_NUMREG_cr_spi                0x29
#define   config_NUMREG_dr_spi_msw            0x2A
#define   config_NUMREG_dr_spi_lsw            0x2B
//                                         0x2C
#define   config_NUMREG_time_rsp              0x2D
#define   config_NUMREG_cnt_pct_tx_msw        0x2E
#define   config_NUMREG_cnt_pct_tx_lsw        0x2F
#define   config_NUMREG_cnt_pct_rx_msw        0x30
#define   config_NUMREG_cnt_pct_rx_lsw        0x31

#define   config_NUMREG_lvl_sync_kf_rx_msw    0x32   // чтение/запись
#define   config_NUMREG_prcs_max_sync_msw     0x32   // только чтение

#define   config_NUMREG_lvl_sync_kf_rx_lsw    0x33   // чтение/запись
#define   config_NUMREG_prcs_max_sync_lsw     0x33   // только чтение

#define   config_NUMREG_lvl_sync_pre_rx_msw   0x34   // чтение/запись
#define   config_NUMREG_prs_level_max_rn_msw  0x34   // только чтение

#define   config_NUMREG_lvl_sync_pre_rx_lsw   0x35   // чтение/запись
#define   config_NUMREG_prs_level_max_rn_lsw  0x35   // только чтение

#define   config_NUMREG_lvl_qam16             0x36
#define   config_NUMREG_lvl_qam64_low         0x37
#define   config_NUMREG_lvl_qam64_middle      0x38
#define   config_NUMREG_lvl_qam64_high        0x39
#define   config_NUMREG_amplification_factor  0x3A
#define   config_NUMREG_amplitude_signal      0x3B   // только чтение
#define   config_NUMREG_g_sp                  0x3C
#define   config_NUMREG_g_1_sp_high           0x3D
#define   config_NUMREG_g_1_sp_low            0x3E
#define   config_NUMREG_pll_reg               0x3F

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
    QString getName() const { return path; }
    int getConfigReg() const;

private:
    Ui::Configuration *ui;

//    char imprintRegVSK[NUMOFREGVSK * SIZEOFWORD];
//    char imprintRegNSK[NUMOFREGNSK * SIZEOFWORD];
    int currentTab;
    bool stopSign;
    QString path;

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
    void applyRegFlag(int num_reg, word16_t flag, bool b);

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
    bool onPushRead();
    void onExpandVSK(int);
    void onExpandNSK(int);
    void onCheckSelectVSK(int);
    void onCheckSelectNSK(int);
    void onExit(int);

    void on_comboBoxManType_currentIndexChanged(int index);
    void on_checkBoxCodec_clicked(bool checked);
    void on_checkBoxEnaAru_clicked(bool checked);
    void on_comboBoxEnaMemVsk_currentIndexChanged(int index);
    void on_comboBox_BC_RT_currentIndexChanged(int index);
    void on_checkBoxEnaInt_clicked(bool checked);
    void on_checkBoxEnaRtaVSK_clicked(bool checked);
    void on_comboBoxRTA_currentIndexChanged(int index);

    void onCellChangedNSK(int, int);
    void onCellChangedVSK(int, int);
    void on_lineEditQ16_editingFinished();
    void on_lineEditQ16_textChanged(const QString &arg1);
    void on_lineEditQ16_textEdited(const QString &arg1);

    void on_lineEditQ16_cursorPositionChanged(int arg1, int arg2);

    void on_lineEditQ16_returnPressed();

    void on_lineEditQ16_selectionChanged();

    void on_checkBoxResetFL_clicked(bool checked);

    void on_checkBoxResetFLT_clicked(bool checked);

    void on_checkBoxResetFLR_clicked(bool checked);

    void on_checkBoxStartBC_clicked(bool checked);

    void on_comboBoxLvlPre_activated(int index);

    void on_comboBoxLvlCor_activated(int index);

    void on_checkBox_2_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);
    void on_checkBox_4_clicked(bool checked);
    void on_checkBox_5_clicked(bool checked);
    void on_checkBox_6_clicked(bool checked);
    void on_checkBox_7_clicked(bool checked);
    void on_checkBox_8_clicked(bool checked);
    void on_checkBox_9_clicked(bool checked);
    void on_checkBox_10_clicked(bool checked);

    void on_comboBoxSPI1_currentIndexChanged(int index);

    void on_comboBoxSPI2_currentIndexChanged(int index);

    void on_checkBoxSPI_clicked(bool checked);

    void on_checkBoxRX_AMPL0_clicked(bool checked);

    void on_checkBoxRX_AMPL1_clicked(bool checked);

    void on_checkBoxRX_AMPL2_clicked(bool checked);

    void on_checkBoxRX_AMPL3_clicked(bool checked);

    void on_checkBoxRX_OE_AMPL_clicked(bool checked);

    void on_comboBoxFreq_currentIndexChanged(int index);

    void on_comboBoxOff_currentIndexChanged(int index);

    /// Функции, отображающие в окне конфигурации результаты чтения/записи регистров устройства.
    void  doneWriteReg(int);
    bool  doneReadReg(int,QByteArray);

public slots:
    /// Задание конфигурации.
    bool initFrom(QString name, int* err);
    /// Запись регистров.
    bool onPushWrite();
    void blockReadWrite();
    void enableReadWrite();

signals:
    /// Сигналы устройству для чтения/записи регистров.
    void  doWriteReg(QByteArray);
    void  doReadReg(QByteArray);
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
