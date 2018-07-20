#include "configuration.h"
#include "ui_configuration.h"

#include "vskregview.h"
#include "registers.h"

#include <QDebug>

enum REG_COL_NUM
{
    rcn_Addr,
    rcn_Name,
    rcn_Descr,
    rcn_Val,
    /*
    tableWidgetRegRO,
    tableWidgetRegType,
    tableWidgetRegUse,*/
    rcn_End
};

#define NUM_ADDR_2_NUM_REG(addr) ((addr)/4-32)

static char* TXT_lvl_sync_kf_rx_msw_OR_prcs_max_sync_msw[2] = {
    "Регистр уровня корреляционной функции, СЗC",
    "Регистр уровня максимума точной синхронизации, СЗC" };

static char* TXT_lvl_sync_kf_rx_lsw_OR_prcs_max_sync_lsw[2] = {
    "Регистр уровня корреляционной функции, МЗC",
    "Регистр уровня максимума точной синхронизации, МЗC" };

static char* KEY_lvl_sync_kf_rx_msw_OR_prcs_max_sync_msw[2] = {
    "lvl_sync_kf_rx_msw",
    "prcs_max_sync_msw" };

static char* KEY_lvl_sync_kf_rx_lsw_OR_prcs_max_sync_lsw[2] = {
    "lvl_sync_kf_rx_lsw",
    "prcs_max_sync_lsw" };

static char* TXT_lvl_sync_pre_rx_msw_OR_prs_level_max_rn_msw[2] = {
    "Регистр уровня предварительной синхронизации, СЗC",
    "Регистр уровня максимума Rn, СЗC" };

static char* TXT_lvl_sync_pre_rx_lsw_OR_prs_level_max_rn_lsw[2] = {
    "Регистр уровня предварительной синхронизации, МЗC",
    "Регистр уровня максимума Rn, МЗC" };

static char* KEY_lvl_sync_pre_rx_msw_OR_prs_level_max_rn_msw[2] = {
    "lvl_sync_pre_rx_msw",
    "prs_level_max_rn_msw" };

static char* KEY_lvl_sync_pre_rx_lsw_OR_prs_level_max_rn_lsw[2] = {
    "lvl_sync_pre_rx_lsw",
    "prs_level_max_rn_lsw" };


/*static bool regVSKUse[NUMOFREGVSK] = { true, false, false, true, true, false, false, true,
                                      true, false, false, false, false, true, false, false,
                                      false, false, true, true, true, true, true, true,
                                      true, true, false, false, false, false, false, false };

static bool regVSKRO[NUMOFREGVSK] = { false };*/
static bool regNSKRO[NUMOFREGNSK] = { false };
/*
static enum REG_PROC_TYPE regVSKType[NUMOFREGVSK] =
        { rpt_RAM,   rpt_NONE,  rpt_INT,   rpt_STATUS, rpt_CONFIG, rpt_BIT, rpt_NONE,  rpt_RX_CNTR,
          rpt_CREG,  rpt_CR_SPI, rpt_BIT,  rpt_BIT,   rpt_NONE,  rpt_INT,   rpt_INT,   rpt_INT,
          rpt_INT,   rpt_INT,   rpt_INT,   rpt_INT,   rpt_INT,   rpt_INT,   rpt_INT,   rpt_INT,
          rpt_INT,   rpt_INT, rpt_AMPLIFICATION, rpt_INT, rpt_INT, rpt_INT, rpt_INT,   rpt_PLL };
*/

Configuration::Configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(1);
    initNSK();
    initVSK();
}

Configuration::~Configuration()
{
    delete ui;
}

//////////////////////////////////////////////////////////
/// НАСТРОЙКА ВЫВОДА В ОКНО

void resizeCol(QTableView *tab)
{
    tab->setColumnWidth(REG_COL_NUM::rcn_Addr,55);
    //tab->setColumnWidth(REG_COL_NUM::rcn_Name,100);
    tab->setColumnWidth(REG_COL_NUM::rcn_Descr,300);
    //tab->setColumnWidth(REG_COL_NUM::rcn_Val,100);
    tab->setColumnHidden(4,true);
}

void buildTable(QTableWidget *tab)
{
    for (int row=0; row < tab->rowCount(); row++)
    {
        for (int col=0; col <REG_COL_NUM::rcn_End; col++)
        {
            QTableWidgetItem *pItem = tab->item(row, col);
            if (pItem == NULL)
            {
                pItem = new QTableWidgetItem();
                if (pItem == NULL)
                    throw("Внутренняя ошибка в конструкторе Configuration");
                tab->setItem(row, col, pItem);
            }
        }
    }
}

void setRegView(QLineEdit* win, QLabel *label, QLabel *mark, bool ena)
{
    label->setEnabled(ena);
    mark->setEnabled(!ena);
    mark->setVisible(!ena);
    QFont f = win->font();
    f.setBold(ena);
    win->setFont(f);
    QPalette p = win->palette();
    if (ena)
        p.setColor(QPalette::Base, Qt::white);
    else
        p.setColor(QPalette::Base, qRgb(241, 241, 241));
    win->setPalette(p);
     //   win->setStyleSheet("QLabel { background-color : yellow;}");//rgb(241, 241, 241); }");
}

void Configuration::switchReg(int num_reg, QLabel *labelText, QLabel *labelInd, char* description[2], char* name[2], int num)
{
    ui->tableWidgetVSK->item(num_reg, rcn_Descr)->setText(description[num]);
    ui->tableWidgetVSK->item(num_reg, rcn_Name)->setText(name[num]);
    labelText->setText(description[num]);
    labelInd->setVisible(num);
    labelInd->setEnabled(num);
    QFont f = labelText->font();
    f.setBold(num);
    labelText->setFont(f);
    QPalette p = labelText->palette();
    if (num)
        p.setColor(QPalette::Base, Qt::white);
    else
        p.setColor(QPalette::Base, qRgb(241, 241, 241));
    labelText->setPalette(p);
    regVSKRO[num_reg] = num;
}

void Configuration::initVSK()
{
//    regVSKRO[2] = regVSKRO[3] = regVSKRO[27] = true;
//    ReadFormat1(imprintRegVSK, REGVSKADDRFROM, NUMOFREGVSK * SIZEOFWORD);

    buildTable(ui->tableWidgetVSK);
    resizeCol(ui->tableWidgetVSK);
    ui->tableWidgetVSK->setColumnHidden(5,true);
}

void Configuration::initNSK()
{
    regNSKRO[0xB] = regNSKRO[0xC] = regNSKRO[0xE] = regNSKRO[0xF] = true;
    buildTable(ui->tableWidgetNSK);
    resizeCol(ui->tableWidgetNSK);
}

/// Эта функция пересчитывает заполнение всех окон, относящихся к данному регистру,
///  учитывая зависимости от других регистров.
void Configuration::updateRegVSK(int num_reg, char *values)
{
    int addr = REGVSKADDRFROM + num_reg*4;

    QString value = (QString("%1").arg(memToWord16(values, addr), 4, 16, QChar('0'))).toUpper();

    switch (addr)
    {
    case REG_VSK_ram_tx_rx:
        ui->lineEditRAM->setText(value);
        regVSKUse[num_reg] = true;
        break;

    case REG_VSK_id:
        regVSKRO[num_reg] = true;
        /// LLL !!! надо вписать значение в поле на дополнительных страницах, которого пока нет
        break;

    case REG_VSK_status:
        regVSKRO[num_reg] = true;
        /// LLL !!! надо заполнить виджет статуса, которого пока нет
        regVSKUse[num_reg] = true;
        break;

    case REG_VSK_cfg:
    {
        regVSKUse[num_reg] = true;
        word16_t val = 1;//memToWord16(values, addr);
        ui->comboBoxEnaMemVsk->setCurrentIndex(val & fl_REG_CFG_ena_mem_vsk ? 1 : 0);
        setRegView(ui->lineEditRAM, ui->labelRAM, ui->labelMarkRAM, val & fl_REG_CFG_ena_mem_vsk);

        int man_type = val & FL_REG_CFG_type_man;
        ui->comboBoxManType->setCurrentIndex(man_type == val_REG_CFG_type_man_ERROR ? -1 : man_type);
        setRegView(ui->lineEditQ16, ui->labelQ16, ui->labelMarkQ16, man_type == val_REG_CFG_type_man_QAM16);
        setRegView(ui->lineEditQ64l, ui->labelQ64l, ui->labelMarkQ64l, man_type == val_REG_CFG_type_man_QAM64);
        setRegView(ui->lineEditQ64m, ui->labelQ64m, ui->labelMarkQ64m, man_type == val_REG_CFG_type_man_QAM64);
        setRegView(ui->lineEditQ64h, ui->labelQ64h, ui->labelMarkQ64h, man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam16)] = (man_type == val_REG_CFG_type_man_QAM16);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam64_low)] = (man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam64_middle)] = (man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam64_high)] = (man_type == val_REG_CFG_type_man_QAM64);

        ui->checkBoxCodec->setChecked(val & fl_REG_CFG_ena_codec);
        ui->checkBoxEnaAru->setChecked(val & fl_REG_CFG_ena_aru);

        ui->comboBox_BC_RT->setCurrentIndex(val & fl_REG_CFG_mode_rt_bc ? 1 : 0);
        ui->checkBoxEnaInt->setText(ui->checkBoxEnaInt->text() + QString(val & fl_REG_CFG_mode_rt_bc ? "ОУ" : "КШ"));
        ui->checkBoxEnaInt->setChecked(val & fl_REG_CFG_en_rt_bc_int);

        bool b = val & fl_REG_CFG_rtavsk_ena;
        ui->checkBoxEnaRtaVSK->setChecked(b);
        ui->comboBoxRTA->setCurrentIndex((val&FL_REG_CFG_rtavsk)>>7);
        QFont f = ui->comboBoxRTA->font();
        f.setBold(b);
        ui->comboBoxRTA->setFont(f);
        QPalette p = ui->comboBoxRTA->palette();
        if (b)
            p.setColor(QPalette::Base, Qt::white);
        else
            p.setColor(QPalette::Base, qRgb(241, 241, 241));
        ui->comboBoxRTA->setPalette(p);
    }
        break;

    case REG_VSK_tx_cntr:
        break;

    case REG_VSK_rx_cntr:
    {
        regVSKUse[num_reg] = true;
        word16_t val = 0;// memToWord16(values, addr);
        int num1 = val & fl_REG_RX_CNTR_max_Rn_sync_pre ? 1 : 0;
        int num2 = val & fl_REG_RX_CNTR_prcs_max_sync ? 1 : 0;
        ui->comboBoxLvlPre->setCurrentIndex(num1);
        ui->comboBoxLvlCor->setCurrentIndex(num2);

        switchReg(NUM_ADDR_2_NUM_REG(REG_VSK_lvl_sync_kf_rx_msw), ui->label_26, ui->label_25,
                  TXT_lvl_sync_kf_rx_msw_OR_prcs_max_sync_msw,
                  KEY_lvl_sync_kf_rx_msw_OR_prcs_max_sync_msw, num2);
        switchReg(NUM_ADDR_2_NUM_REG(REG_VSK_lvl_sync_kf_rx_lsw), ui->label_24, ui->label_23,
                  TXT_lvl_sync_kf_rx_lsw_OR_prcs_max_sync_lsw,
                  KEY_lvl_sync_kf_rx_lsw_OR_prcs_max_sync_lsw, num2);

        switchReg(NUM_ADDR_2_NUM_REG(REG_VSK_lvl_sync_pre_rx_msw), ui->label_20, ui->label_22,
                  TXT_lvl_sync_pre_rx_msw_OR_prs_level_max_rn_msw,
                  KEY_lvl_sync_pre_rx_msw_OR_prs_level_max_rn_msw, num1);
        switchReg(NUM_ADDR_2_NUM_REG(REG_VSK_lvl_sync_pre_rx_lsw), ui->label_17, ui->label_18,
                  TXT_lvl_sync_pre_rx_lsw_OR_prs_level_max_rn_lsw,
                  KEY_lvl_sync_pre_rx_lsw_OR_prs_level_max_rn_lsw, num1);

    }
        break;

   /* case REG_VSK_creg                  0xA0
    break;

    case REG_VSK_cr_spi                0xA4
    break;

    case REG_VSK_dr_spi_msw            0xA8
    break;

    case REG_VSK_dr_spi_lsw            0xAC
    break;

    case REG_VSK_time_rsp              0xB4
    break;

    case REG_VSK_cnt_pct_tx_msw        0xB8
    break;

    case REG_VSK_cnt_pct_tx_lsw        0xBC
    break;

    case REG_VSK_cnt_pct_rx_msw        0xC0
    break;

    case REG_VSK_cnt_pct_rx_lsw        0xC4
    break;

    case REG_VSK_lvl_sync_kf_rx_msw    0xC8   // чтение/запись
    case REG_VSK_prcs_max_sync_msw     0xC8   // только чтение

    case REG_VSK_lvl_sync_kf_rx_lsw    0xCC   // чтение/запись
    case REG_VSK_prcs_max_sync_lsw     0xCC   // только чтение

    case REG_VSK_lvl_sync_pre_rx_msw   0xD0   // чтение/запись
    case REG_VSK_prs_level_max_rn_msw  0xD0   // только чтение

    case REG_VSK_lvl_sync_pre_rx_lsw   0xD4   // чтение/запись
    case REG_VSK_prs_level_max_rn_lsw  0xD4   // только чтение

    break;

    case REG_VSK_lvl_qam16             0xD8
    break;

    case REG_VSK_lvl_qam64_low         0xDC
    break;

    case REG_VSK_lvl_qam64_middle       0xE0
    break;

    case REG_VSK_lvl_qam64_high        0xE4
    case REG_VSK_amplification_factor  0xE8
    case REG_VSK_amplitude_signal      0xEC   // только чтение
    case REG_VSK_g_sp                  0xF0
    case REG_VSK_g_1_sp_high           0xF4
    case REG_VSK_g_1_sp_low            0xF8
    case REG_VSK_pll_reg               0xFC*/
        default:
            regVSKEd[num_reg] = false;
            break;
    }
    if (regVSKEd[num_reg])
    {
        QTableWidgetItem *pItem = ui->tableWidgetVSK->item(num_reg, REG_COL_NUM::rcn_Val);
        pItem->setText(value);
        pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);
    }
}

//////////////////////////////////////////////////////////
/// СЛОТЫ

void Configuration::onEditRegVSK(int row, int col)
{
    if (col == REG_COL_NUM::rcn_Val)
    {
        QTableWidgetItem* curItem = ui->tableWidgetVSK->item(row, col);
        if (curItem)
        {
            qDebug() << "onEditRegVSK";
            VSKRegView reg(this);
            int row = curItem->row();
            reg.configRegView(/*regVSKType[row],*/ registerVSKInfo_read_only(row));
            reg.exec();
        }
    }
}

void Configuration::onEditRegNSK(int row, int col)
{
    if (col == REG_COL_NUM::rcn_Val)
    {
    qDebug() << "onEditRegNSK";
    }
}

void Configuration::onEditRegVSK(QPoint point)
{
    if (ui->tableWidgetVSK->columnAt(point.rx()) == REG_COL_NUM::rcn_Val)
    {
        QTableWidgetItem* curItem = ui->tableWidgetVSK->itemAt(point);
        if (curItem)
        {
            qDebug() << "onEditRegVSK";
            VSKRegView reg(this);
            int row = curItem->row();
            reg.configRegView(/*regVSKType[row],*/ registerVSKInfo_read_only(row));
            reg.exec();
        }
    }
}

void Configuration::onEditRegNSK(QPoint point)
{
    QTableWidgetItem* curItem = ui->tableWidgetNSK->itemAt(point);
    if (curItem && curItem->column() == REG_COL_NUM::rcn_Val)
    {
    qDebug() << "onEditRegNSK";
    }
}

void Configuration::onChangeTab(int num)
{
    ui->comboBox->setVisible(num <= 1);
    ui->comboBox->setEnabled(num <= 1);
}

void Configuration::onPushSave()
{

}

void Configuration::onPushChoose()
{

}

void Configuration::onPushWriteReg()
{

}

void Configuration::onExpand(int mode)
{
    for (int i=0; i<NUMOFREGNSK; ++i)
        ui->tableWidgetNSK->setRowHidden(i, mode!=0 && !registerNSKInfo_view_always(i));

    for (int i=0; i<NUMOFREGVSK; ++i)
        ui->tableWidgetVSK->setRowHidden(i, mode!=0 && !registerVSKInfo_view_always(i));
}

bool Configuration::registerNSKInfo_read_only(int num_reg) const { return regNSKRO[num_reg]; }
bool Configuration::registerVSKInfo_read_only(int num_reg) const { return regVSKRO[num_reg]; }
bool Configuration::registerNSKInfo_view_always(int num_reg) const { return num_reg <= 0x12 || num_reg == 0x18; }
bool Configuration::registerVSKInfo_view_always(int num_reg) const { return regVSKUse[num_reg]; }

word16_t Configuration::getRegVal(addr_t addr) const
{
    word16_t ret = 0;
    if (addr < REG_VSK_ram_tx_rx)
    {
        QTableWidgetItem *pItem = ui->tableWidgetNSK->item(addr/4, rcn_Val);
        if (pItem)
            ret = pItem->text().toInt();
    }
    else if (addr <= REG_VSK_pll_reg)
    {
        QTableWidgetItem *pItem = ui->tableWidgetNSK->item((addr-REG_VSK_pll_reg)/4, rcn_Val);
        if (pItem)
            ret = pItem->text().toInt();
    }
    return ret;
}

bool Configuration::setRegVal(addr_t addr, word16_t val, bool force)
{
    bool bRet = false;
    if (force)
    {
        if (addr < REG_VSK_ram_tx_rx)
        {
            QTableWidgetItem *pItem = ui->tableWidgetNSK->item(addr/4, rcn_Val);
            if (pItem == NULL)
            {
                pItem = new QTableWidgetItem();
                if (pItem == NULL)
                    return bRet;
                ui->tableWidgetNSK->setItem(addr/4, rcn_Val, pItem);
            }
            QString value = QString("%1").arg(val, 4, 16, QChar('0'));
            pItem->setText(value.toUpper());
            return true;
        }
        else if (addr <= REG_VSK_pll_reg)
        {
            int row = (addr-REG_VSK_pll_reg)/4;
            QTableWidgetItem *pItem = ui->tableWidgetVSK->item(row, rcn_Val);
            if (pItem == NULL)
            {
                pItem = new QTableWidgetItem();
                if (pItem == NULL)
                    return bRet;
                ui->tableWidgetVSK->setItem(row, rcn_Val, pItem);
            }
            QString value = QString("%1").arg(val, 4, 16, QChar('0'));
            pItem->setText(value.toUpper());
            return true;
        }
    }
    return bRet;
}

bool Configuration::update(void* reg)
{
    char *ptr = (char*)reg;
    ptr += REGVSKADDRFROM;
    for (int row=0; row < ui->tableWidgetVSK->rowCount() && row < NUMOFREGVSK; row++)
    {
        regVSKEd[row] = true;
        regVSKRO[row] = false;
        regVSKUse[row] = false;
    }

    for (int row=0; row < ui->tableWidgetVSK->rowCount() && row < NUMOFREGVSK; row++)
    {
        updateRegVSK(row, (char*)reg);
        continue;
        QString value = QString("%1").arg(memToWord16(ptr, row*4), 4, 16, QChar('0'));
        QTableWidgetItem *pItem = ui->tableWidgetVSK->item(row, REG_COL_NUM::rcn_Val);
        pItem->setText(value.toUpper());
        pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);
        pItem = ui->tableWidgetVSK->item(row, 5);
        if (pItem && pItem->text().contains("qam"))
        {
        }


    }

    return true;
}

word16_t memToWord16(char *buf, int addr) { return 0xFFFF; }// *(buf+addr) //num_reg+0xAA0;}
void ReadFormat1(char *mem_dst, addr_t mem_src, int num_byte) {}
