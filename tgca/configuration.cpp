#include "configuration.h"
#include "ui_configuration.h"

#include "vskregview.h"
#include "registers.h"

#include <QDebug>

enum REG_COL_NUM
{
    rcn_Addr,
    rcn_Name_Check,
    rcn_Descr,
    rcn_Val,
    /*
    tableWidgetRegRO,
    tableWidgetRegType,
    tableWidgetRegUse,*/
    rcn_End
};

static bool regNSKRO[NUMOFREGNSK] = { false };
#define NUM_ADDR_2_NUM_REG(addr) ((addr)/4-32)

static bool regVSKUseGlobal[NUMOFREGVSK] = { true, false, false, true, true, false, false, true,
                                      true, false, false, false, false, true, false, false,
                                      false, false, true, true, true, true, true, true,
                                      true, true, false, false, false, false, false, false };

//static bool regVSKRO[NUMOFREGVSK] = { false };
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

    setWindowFlags(Qt::WindowCloseButtonHint);
    ui->tabWidget->setCurrentIndex(1);
    initNSK();
    initVSK();

    connect(ui->tableWidgetVSK, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChangedVSK(int,int)));
    connect(ui->tableWidgetNSK, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChangedNSK(int,int)));
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
    tab->setColumnWidth(REG_COL_NUM::rcn_Name_Check,120);
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
            if (col == REG_COL_NUM::rcn_Name_Check)
                pItem->setCheckState(Qt::Unchecked);
            if (col == REG_COL_NUM::rcn_Val)
            {
                pItem->setTextColor(Qt::gray);
                pItem->setFlags(pItem->flags() | Qt::ItemIsEditable);
            }
        }
    }
}

void Configuration::initVSK()
{
//    ReadFormat1(imprintRegVSK, REGVSKADDRFROM, NUMOFREGVSK * SIZEOFWORD);

    buildTable(ui->tableWidgetVSK);
    resizeCol(ui->tableWidgetVSK);
    for (int row=0; row < NUMOFREGVSK; row++)
    {
        regVSKEd[row] = true;
        regVSKRO[row] = false;
        regVSKUse[row] = regVSKUseGlobal[row];
    }
    regVSKRO[2] = regVSKRO[3] = regVSKRO[27] = true;
    //ui->tableWidgetVSK->setColumnHidden(5,true);
    ui->comboBoxVSK->setCurrentIndex(0);
 }

void Configuration::initNSK()
{
    regNSKRO[0xB] = regNSKRO[0xC] = regNSKRO[0xE] = regNSKRO[0xF] = true;
    buildTable(ui->tableWidgetNSK);
    resizeCol(ui->tableWidgetNSK);
    ui->comboBoxNSK->setCurrentIndex(0);
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

static const QString HeaderPrefix("• Регистры уровня ");

void Configuration::switchRegisterAsgmt(int num_reg, QLabel *labelHeader, QLabel *labelTextL, QLabel *labelTextH,
                 QLineEdit *lineL, QLineEdit *lineH, QLabel *labelInd, QString* description, QString* name, int num)
{
    ui->tableWidgetVSK->item(num_reg, rcn_Descr)->setText(description[num] + QString(", СЗС"));
    ui->tableWidgetVSK->item(num_reg+1, rcn_Descr)->setText(description[num] + QString(", МЗС"));
    ui->tableWidgetVSK->item(num_reg, rcn_Name_Check)->setText(name[num]);
    ui->tableWidgetVSK->item(num_reg+1, rcn_Name_Check)->setText(name[num+2]);
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

/// Эта функция пересчитывает заполнение всех окон, относящихся к данному регистру и к зависимым регистрам
void Configuration::setRegVSK(int addr, word16_t val)
{
    switch (addr)
    {
    case REG_VSK_ram_tx_rx:               // 0x80 (0x20)
    {
        ui->checkBox_2->setChecked(val & 1);
        ui->checkBox_3->setChecked(val & (1 << 1));
        ui->checkBox_4->setChecked(val & (1 << 2));
        ui->checkBox_5->setChecked(val & (1 << 3));
        ui->checkBox_6->setChecked(val & (1 << 4));
        ui->checkBox_7->setChecked(val & (1 << 5));
        ui->checkBox_8->setChecked(val & (1 << 6));
        ui->checkBox_9->setChecked(val & (1 << 7));
        ui->checkBox_10->setChecked(val & (1 << 8));
    }
        break;

    case REG_VSK_id:                      // 0x88 (0x22)
        /// LLL !!! надо вписать значение в поле на дополнительных страницах, которого пока нет
        break;

    case REG_VSK_status:                  // 0x8C (0x23)
        /// LLL !!! надо заполнить виджет статуса, которого пока нет
        break;

    case REG_VSK_cfg:                     // 0x90 (0x24)
    {
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
        ui->checkBoxResetFL->setChecked(val & fl_REG_CREG_com_res);
        ui->checkBoxResetFLT->setChecked(val & fl_REG_CREG_tx_res);
        ui->checkBoxResetFLR->setChecked(val & fl_REG_CREG_rx_res);
        ui->checkBoxStartBC->setChecked(val & fl_REG_CREG_start_bc);   /// LLL Здесь, возможно, надо изменить иконку этого чекбокса
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
            break;
    }
}

/// Эта функция инициализирует все признаки и пересчитывает заполнение всех окон, относящихся
///  к данному регистру, учитывая зависимости от других регистров.
void Configuration::updateRegVSK(int num_reg, char *values)
{
    int addr = REGVSKADDRFROM + num_reg*4;
    QString value = (QString("%1").arg(memToWord16(values, addr), 4, 16, QChar('0'))).toUpper();
    setRegVSK(addr, memToWord16(values, addr));

    switch (addr)
    {
    case REG_VSK_ram_tx_rx:           // 0x80 (0x20)
    case REG_VSK_id:                  // 0x88 (0x22)
    case REG_VSK_status:              // 0x8C (0x23)
    case REG_VSK_cfg:                 // 0x90 (0x24)
    case REG_VSK_tx_cntr:             // 0x94 (0x25)
    case REG_VSK_rx_cntr:             // 0x9C (0x27)
    case REG_VSK_creg:                //  0xA0 (0x28)
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
    return;
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
    ui->comboBoxVSK->setVisible(num == 1);
    ui->comboBoxVSK->setEnabled(num == 1);
    ui->comboBoxNSK->setVisible(num == 0);
    ui->comboBoxNSK->setEnabled(num == 0);
    ui->checkBoxSelectNSK->setVisible(num == 0);
    ui->checkBoxSelectNSK->setEnabled(num == 0);
    ui->checkBoxSelectVSK->setVisible(num == 1);
    ui->checkBoxSelectVSK->setEnabled(num == 1);
}

void Configuration::onPushSave()
{

}

void Configuration::onPushChoose()
{

}

void Configuration::onPushWrite()
{

}

void Configuration::onPushRead()
{

}

void Configuration::onExpandNSK(int mode)
{
    for (int i=0; i<NUMOFREGNSK; ++i)
    {
        bool b = false;
        if (mode == 1)
            b = (ui->tableWidgetNSK->item(i, REG_COL_NUM::rcn_Name_Check)->checkState() == Qt::Unchecked);
        else if (mode == 0)
            b = !registerNSKInfo_view_always(i);
        ui->tableWidgetNSK->setRowHidden(i, b);
    }
}

void Configuration::onExpandVSK(int mode)
{
    for (int i=0; i<NUMOFREGVSK; ++i)
    {
        bool b = false;
        if (mode == 1)
            b = (ui->tableWidgetVSK->item(i, REG_COL_NUM::rcn_Name_Check)->checkState() == Qt::Unchecked);
        else if (mode == 2)
            b = !registerVSKInfo_view_always(i);
        ui->tableWidgetVSK->setRowHidden(i, b);
    }
}

void Configuration::onCheckSelectVSK(int ch)
{
    qDebug() << ch << "   " << ui->checkBoxSelectVSK->checkState();
    if (ui->checkBoxSelectVSK->checkState() == Qt::PartiallyChecked)
    {
        ui->checkBoxSelectVSK->blockSignals(true);
        ui->checkBoxSelectVSK->setCheckState(Qt::Checked);
        ui->checkBoxSelectVSK->blockSignals(false);
    }
    Qt::CheckState b = ui->checkBoxSelectVSK->checkState();
    ui->tableWidgetVSK->blockSignals(true);
    for (int i=0; i<ui->tableWidgetVSK->rowCount();i++)
        ui->tableWidgetVSK->item(i, rcn_Name_Check)->setCheckState(b);
    ui->tableWidgetVSK->blockSignals(false);
}

void Configuration::onCheckSelectNSK(int ch)
{
    qDebug() << ch << "   " << ui->checkBoxSelectNSK->checkState();
    if (ui->checkBoxSelectNSK->checkState() == Qt::PartiallyChecked)
    {
        ui->checkBoxSelectNSK->blockSignals(true);
        ui->checkBoxSelectNSK->setCheckState(Qt::Checked);
        ui->checkBoxSelectNSK->blockSignals(false);
    }
    Qt::CheckState b = ui->checkBoxSelectNSK->checkState();
    ui->tableWidgetNSK->blockSignals(true);
    for (int i=0; i<ui->tableWidgetNSK->rowCount();i++)
        ui->tableWidgetNSK->item(i, rcn_Name_Check)->setCheckState(b);
    ui->tableWidgetNSK->blockSignals(false);
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

/// Изменение регистра в форме
bool Configuration::setRegVal(addr_t addr, word16_t val, bool force)
{
    bool bRet = false;
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
        if (!force)
        {
            setRegVSK(row, val);
            regVSKChecked[row] = true;
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
        regVSKUse[row] = regVSKUseGlobal[row];
        //regVSKChecked[row] = false;
    }
    regVSKRO[2] = regVSKRO[3] = regVSKRO[27] = true;

    ui->tableWidgetVSK->blockSignals(true);

    for (int row=0; row < ui->tableWidgetVSK->rowCount() && row < NUMOFREGVSK; row++)
    {
        updateRegVSK(row, (char*)reg);
        continue;
        QString value = QString("%1").arg(memToWord16(ptr, row*4), 4, 16, QChar('0'));
        QTableWidgetItem *pItem = ui->tableWidgetVSK->item(row, REG_COL_NUM::rcn_Val);
        pItem->setText(value.toUpper());
/// ...

    }
    ui->tableWidgetVSK->blockSignals(false);

    return true;
}

bool matchSTD0(const QStringList& list, word16_t& param)
{
    bool bRet = false;
    param = 0;

    if (list[1] == "RT")
         param |= val_REG_CFG_mode_RT;
    else if (list[1] == "BC")
        param |= val_REG_CFG_mode_BC;
    else
        return bRet;

    if (list[2] == "INT")
        param |= fl_REG_CFG_en_rt_bc_int;
    else if (list[2] != "NOINT")
        return bRet;

    if (list[3] == "QPSK")
        param |= val_REG_CFG_type_man_QPSK;
    else if (list[3] == "QAM16")
        param |= val_REG_CFG_type_man_QAM16;
    else if (list[3] == "QAM16")
        param |= val_REG_CFG_type_man_QAM64;
    else
        return bRet;

    if (list[4] == "CODEC")
        param |= fl_REG_CFG_ena_codec;
    else if (list[4] != "NOCODEC")
        return bRet;

    if (list.size() == 6)
    {
        if (list[1] == "BC")
            return bRet;
        int addr = 0;
        if (list[5] == "EN1")
            addr = 1;
        else if (list[5] == "EN2")
            addr = 2;
        else if (list[5] != "EN0")
            return bRet;
        param |= fl_REG_CFG_rtavsk_ena;
        param |= (addr << 7);
    }
    return true;
}

CONFIG_DEFAULT_TYPE parsConfigKey(QString name, word16_t& param)
{
    CONFIG_DEFAULT_TYPE ret = cdt_Error;

    QStringList list = name.split(QRegExp("_"));
    if (list.isEmpty())
        return ret;
    int sz = list.size();
    if (sz<5 || sz>6)
        return ret;
    if (list[0] == "STD0")  // шаблон  "STD0_BC_NO_QPSK_CODEC_EN2"
    {
        if (matchSTD0(list, param))
            return cdt_Default;
    }
    /// могут быть другие шаблоны

    return ret;
}

bool Configuration::setConfigDefault0(word16_t reg_config)
{
    setRegVal(REG_VSK_cfg, reg_config);
    setRegVal(REG_VSK_creg, REG_VSK_creg_INIT_RESET);
    setRegVal(REG_VSK_time_rsp, REG_VSK_time_rsp_INIT_DEFAULT);

    setRegVal(REG_VSK_lvl_sync_kf_rx_msw, REG_VSK_lvl_sync_kf_rx_msw_INIT_DEFAULT);
    setRegVal(REG_VSK_lvl_sync_kf_rx_lsw, REG_VSK_lvl_sync_kf_rx_lsw_INIT_DEFAULT);
    setRegVal(REG_VSK_lvl_sync_pre_rx_msw, REG_VSK_lvl_sync_pre_rx_msw_INIT_DEFAULT);
    setRegVal(REG_VSK_lvl_sync_pre_rx_lsw, REG_VSK_lvl_sync_pre_rx_lsw_INIT_DEFAULT);

    int man_type = reg_config & FL_REG_CFG_type_man;
    if (man_type == val_REG_CFG_type_man_QAM16)
        setRegVal(REG_VSK_lvl_qam16, REG_VSK_lvl_qam16_INIT_DEFAULT);
    if (man_type == val_REG_CFG_type_man_QAM64)
    {
        setRegVal(REG_VSK_lvl_qam64_low, REG_VSK_lvl_qam64_low_INIT_DEFAULT);
        setRegVal(REG_VSK_lvl_qam64_middle, REG_VSK_lvl_qam64_midle_INIT_DEFAULT);
        setRegVal(REG_VSK_lvl_qam64_high, REG_VSK_lvl_qam64_high_INIT_DEFAULT);
    }
    return true;
}

bool Configuration::initDefault(CONFIG_DEFAULT_TYPE key, word16_t param)
{
    switch (key)
    {
    case cdt_Default:
        return setConfigDefault0(param);
    }
    return false;
}

bool initFromFile(QString name)
{
    return false;
}

bool Configuration::initFrom(QString name)
{
    if (name.isEmpty())
        return false;
    word16_t param;
    CONFIG_DEFAULT_TYPE ret = parsConfigKey(name, param);
    if (ret != cdt_Error)
        return initDefault(ret, param);

    return initFromFile(name);
}


word16_t memToWord16(char *buf, int addr) { return 0xFFFF; }// *(buf+addr) //num_reg+0xAA0;}
void ReadFormat1(char *mem_dst, addr_t mem_src, int num_byte) {}

void Configuration::onCellChangedVSK(int row, int col)
{
   if (col == rcn_Name_Check)
   {
       ui->checkBoxSelectVSK->blockSignals(true);
       ui->checkBoxSelectVSK->setCheckState(Qt::PartiallyChecked);
       ui->checkBoxSelectVSK->blockSignals(false);
   }
}

void Configuration::onCellChangedNSK(int row, int col)
{
   if (col == rcn_Name_Check)
   {
       ui->checkBoxSelectNSK->blockSignals(true);
       ui->checkBoxSelectNSK->setCheckState(Qt::PartiallyChecked);
       ui->checkBoxSelectNSK->blockSignals(false);
   }
}
