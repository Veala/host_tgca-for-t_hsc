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

const QString HeaderPrefix("• Регистры уровня ");

static QString TXT_lvl_sync_kf_rx_OR_prcs_max_sync[4] = {
    "Регистр уровня корреляционной функции",
    "Регистр уровня максимума точной синхронизации",
    "корреляционной функции",
    "максимума точной синхронизации" };

static QString KEY_lvl_sync_kf_rx_OR_prcs_max_sync[4] = {
    "lvl_sync_kf_rx_msw",
    "prcs_max_sync_msw",
    "lvl_sync_kf_rx_lsw",
    "prcs_max_sync_lsw" };

static QString TXT_lvl_sync_pre_rx_OR_prs_level_max_rn[4] = {
    "Регистр уровня предварительной синхронизации",
    "Регистр уровня максимума Rn",
    "Регистры уровня предварительной синхронизации",
    "Регистры уровня максимума Rn" };

static QString KEY_lvl_sync_pre_rx_OR_prs_level_max_rn[4] = {
    "lvl_sync_pre_rx_msw",
    "prs_level_max_rn_msw",
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
        for (int col=0; col < REG_COL_NUM::rcn_End; col++)
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

/////////////////////////////////////////////////////////////
/// "ПЕРЕКРАШИВАНИЕ" ЗАВИСИМЫХ ПОЛЕЙ

void markRegisterEnabled(QLineEdit* win, QLabel *label, QLabel *mark, bool ena)
{
    label->setEnabled(ena);
    mark->setEnabled(!ena);
    mark->setVisible(!ena);
    QFont f = win->font();
    f.setBold(ena);
    win->setFont(f);
    QPalette p = win->palette();
    p.setColor(QPalette::Base, ena ? Qt::white : qRgb(241, 241, 241));
    win->setPalette(p);
    /*
    if (ena)
        p.setColor(QPalette::Base, Qt::white);
    else
        p.setColor(QPalette::Base, qRgb(241, 241, 241));
    win->setPalette(p);
     //   win->setStyleSheet("QLabel { background-color : yellow;}");//rgb(241, 241, 241); }");
     */
}

void Configuration::switchRegisterAsgmt(int num_reg, QLabel *labelHeader, QLabel *labelTextL, QLabel *labelTextH,
                 QLineEdit *lineL, QLineEdit *lineH, QLabel *labelInd, QString* description, QString* name, int num)
{
    ui->tableWidgetVSK->item(num_reg, rcn_Descr)->setText(description[num] + QString(", СЗС"));
    ui->tableWidgetVSK->item(num_reg+1, rcn_Descr)->setText(description[num] + QString(", МЗС"));
    ui->tableWidgetVSK->item(num_reg, rcn_Name)->setText(name[num]);
    ui->tableWidgetVSK->item(num_reg+1, rcn_Name)->setText(name[num+2]);
    labelHeader->setText(HeaderPrefix + description[num+2]);
    labelInd->setVisible(num);
    labelInd->setEnabled(num);
    QFont f = labelTextL->font();
    f.setBold(num==0);
    //labelHeader->setFont(f);
    labelTextL->setFont(f);
    labelTextH->setFont(f);
    lineL->setFont(f);
    lineH->setFont(f);
    QPalette p = lineL->palette();
    if (num)
        p.setColor(QPalette::Base, qRgb(241, 241, 241));
    else
        p.setColor(QPalette::Base, Qt::white);
    lineL->setPalette(p);
    lineH->setPalette(p);
    regVSKRO[num_reg] = num;
}


/// Эта функция пересчитывает заполнение всех окон, относящихся к данному регистру,
///  учитывая зависимости от других регистров.
void Configuration::updateRegVSK(int num_reg, char *values)
{
    int addr = REGVSKADDRFROM + num_reg*4;
    QString value = (QString("%1").arg(memToWord16(values, addr), 4, 16, QChar('0'))).toUpper();

    switch (addr)
    {
    case REG_VSK_ram_tx_rx:               // 0x80 (0x20)
    {
        word16_t val = memToWord16(values, addr);

        val = 5;
        ui->checkBox_2->setChecked(val & 1);
        ui->checkBox_3->setChecked(val & (1 << 1));
        ui->checkBox_4->setChecked(val & (1 << 2));
        ui->checkBox_5->setChecked(val & (1 << 3));
        ui->checkBox_6->setChecked(val & (1 << 4));
        ui->checkBox_7->setChecked(val & (1 << 5));
        ui->checkBox_8->setChecked(val & (1 << 6));
        ui->checkBox_9->setChecked(val & (1 << 7));
        ui->checkBox_10->setChecked(val & (1 << 8));
        regVSKUse[num_reg] = true;
    }
        break;

    case REG_VSK_id:                      // 0x88 (0x22)
        regVSKRO[num_reg] = true;
        /// LLL !!! надо вписать значение в поле на дополнительных страницах, которого пока нет
        break;

    case REG_VSK_status:                  // 0x8C (0x23)
        regVSKRO[num_reg] = true;
        /// LLL !!! надо заполнить виджет статуса, которого пока нет
        regVSKUse[num_reg] = true;
        break;

    case REG_VSK_cfg:                     // 0x90 (0x24)
    {
        regVSKUse[num_reg] = true;
        word16_t val = memToWord16(values, addr);
        ui->comboBoxEnaMemVsk->setCurrentIndex(val & fl_REG_CFG_ena_mem_vsk ? 1 : 0);

        ui->groupBoxRAM->setEnabled(val & fl_REG_CFG_ena_mem_vsk);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(true);

        int man_type = val & FL_REG_CFG_type_man;
        ui->comboBoxManType->setCurrentIndex(man_type == val_REG_CFG_type_man_ERROR ? -1 : man_type);
        markRegisterEnabled(ui->lineEditQ16, ui->labelQ16, ui->labelMarkQ16, man_type == val_REG_CFG_type_man_QAM16);
        markRegisterEnabled(ui->lineEditQ64l, ui->labelQ64l, ui->labelMarkQ64l, man_type == val_REG_CFG_type_man_QAM64);
        markRegisterEnabled(ui->lineEditQ64m, ui->labelQ64m, ui->labelMarkQ64m, man_type == val_REG_CFG_type_man_QAM64);
        markRegisterEnabled(ui->lineEditQ64h, ui->labelQ64h, ui->labelMarkQ64h, man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam16)] = (man_type == val_REG_CFG_type_man_QAM16);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam64_low)] = (man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam64_middle)] = (man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[NUM_ADDR_2_NUM_REG(REG_VSK_lvl_qam64_high)] = (man_type == val_REG_CFG_type_man_QAM64);

        ui->checkBoxCodec->setChecked(val & fl_REG_CFG_ena_codec);
        ui->checkBoxEnaAru->setChecked(val & fl_REG_CFG_ena_aru);

        bool rt = ((val & fl_REG_CFG_mode_rt_bc) != 0);
        ui->comboBox_BC_RT->setCurrentIndex(rt ? 1 : 0);
        ui->checkBoxEnaInt->setText(ui->checkBoxEnaInt->text() + QString(val & fl_REG_CFG_mode_rt_bc ? "ОУ" : "КШ"));
        ui->checkBoxEnaInt->setChecked(val & fl_REG_CFG_en_rt_bc_int);

        bool b = val & fl_REG_CFG_rtavsk_ena;
        ui->checkBoxEnaRtaVSK->setChecked(b);
        ui->comboBoxRTA->setCurrentIndex((val&FL_REG_CFG_rtavsk)>>7);
        QFont f = ui->comboBoxRTA->font();
        b = b && rt;
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

    case REG_VSK_tx_cntr:                  // 0x94 (0x25)
        break;

    case REG_VSK_rx_cntr:                  // 0x9C (0x27)
    {
        regVSKUse[num_reg] = true;
        word16_t val = memToWord16(values, addr);
        int num1 = val & fl_REG_RX_CNTR_max_Rn_sync_pre ? 1 : 0;
        ui->comboBoxLvlPre->setCurrentIndex(num1);

        switchRegisterAsgmt(NUM_ADDR_2_NUM_REG(REG_VSK_lvl_sync_pre_rx_msw), ui->label_20, ui->label_8, ui->label_9,
                  ui->lineEdit_10, ui->lineEdit_11, ui->label_18, TXT_lvl_sync_pre_rx_OR_prs_level_max_rn,
                  KEY_lvl_sync_pre_rx_OR_prs_level_max_rn, num1);

        int num2 = val & fl_REG_RX_CNTR_prcs_max_sync ? 1 : 0;
        ui->comboBoxLvlCor->setCurrentIndex(num2);

        switchRegisterAsgmt(NUM_ADDR_2_NUM_REG(REG_VSK_lvl_sync_kf_rx_msw), ui->label_26, ui->label_6, ui->label_7,
                  ui->lineEdit_9, ui->lineEdit_12, ui->label_25, TXT_lvl_sync_kf_rx_OR_prcs_max_sync,
                  KEY_lvl_sync_kf_rx_OR_prcs_max_sync, num2);
    }
        break;

    case REG_VSK_creg:                //  0xA0 (0x28)
    {
        regVSKUse[num_reg] = true;
        word16_t val = memToWord16(values, addr);
        ui->checkBoxResetFL->setChecked(val & fl_REG_CREG_com_res);
        ui->checkBoxResetFLT->setChecked(val & fl_REG_CREG_tx_res);
        ui->checkBoxResetFLR->setChecked(val & fl_REG_CREG_rx_res);
        ui->checkBoxStartBC->setChecked(val & fl_REG_CREG_start_bc);   /// LLL Здесь, возможно, надо изменить иконку этого чекбокса
    }
    break;
/*
    case REG_VSK_cr_spi                0xA4 (0x29)
    break;

    case REG_VSK_dr_spi_msw            0xA8 (0x2A)
    break;

    case REG_VSK_dr_spi_lsw            0xAC (0x2B)
    break;

    case REG_VSK_time_rsp              0xB4 (0x2D)
    break;

    case REG_VSK_cnt_pct_tx_msw        0xB8 (0x2E)
    break;

    case REG_VSK_cnt_pct_tx_lsw        0xBC (0x2F)
    break;

    case REG_VSK_cnt_pct_rx_msw        0xC0 (0x30)
    break;

    case REG_VSK_cnt_pct_rx_lsw        0xC4 (0x31)
    break;

    case REG_VSK_lvl_sync_kf_rx_msw    0xC8 (0x32)  // чтение/запись
    case REG_VSK_prcs_max_sync_msw     0xC8   // только чтение

    case REG_VSK_lvl_sync_kf_rx_lsw    0xCC  (0x33) // чтение/запись
    case REG_VSK_prcs_max_sync_lsw     0xCC   // только чтение

    case REG_VSK_lvl_sync_pre_rx_msw   0xD0  (0x34) // чтение/запись
    case REG_VSK_prs_level_max_rn_msw  0xD0   // только чтение

    case REG_VSK_lvl_sync_pre_rx_lsw   0xD4  (0x35) // чтение/запись
    case REG_VSK_prs_level_max_rn_lsw  0xD4   // только чтение

    break;

    case REG_VSK_lvl_qam16             0xD8 (0x36)
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
        default:                                     // 0x84, 0x98, 0xB0
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
