#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <QLabel>
#include <QIcon>

#include "extern_hsc.h"
#include "registers.h"
#include "ctableeditcol.h"

// #define SIZEOFWORD 4

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

inline bool regVSKReserve(int row) { return row==1 || row==6 || row == 12; }
CONFIG_DEFAULT_TYPE parsConfigKey(QString name, word16_t& param);

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = 0);
    ~Configuration();

    QString getName() const { return path; }
    int getConfigReg() const;
    /// Запись регистров.
    QByteArray getRegistersToWrite();
    bool readRegisters();
    void setChecked(int num, bool ch);
    void setWritten(int addr, int val);
    bool disInt();
    CTableEditCol *getTableVSK() const;
    CTableEditCol *tableVSK();

    /// Функции, отображающие в окне конфигурации результаты чтения/записи регистров устройства.
    void  doneWriteReg(QByteArray array);
    bool  doneReadReg(int, QByteArray);

private:
    Ui::Configuration *ui;

//    char imprintRegVSK[NUMOFREGVSK * SIZEOFWORD];
//    char imprintRegNSK[NUMOFREGNSK * SIZEOFWORD];
    int currentTab;
    bool stopSign;
    QString path;

    bool regVSKUse[NUMOFREGVSK];
    bool regVSKRO[NUMOFREGVSK];

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
    void markWritten(int addr);

    bool registerNSKInfo_read_only(int num_reg) const;
    bool registerVSKInfo_read_only(int num_reg) const;
    bool registerNSKInfo_view_always(int num_reg) const;
    bool registerVSKInfo_view_always(int num_reg) const;

    void switchRegisterAsgmt(int num_reg, QLabel *labelHeader, QLabel *labelTextL, QLabel *labelTextH,
             QLineEdit *lineL, QLineEdit *lineH, QLabel *labelInd, QString* description, QString* name, int num);

/*
    bool lineEditQ16_Edited;
    bool lineEditQ64l_Edited;
    bool lineEditQ64m_Edited;
    bool lineEditQ64h_Edited;
    bool lineEditTime_Edited;
    bool lineEdit_11_Edited;
    bool lineEdit_10_Edited;
    bool lineEdit_12_Edited;
    bool lineEdit_9_Edited;
    bool lineEditTrCntr_Edited;
    bool lineEditCntTrh_Edited;
    bool lineEditCntTrl_Edited;
    bool lineEditCntReh_Edited;
    bool lineEditCntRel_Edited;
    bool lineEdit_g_sp_Edited;
    bool lineEdit_g1_sph_Edited;
    bool lineEdit_g1_spl_Edited;
    bool lineEdit_ID_Edited;
    bool lineEditAmplitude_Edited;
    bool lineEditMult_Edited;
    bool lineEditDiv_Edited;
    bool lineEditSPIh_Edited;
    bool lineEditSPIl_Edited;
*/
private slots:
    void onEditRegVSK(QPoint);
    void onEditRegNSK(QPoint);
    void onChangeTab(int);
    bool onPushSave();
    void onPushChoose();
    void onExpandVSK(int);
    void onExpandNSK(int);
    void onCheckSelectVSK();
    void onCheckSelectNSK();
    void onExit();

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
    void on_checkBoxResetFL_clicked(bool checked);

    void on_checkBoxResetFLT_clicked(bool checked);

    void on_checkBoxResetFLR_clicked(bool checked);

    void on_checkBoxStartBC_clicked(bool checked);

    void on_comboBoxLvlPre_activated(int index);

    void on_comboBoxLvlCor_activated(int index);

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

    void on_comboBoxRam_activated(int index);

    void on_lineEditQ16_textChanged(const QString &arg1);
    void on_lineEditQ64l_textChanged(const QString &arg1);
    void on_lineEditQ64m_textChanged(const QString &arg1);
    void on_lineEditQ64h_textChanged(const QString &arg1);
    void on_lineEditTime_textChanged(const QString &arg1);
    void on_lineEditAmplitude_textChanged(const QString &arg1);
    void on_lineEdit_ID_textChanged(const QString &arg1);
    void on_lineEdit_g1_sph_textChanged(const QString &arg1);
    void on_lineEdit_g1_spl_textChanged(const QString &arg1);
    void on_lineEdit_g_sp_textChanged(const QString &arg1);
    void on_lineEditCntReh_textChanged(const QString &arg1);
    void on_lineEditCntRel_textChanged(const QString &arg1);
    void on_lineEditCntTrh_textChanged(const QString &arg1);
    void on_lineEditCntTrl_textChanged(const QString &arg1);
    void on_lineEditTrCntr_textChanged(const QString &arg1);
    void on_lineEdit_12_textChanged(const QString &arg1);
    void on_lineEdit_9_textChanged(const QString &arg1);
    void on_lineEdit_11_textChanged(const QString &arg1);
    void on_lineEdit_10_textChanged(const QString &arg1);
    void on_lineEditMult_textChanged(const QString &arg1);
    void on_lineEditDiv_textChanged(const QString &arg1);
    void on_lineEditSPIh_textChanged(const QString &arg1);
    void on_lineEditSPIl_textChanged(const QString &arg1);
/*
    void on_lineEditQ16_editingFinished();
    void on_lineEditQ64l_editingFinished();
    void on_lineEditQ64m_editingFinished();
    void on_lineEditQ64h_editingFinished();
    void on_lineEditTime_editingFinished();
    void on_lineEdit_11_editingFinished();
    void on_lineEdit_10_editingFinished();
    void on_lineEdit_12_editingFinished();
    void on_lineEdit_9_editingFinished();
    void on_lineEditTrCntr_editingFinished();
    void on_lineEditCntTrh_editingFinished();
    void on_lineEditCntTrl_editingFinished();
    void on_lineEditCntReh_editingFinished();
    void on_lineEditCntRel_editingFinished();
    void on_lineEdit_g_sp_editingFinished();
    void on_lineEdit_g1_sph_editingFinished();
    void on_lineEdit_g1_spl_editingFinished();
    void on_lineEdit_ID_editingFinished();
    void on_lineEditAmplitude_editingFinished();
    void on_lineEditMult_editingFinished();
    void on_lineEditDiv_editingFinished();
    void on_lineEditSPIh_editingFinished();
    void on_lineEditSPIl_editingFinished();
*/
public slots:
    /// Задание конфигурации.
    bool initFrom(QString name, int* err);

};

#endif // CONFIGURATION_H
