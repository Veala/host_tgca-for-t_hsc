#include "configuration.h"
#include "ui_configuration.h"

#include "confselect.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

//const bool  DebugL = false;

static bool regNSKRO[NUMOFREGNSK] = { false };

bool regVSKUseGlobal[NUMOFREGVSK] = { false, false, false, true, true, false, false, false,
                                      true, false, false, false, false, true, true, true,
                                      true, true, true, true, true, true, true, true,
                                      true, true, true, false, false, false, false, false };

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
    ui(new Ui::Configuration),
    currentTab(-1),
    stopSign(false),
    path("")
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

CTableEditCol* Configuration::getTableVSK() const
{
    return ui->tableWidgetVSK;
}

CTableEditCol* Configuration::tableVSK()
{
    return ui->tableWidgetVSK;
}

//////////////////////////////////////////////////////////
/// НАСТРОЙКА ВЫВОДА В ОКНО

void resizeCol(QTableView *tab)
{
    tab->setColumnWidth(REG_COL_NUM::rcn_Addr,55);
    tab->setColumnWidth(REG_COL_NUM::rcn_Name_Check,120);
    tab->setColumnWidth(REG_COL_NUM::rcn_Descr,300);
    //tab->setColumnWidth(REG_COL_NUM::rcn_Val,100);
}

void buildTable(CTableEditCol *tab)
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
        tab->item(row, REG_COL_NUM::rcn_Val)->setTextColor(Qt::gray);
        tab->item(row, REG_COL_NUM::rcn_Name_Check)->setCheckState(Qt::Unchecked);
    }
}

void Configuration::initVSK()
{
    buildTable(ui->tableWidgetVSK);
    resizeCol(ui->tableWidgetVSK);
    for (int row=0; row < NUMOFREGVSK; row++)
    {
        regVSKRO[row] = false;
        regVSKUse[row] = regVSKUseGlobal[row];
    }
    regVSKRO[2] = regVSKRO[3] = regVSKRO[27] = true;
    ui->comboBoxVSK->setCurrentIndex(0);
    ui->checkBoxStartBC->setIcon(QIcon());

    ui->labelMarkQ16->setVisible(false);
    ui->labelMarkQ64l->setVisible(false);
    ui->labelMarkQ64m->setVisible(false);
    ui->labelMarkQ64h->setVisible(false);
    ui->labelC8CCRO->setVisible(false);
    ui->labelD0D4RO->setVisible(false);

    ui->comboBoxEnaMemVsk->setCurrentText("");
    ui->comboBox_BC_RT->setCurrentText("");
    ui->comboBoxManType->setCurrentText("");
    ui->comboBoxRTA->setCurrentText("");
    ui->comboBoxLvlCor->setCurrentText("");
    ui->comboBoxLvlPre->setCurrentText("");
/*
    lineEditQ16_Edited = lineEditQ64l_Edited = lineEditQ64m_Edited = lineEditQ64h_Edited =
    lineEditTime_Edited = lineEdit_11_Edited = lineEdit_10_Edited = lineEdit_12_Edited =
    lineEdit_9_Edited = lineEditTrCntr_Edited = lineEditCntTrh_Edited = lineEditCntTrl_Edited =
    lineEditCntReh_Edited = lineEditCntRel_Edited = lineEdit_g_sp_Edited = lineEdit_g1_sph_Edited =
    lineEdit_g1_spl_Edited = lineEdit_ID_Edited = lineEditAmplitude_Edited = lineEditMult_Edited =
    lineEditDiv_Edited = lineEditSPIh_Edited = lineEditSPIl_Edited = false;
*/
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

static void camouflage(QWidget* wid, bool mask)
{
    QFont f = wid->font();
    f.setBold(mask);
    wid->setFont(f);
    QPalette p = wid->palette();
    if (mask)
        p.setColor(QPalette::Base, qRgb(255, 255, 255)); //Qt::white);  //Window
    else
        p.setColor(QPalette::Base, qRgb(241, 241, 241));
    wid->setPalette(p);
}

void markRegisterEnabled(QLineEdit* win, QLabel *label, QLabel *mark, bool ena)
{
    label->setEnabled(ena);
    mark->setEnabled(!ena);
    mark->setVisible(!ena);
    camouflage(win, ena);
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
    //labelInd->setEnabled(num);
    QFont f = labelTextL->font();
    f.setBold(num==0);
    labelTextL->setFont(f);
    labelTextH->setFont(f);
    lineL->setFont(f);
    lineH->setFont(f);
    f.setBold(true);
    labelHeader->setFont(f);
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
    "предварительной синхронизации",
    "максимума Rn" };

static QString KEY_lvl_sync_pre_rx_OR_prs_level_max_rn[4] = {
    "lvl_sync_pre_rx_msw",
    "prs_level_max_rn_msw",
    "lvl_sync_pre_rx_lsw",
    "prs_level_max_rn_lsw" };


/////////////////////////////////////////////////////////////////////////
/// Эта функция пересчитывает заполнение всех окон, относящихся к данному регистру и к зависимым регистрам
void Configuration::adaptRegVSK(int addr, word16_t val, QString strval)
{
    switch (addr)
    {
    case REG_VSK_ram_tx_rx:               // 0x80 (0x20)
    {
        if (val == 0)
            ui->comboBoxRam->setCurrentIndex(0);
        else
        for (int i=1; i<9; i++)
        {
            if (val & 1)
            {
                ui->comboBoxRam->setCurrentIndex(i);
                break;
            }
            val = val >> 1;
        }
    }
        break;

    case REG_VSK_id:                      // 0x88 (0x22)
        ui->lineEdit_ID->setText(strval);
        break;

    case REG_VSK_status:                  // 0x8C (0x23)
        ui->lineEditStatInt->setText((val & fl_REG_STATUS_rt_bc_int) ? "0" : "1");
        ui->lineEditStatErCodec->setText((val & fl_REG_STATUS_rs_err) ? "0" : "1");
        ui->lineEditStatNoResp->setText((val & fl_REG_STATUS_no_aw_err) ? "0" : "1");
        ui->lineEditStatErBroad->setText((val & fl_REG_STATUS_yes_aw_gr_err) ? "0" : "1");
        ui->lineEditStatBufT->setText((val & fl_REG_STATUS_tx_num_buf) ? "0" : "1");
        ui->lineEditStatBufR->setText((val & fl_REG_STATUS_rx_num_buf) ? "0" : "1");
        break;

    case REG_VSK_cfg:                     // 0x90 (0x24)
    {
        ui->comboBoxEnaMemVsk->setCurrentIndex(val & fl_REG_CFG_ena_mem_vsk ? 1 : 0);
        camouflage(ui->comboBoxRam, val & fl_REG_CFG_ena_mem_vsk);
        QString title("•	Регистр доступа к внутренней памяти");
        if ((val & fl_REG_CFG_ena_mem_vsk) == 0)
            title += "   (доступ выключен)";
        ui->labelRam->setText(title);

        int man_type = manType(val);
        ui->comboBoxManType->setCurrentIndex(man_type == val_REG_CFG_type_man_ERROR ? -1 : man_type);
        markRegisterEnabled(ui->lineEditQ16, ui->labelQ16, ui->labelMarkQ16, man_type == val_REG_CFG_type_man_QAM16);
        markRegisterEnabled(ui->lineEditQ64l, ui->labelQ64l, ui->labelMarkQ64l, man_type == val_REG_CFG_type_man_QAM64);
        markRegisterEnabled(ui->lineEditQ64m, ui->labelQ64m, ui->labelMarkQ64m, man_type == val_REG_CFG_type_man_QAM64);
        markRegisterEnabled(ui->lineEditQ64h, ui->labelQ64h, ui->labelMarkQ64h, man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam16)] = (man_type == val_REG_CFG_type_man_QAM16);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_low)] = (man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_middle)] = (man_type == val_REG_CFG_type_man_QAM64);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_high)] = (man_type == val_REG_CFG_type_man_QAM64);

        ui->checkBoxCodec->setChecked(val & fl_REG_CFG_ena_codec);
        ui->checkBoxEnaAru->setChecked(val & fl_REG_CFG_ena_aru);

        bool rt = ((val & fl_REG_CFG_mode_rt_bc) != 0);
        ui->comboBox_BC_RT->setCurrentIndex(rt ? 1 : 0);
        ui->checkBoxEnaInt->setText(QString("разрешение выработки прерывания в режиме ") + QString(rt ? "ОУ" : "КШ"));
        ui->label_14->setText(QString("флаг прерывания от ") + QString(rt ? "ОУ" : "КШ"));
        ui->checkBoxEnaInt->setChecked(val & fl_REG_CFG_en_rt_bc_int);

        bool b = val & fl_REG_CFG_rtavsk_ena;
        ui->checkBoxEnaRtaVSK->setChecked(b);
        ui->comboBoxRTA->setCurrentIndex((val&FL_REG_CFG_rtavsk)>>7);
        camouflage(ui->comboBoxRTA, b && rt);
    }
        break;

    case REG_VSK_tx_cntr:                  // 0x94 (0x25)
        ui->lineEditTrCntr->setText(strval);
        break;

    case REG_VSK_rx_cntr:                  // 0x9C (0x27)
    {
        int num1 = val & fl_REG_RX_CNTR_max_Rn_sync_pre ? 1 : 0;
        ui->comboBoxLvlPre->setCurrentIndex(num1);

        switchRegisterAsgmt(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_msw), ui->label_20, ui->label_8, ui->label_9,
                  ui->lineEdit_10, ui->lineEdit_11, ui->labelD0D4RO, TXT_lvl_sync_pre_rx_OR_prs_level_max_rn,
                  KEY_lvl_sync_pre_rx_OR_prs_level_max_rn, num1);

        int num2 = val & fl_REG_RX_CNTR_prcs_max_sync ? 1 : 0;
        ui->comboBoxLvlCor->setCurrentIndex(num2);

        switchRegisterAsgmt(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_msw), ui->label_26, ui->label_6, ui->label_7,
                  ui->lineEdit_9, ui->lineEdit_12, ui->labelC8CCRO, TXT_lvl_sync_kf_rx_OR_prcs_max_sync,
                  KEY_lvl_sync_kf_rx_OR_prcs_max_sync, num2);
    }
        break;

    case REG_VSK_creg:                //  0xA0 (0x28)
        ui->checkBoxResetFL->setChecked(val & fl_REG_CREG_com_res);
        ui->checkBoxResetFLT->setChecked(val & fl_REG_CREG_tx_res);
        ui->checkBoxResetFLR->setChecked(val & fl_REG_CREG_rx_res);
        ui->checkBoxStartBC->setChecked(val & fl_REG_CREG_start_bc);
        /// LLL Здесь надо попробовать изменить иконку этого чекбокса
        break;

    case REG_VSK_cr_spi:              //  0xA4 (0x29)
    {
        ui->comboBoxSPI1->setCurrentIndex( val & FL_REG_CR_SPI_spi_en);
        int dep = 0;
        if ((val & FL_REG_CR_SPI_dr8_16_32) == val_REG_CR_SPI_dr_DEP16)
            dep = 1;
        else if ((val & FL_REG_CR_SPI_dr8_16_32) == val_REG_CR_SPI_dr_DEP32)
            dep = 2;
        ui->comboBoxSPI2->setCurrentIndex(dep);
        ui->checkBoxSPI->setChecked(val & fl_REG_CR_SPI_spif);
    }
        break;

    case REG_VSK_dr_spi_msw:          //  0xA8 (0x2A)
        ui->lineEditSPIh->setText(strval);
        break;

    case REG_VSK_dr_spi_lsw:          //  0xAC (0x2B)
        ui->lineEditSPIl->setText(strval);
        break;

    case REG_VSK_time_rsp:            //  0xB4 (0x2D)
        ui->lineEditTime->setText(strval);
        break;

    case REG_VSK_cnt_pct_tx_msw:      //  0xB8 (0x2E)
        ui->lineEditCntTrh->setText(strval);
        break;

    case REG_VSK_cnt_pct_tx_lsw:      //  0xBC (0x2F)
        ui->lineEditCntTrl->setText(strval);
        break;

    case REG_VSK_cnt_pct_rx_msw:      //  0xC0 (0x30)
        ui->lineEditCntReh->setText(strval);
        break;

    case REG_VSK_cnt_pct_rx_lsw:      //  0xC4 (0x31)
        ui->lineEditCntRel->setText(strval);
        break;

    case REG_VSK_lvl_sync_kf_rx_msw:  //  0xC8 (0x32)
        ui->lineEdit_12->setText(strval);
        break;

    case REG_VSK_lvl_sync_kf_rx_lsw:  //  0xCC  (0x33) // чтение/запись
        ui->lineEdit_9->setText(strval);
        break;

    case REG_VSK_lvl_sync_pre_rx_msw: //  0xD0  (0x34) // чтение/запись
        ui->lineEdit_11->setText(strval);
        break;

    case REG_VSK_lvl_sync_pre_rx_lsw: //  0xD4  (0x35) // чтение/запись
        ui->lineEdit_10->setText(strval);
        break;

    case REG_VSK_lvl_qam16:           //  0xD8 (0x36)
        ui->lineEditQ16->setText(strval);
        break;

    case REG_VSK_lvl_qam64_low:       //  0xDC
        ui->lineEditQ64l->setText(strval);
        break;

    case REG_VSK_lvl_qam64_middle:    //   0xE0
        ui->lineEditQ64m->setText(strval);
        break;

    case REG_VSK_lvl_qam64_high:      //  0xE4
        ui->lineEditQ64h->setText(strval);
        break;

    case REG_VSK_amplification_factor: // 0xE8
        ui->checkBoxRX_AMPL0->setChecked(val & fl_REG_AMPL_FACTOR_rx_ampl0);
        ui->checkBoxRX_AMPL1->setChecked(val & fl_REG_AMPL_FACTOR_rx_ampl1);
        ui->checkBoxRX_AMPL2->setChecked(val & fl_REG_AMPL_FACTOR_rx_ampl2);
        ui->checkBoxRX_AMPL3->setChecked(val & fl_REG_AMPL_FACTOR_rx_ampl3);
        ui->checkBoxRX_OE_AMPL->setChecked(val & fl_REG_AMPL_FACTOR_rx_oe_ampl);
        break;

    case REG_VSK_amplitude_signal:    //  0xEC   // только чтение
        ui->lineEditAmplitude->setText(strval);
        break;

    case REG_VSK_g_sp:                //  0xF0
        ui->lineEdit_g_sp->setText(strval);
        break;

    case REG_VSK_g_1_sp_high:         //  0xF4
        ui->lineEdit_g1_sph->setText(strval);
        break;

    case REG_VSK_g_1_sp_low:          //  0xF8
        ui->lineEdit_g1_spl->setText(strval);
        break;

    case REG_VSK_pll_reg:             //  0xFC
        ui->lineEditMult->setText(QString("%1").arg(val & FL_REG_PLL_ns, 4, 16, QChar('0')).toUpper());
        ui->lineEditDiv->setText(QString("%1").arg((val & FL_REG_PLL_ms) >> 6, 4, 16, QChar('0')).toUpper());
        ui->comboBoxFreq->setCurrentIndex((val & fl_REG_PLL_frange) ? 1 : 0);
        ui->comboBoxOff->setCurrentIndex((val & fl_REG_PLL_pd) ? 1 : 0);
        break;

    default:                          // 0x84, 0x98, 0xB0
        break;
    }
}

/// Эта функция инициализирует все признаки и пересчитывает заполнение всех окон, относящихся
///  к данному регистру, учитывая зависимости от других регистров.
void Configuration::updateRegVSK(int num_reg, word16_t regval)
{
    int addr = REGVSKADDRFROM + num_reg*4;

    QString value = (QString("%1").arg(regval, 4, 16, QChar('0'))).toUpper();
    adaptRegVSK(addr, regval, value);

    if (!regVSKReserve(num_reg))
    {
        ui->tableWidgetVSK->item(num_reg, REG_COL_NUM::rcn_Val)->setText(value);
        ui->tableWidgetVSK->item(num_reg, REG_COL_NUM::rcn_Val)->setTextColor(Qt::black);
//        ui->tableWidgetVSK->item(num_reg, REG_COL_NUM::rcn_Name_Check)->setCheckState(Qt::Checked);
    }
}

//////////////////////////////////////////////////////////
/// СЛОТЫ
/*
void Configuration::onEditRegVSK(int row, int col)
{
    if (col == REG_COL_NUM::rcn_Val)
    {
        QTableWidgetItem* curItem = ui->tableWidgetVSK->item(row, col);
        if (curItem)
        {
            qDebug() << "onEditRegVSK";
            return;
            VSKRegView reg(this);
            int row = curItem->row();
            reg.configRegView(registerVSKInfo_read_only(row));
            reg.exec();
        }
    }
}
*/

void Configuration::onEditRegVSK(QPoint point)
{
/*
    if (ui->tableWidgetVSK->columnAt(point.rx()) == REG_COL_NUM::rcn_Val)
    {
        QTableWidgetItem* curItem = ui->tableWidgetVSK->itemAt(point);
        if (curItem)
        {
            qDebug() << "onEditRegVSK";
            return;
            VSKRegView reg(this);
            int row = curItem->row();
       //     reg.configRegView(regVSKType[row], registerVSKInfo_read_only(row));
            reg.configRegView(registerVSKInfo_read_only(row));
            reg.exec();
        }
    }
*/
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
    currentTab = num;
    if(num==1)
        onExpandVSK(ui->comboBoxVSK->currentIndex());
    else if(num==0)
        onExpandNSK(ui->comboBoxNSK->currentIndex());
}

bool Configuration::onPushSave()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить в файл"), "", tr("Файлы конфигурации (*.cnf)\nВсе файлы (*.*)"));

    if (fileName == "")
        return false;

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!file.isOpen())
    {
        QMessageBox::warning(this, tr("Сохранение конфигурации"), tr("Невозможно открыть/создать файл ") + fileName, tr("Вернуться"));
        return false;
    }

    QTextStream out(&file);
    for (int i = 0; i < NUMOFREGNSK; i++)
    {
        ui->tableWidgetNSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        if (ui->tableWidgetNSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
        {
            QString str = ui->tableWidgetNSK->item(i, rcn_Val)->text();
            if (!str.isEmpty())
                out << str;
        }
        out << "\n";
    }
    for (int i = 0; i < NUMOFREGVSK; i++)
    {
        ui->tableWidgetVSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        if (ui->tableWidgetVSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
        {
            QString str = ui->tableWidgetVSK->item(i, rcn_Val)->text();
            if (!str.isEmpty())
                out << str;
        }
        out << "\n";
    }

    file.close();
    path = fileName;
    return true;
}

void Configuration::onPushChoose()
{
    ConfSelect cs(this);
    connect(&cs, SIGNAL(configurate(QString, int*)), this, SLOT(initFrom(QString, int*)));
    cs.exec();
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

void Configuration::onCheckSelectVSK()
{
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

void Configuration::onCheckSelectNSK()
{
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
    if (addr < REG_VSK_BEGIN_ADDR)
    {
        QTableWidgetItem *pItem = ui->tableWidgetNSK->item(addr/4, rcn_Val);
        if (pItem)
            ret = pItem->text().toInt();
    }
    else if (addr <= REG_VSK_END_ADDR)
    {
        QTableWidgetItem *pItem = ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(addr), rcn_Val);
        if (pItem)
            ret = pItem->text().toInt();
    }
    return ret;
}

/// Изменение регистра в форме
bool Configuration::setRegVal(addr_t addr, word16_t val, bool force)
{
    bool bRet = false;
    if (addr < REG_VSK_BEGIN_ADDR)
    {
        QTableWidgetItem *pItem = ui->tableWidgetNSK->item(addr/4, rcn_Val);
        if (pItem == NULL)
                return bRet;
        QString value = QString("%1").arg(val, 4, 16, QChar('0'));
        pItem->setText(value.toUpper());
        pItem->setTextColor(Qt::black);
        pItem = ui->tableWidgetNSK->item(addr/4, rcn_Name_Check);
        if (pItem == NULL)
                return bRet;
        pItem->setCheckState(Qt::Checked);
        return true;
    }
    else if (addr <= REG_VSK_END_ADDR)
    {
        int row = ADDR_2_VSK_ROW(addr);
        QString value = QString("%1").arg(val, 4, 16, QChar('0')).toUpper();
        ui->tableWidgetVSK->item(row, rcn_Val)->setText(value);
        ui->tableWidgetVSK->item(row, rcn_Val)->setTextColor(Qt::black);
        ui->tableWidgetVSK->item(row, rcn_Name_Check)->setCheckState(Qt::Checked);
        if (!force)
            adaptRegVSK(addr, val, value);
        return true;
    }
    return bRet;
}

bool matchSTD0(const QStringList& list, word16_t& param)
{
    bool bRet = false;
    param = 0;

    if (list[1] == "RT")
        param |= fl_REG_CFG_mode_rt_bc * CFG_MODE_RT;
    else if (list[1] == "BC")
        param |= fl_REG_CFG_mode_rt_bc * CFG_MODE_BC;
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
    else if (list[3] == "QAM64")
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
    if (list[0] == "STD")  // шаблон  "STD_BC_NO_QPSK_CODEC_EN2"
    {
        if (matchSTD0(list, param))
            return cdt_Default_S;
    }
    else if (list[0] == "STDU")
    {
        if (matchSTD0(list, param))
            return cdt_Default;
    }
    /// могут быть другие шаблоны

    return ret;
}

bool Configuration::setConfigDefault0(word16_t reg_config, bool all)
{
    setRegVal(REG_VSK_cfg, reg_config);
    setRegVal(REG_VSK_creg, REG_VSK_creg_INIT_RESET);
    setRegVal(REG_VSK_time_rsp, REG_VSK_time_rsp_INIT_DEFAULT);

    setRegVal(REG_VSK_lvl_sync_kf_rx_msw, REG_VSK_lvl_sync_kf_rx_msw_INIT_DEFAULT);
    setRegVal(REG_VSK_lvl_sync_kf_rx_lsw, REG_VSK_lvl_sync_kf_rx_lsw_INIT_DEFAULT);
    setRegVal(REG_VSK_lvl_sync_pre_rx_msw, REG_VSK_lvl_sync_pre_rx_msw_INIT_DEFAULT);
    setRegVal(REG_VSK_lvl_sync_pre_rx_lsw, REG_VSK_lvl_sync_pre_rx_lsw_INIT_DEFAULT);

    int man_type = manType(reg_config);
    if (all || man_type == val_REG_CFG_type_man_QAM16)
        setRegVal(REG_VSK_lvl_qam16, REG_VSK_lvl_qam16_INIT_DEFAULT);
    if (all || man_type == val_REG_CFG_type_man_QAM64)
    {
        setRegVal(REG_VSK_lvl_qam64_low, REG_VSK_lvl_qam64_low_INIT_DEFAULT);
        setRegVal(REG_VSK_lvl_qam64_middle, REG_VSK_lvl_qam64_midle_INIT_DEFAULT);
        setRegVal(REG_VSK_lvl_qam64_high, REG_VSK_lvl_qam64_high_INIT_DEFAULT);
    }
    if (all)
        setRegVal(REG_VSK_rx_cntr, REG_VSK_rx_cntr_INIT_DEFAULT);

    return true;
}

bool Configuration::initDefault(CONFIG_DEFAULT_TYPE key, word16_t param)
{
//    ui->tableWidgetVSK->blockSignals(true);
    switch (key)
    {
    case cdt_Default:
        return setConfigDefault0(param, true);
    case cdt_Default_S:
        return setConfigDefault0(param, false);
    }
    return false;
}

bool Configuration::initFromFile(QString name)
{
    QFile file(name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen())
    {
        QMessageBox::warning(this, tr("Загрузка конфигурации"), tr("Невозможно открыть файл ") + name, tr("Вернуться"));
        return false;
    }

    int cntr = 0;
    word16_t values[NUMOFREGNSK + NUMOFREGVSK];
    bool checked[NUMOFREGNSK + NUMOFREGVSK];
    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        if (cntr >= NUMOFREGNSK + NUMOFREGVSK)
        {
            cntr++;
            break;
        }

        QString str = stream.readLine();
        checked[cntr] = !str.isEmpty();
        if (checked[cntr])
        {
            if (str.length() != 4)
                break;
            values[cntr] = str.toUInt(0,16);
            QString comp = (QString("%1").arg(values[cntr], 4, 16, QChar('0'))).toUpper();
            if (comp != str)
                break;
        }
        cntr++;
    }
    file.close();

    if (cntr != NUMOFREGNSK + NUMOFREGVSK)
    {
        QMessageBox::warning(this, tr("Загрузка конфигурации"), tr("Неправильный формат файла ") + name, tr("Вернуться"));
        return false;
    }

    for (int i=0; i< NUMOFREGNSK + NUMOFREGVSK; i++)
        if (checked[i])
            setRegVal(NUM_REG_2_ADDR(i), values[i]);

    return true;
}

void Configuration::onExit()
{
    for (int i=0; i<ui->tableWidgetNSK->rowCount(); i++)
        ui->tableWidgetNSK->item(i, rcn_Val)->setTextColor(Qt::gray);
    for (int i=0; i<ui->tableWidgetVSK->rowCount(); i++)
        ui->tableWidgetVSK->item(i, rcn_Val)->setTextColor(Qt::gray);
 /*
    lineEditQ16_Edited = lineEditQ64l_Edited = lineEditQ64m_Edited = lineEditQ64h_Edited =
    lineEditTime_Edited = lineEdit_11_Edited = lineEdit_10_Edited = lineEdit_12_Edited =
    lineEdit_9_Edited = lineEditTrCntr_Edited = lineEditCntTrh_Edited = lineEditCntTrl_Edited =
    lineEditCntReh_Edited = lineEditCntRel_Edited = lineEdit_g_sp_Edited = lineEdit_g1_sph_Edited =
    lineEdit_g1_spl_Edited = lineEdit_ID_Edited = lineEditAmplitude_Edited = lineEditMult_Edited =
    lineEditDiv_Edited = lineEditSPIh_Edited = lineEditSPIl_Edited = false;
*/
}

bool Configuration::initFrom(QString name, int* err)
{
    bool bRet = false;
    stopSign = true;
    if (!name.isEmpty())
    {
        ui->tableWidgetNSK->blockSignals(true);
        ui->tableWidgetVSK->blockSignals(true);
        for (int i=0; i<ui->tableWidgetNSK->rowCount(); i++)
        {
            ui->tableWidgetNSK->item(i, rcn_Name_Check)->setCheckState(Qt::Unchecked);
            ui->tableWidgetNSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        }
        for (int i=0; i<ui->tableWidgetVSK->rowCount(); i++)
        {
            ui->tableWidgetVSK->item(i, rcn_Name_Check)->setCheckState(Qt::Unchecked);
            ui->tableWidgetVSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        }
        ui->tableWidgetNSK->blockSignals(false);
        ui->tableWidgetVSK->blockSignals(false);

    word16_t param;
    CONFIG_DEFAULT_TYPE ret = parsConfigKey(name, param);

    if (ret != cdt_Error)
        bRet = initDefault(ret, param);
    else
        bRet = initFromFile(name);
    }
    stopSign = false;
    if (err)
        *err = bRet ? 0 : 2;
    if (bRet)
        path = name;

    return bRet;
}

void onCellChange(CTableEditCol *tab, QCheckBox *box, int row, int col)
{
    //qDebug() << "Cell changed " << row << " " << col;
   if (col == rcn_Name_Check)
   {
       box->blockSignals(true);
       box->setCheckState(Qt::PartiallyChecked);
       box->blockSignals(false);
   }
   else if (col == rcn_Val)
   {
       /*
       tab->blockSignals(true);
       tab->item(row, rcn_Name_Check)->setCheckState(Qt::Checked);
       tab->blockSignals(false);
       if (box->checkState() == Qt::Unchecked)
       {
           box->blockSignals(true);
           box->setCheckState(Qt::PartiallyChecked);
           box->blockSignals(false);
       }
       */
   }
}

void Configuration::onCellChangedVSK(int row, int col)
{
    onCellChange(ui->tableWidgetVSK, ui->checkBoxSelectVSK, row, col);
}

void Configuration::onCellChangedNSK(int row, int col)
{
    onCellChange(ui->tableWidgetNSK, ui->checkBoxSelectNSK, row, col);
}


//////////////////////////////////////////////////////////////
/// СЛОТЫ ДЛЯ ЭЛЕМЕНТОВ СТРАНИЦЫ "Основные настройки"

void Configuration::on_comboBoxManType_currentIndexChanged(int index)
{
    if (currentTab==2 && !stopSign)
    {
        QTableWidgetItem *item = ui->tableWidgetVSK->item(config_NUMREG_cfg-config_NUMREG_BEGIN_VSK, rcn_Val);
        word16_t newval = chgManType(item->text().toInt(0,16), index);
        item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
        item->setTextColor(Qt::black);
        markRegisterEnabled(ui->lineEditQ16, ui->labelQ16, ui->labelMarkQ16, index == val_REG_CFG_type_man_QAM16);
        markRegisterEnabled(ui->lineEditQ64l, ui->labelQ64l, ui->labelMarkQ64l, index == val_REG_CFG_type_man_QAM64);
        markRegisterEnabled(ui->lineEditQ64m, ui->labelQ64m, ui->labelMarkQ64m, index == val_REG_CFG_type_man_QAM64);
        markRegisterEnabled(ui->lineEditQ64h, ui->labelQ64h, ui->labelMarkQ64h, index == val_REG_CFG_type_man_QAM64);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam16)] = (index == val_REG_CFG_type_man_QAM16);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_low)] = (index == val_REG_CFG_type_man_QAM64);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_middle)] = (index == val_REG_CFG_type_man_QAM64);
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_high)] = (index == val_REG_CFG_type_man_QAM64);
        ui->tableWidgetVSK->item(config_NUMREG_cfg-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->setCheckState(Qt::Checked);
    }
}

void Configuration::applyRegFlag(int num_reg, word16_t flag, bool b)
{
    QTableWidgetItem *item = ui->tableWidgetVSK->item(num_reg-config_NUMREG_BEGIN_VSK, rcn_Val);
    word16_t newval = 0;
    if (!item->text().isEmpty())
        newval = (item->text().toInt(0,16)) & ~flag;
    if (b)
        newval |= flag;
    item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
    item->setTextColor(Qt::black);
    ui->tableWidgetVSK->item(num_reg-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->setCheckState(Qt::Checked);
}

void Configuration::on_checkBoxCodec_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
        applyRegFlag(config_NUMREG_cfg, fl_REG_CFG_ena_codec, checked);
}

void Configuration::on_checkBoxEnaAru_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
        applyRegFlag(config_NUMREG_cfg, fl_REG_CFG_ena_aru, checked);
}

void Configuration::on_comboBoxEnaMemVsk_currentIndexChanged(int index)
{
    if (currentTab==2 && !stopSign)
    {
        applyRegFlag(config_NUMREG_cfg, fl_REG_CFG_ena_mem_vsk, index);
        QString title("•	Регистр доступа к внутренней памяти");
        if (index==0)
            title += "   (доступ выключен)";
        ui->labelRam->setText(title);
        camouflage(ui->comboBoxRam, index);
    }
}

void Configuration::on_comboBox_BC_RT_currentIndexChanged(int index)
{
    if (currentTab==2 && !stopSign)
    {
        applyRegFlag(config_NUMREG_cfg, fl_REG_CFG_mode_rt_bc, index);
        ui->checkBoxEnaInt->setText(QString("разрешение выработки прерывания в режиме ") + QString(index ? "ОУ" : "КШ"));
        ui->label_14->setText(QString("флаг прерывания от ") + QString(index ? "ОУ" : "КШ"));
        camouflage(ui->comboBoxRTA, index && ui->checkBoxEnaRtaVSK->isChecked());
    }
}

void Configuration::on_checkBoxEnaInt_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
        applyRegFlag(config_NUMREG_cfg, fl_REG_CFG_en_rt_bc_int, checked);
}

void Configuration::on_checkBoxEnaRtaVSK_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
    {
        applyRegFlag(config_NUMREG_cfg, fl_REG_CFG_rtavsk_ena, checked);
        camouflage(ui->comboBoxRTA, checked && ui->comboBox_BC_RT->currentIndex());
    }
}

void Configuration::on_comboBoxRTA_currentIndexChanged(int index)
{
    if (currentTab==2 && !stopSign)
    {
        QTableWidgetItem *item = ui->tableWidgetVSK->item(config_NUMREG_cfg-config_NUMREG_BEGIN_VSK, rcn_Val);
        word16_t newval = (item->text().toInt(0,16)) & ~FL_REG_CFG_rtavsk;
        newval |= AddrToCfg(index);
        item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
        item->setTextColor(Qt::black);
        ui->tableWidgetVSK->item(config_NUMREG_cfg-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->setCheckState(Qt::Checked);
    }
}
/*
void Configuration:: on_lineEditQ16_editingFinished()
{
    if (currentTab==2 && !stopSign)
        qDebug() << "editingFinished";
}
*/
void Configuration::on_lineEditQ16_textChanged(const QString &arg1)
{
    if (currentTab==2 && !stopSign)
    {
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam16)] = true;
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam16), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam16), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam16), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_checkBoxResetFL_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
        applyRegFlag(config_NUMREG_creg, fl_REG_CREG_com_res, checked);
}

void Configuration::on_checkBoxResetFLT_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
        applyRegFlag(config_NUMREG_creg, fl_REG_CREG_tx_res, checked);
}

void Configuration::on_checkBoxResetFLR_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
        applyRegFlag(config_NUMREG_creg, fl_REG_CREG_rx_res, checked);
}

void Configuration::on_checkBoxStartBC_clicked(bool checked)
{
    if (currentTab==2 && !stopSign)
    {
        if (checked)
            QMessageBox::warning(this, tr("Конфигурация регистров"), tr("Вы включили старт работы КШ"), tr("Выход"));
        applyRegFlag(config_NUMREG_creg, fl_REG_CREG_start_bc, checked);
    }
}


/*
void Configuration::on_lineEditQ64l_editingFinished()
{

}
*/
void Configuration::on_lineEditQ64l_textChanged(const QString &arg1)
{
    if (currentTab==2 && !stopSign)
    {
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_low)] = true;
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_low), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_low), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_low), rcn_Val)->setTextColor(Qt::black);
    }
}
/*
void Configuration::on_lineEditQ64m_editingFinished()
{

}
*/
void Configuration::on_lineEditQ64m_textChanged(const QString &arg1)
{
    if (currentTab==2 && !stopSign)
    {
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_middle)] = true;
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_middle), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_middle), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_middle), rcn_Val)->setTextColor(Qt::black);
    }
}
/*
void Configuration::on_lineEditQ64h_editingFinished()
{
    if (currentTab==2 && !stopSign && !DebugL)
        lineEditQ64h_Edited = true;
}
*/
void Configuration::on_lineEditQ64h_textChanged(const QString &arg1)
{
    if (currentTab==2 && !stopSign)
    {
        regVSKUse[ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_high)] = true;
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_high), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_high), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_qam64_high), rcn_Val)->setTextColor(Qt::black);
    }
}
/*
void Configuration::on_lineEditTime_editingFinished()
{

}
*/
void Configuration::on_lineEditTime_textChanged(const QString &arg1)
{
    if (currentTab==2 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_time_rsp), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_time_rsp), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_time_rsp), rcn_Val)->setTextColor(Qt::black);
    }
}

//////////////////////////////////////////////////////////////
/// СЛОТЫ ДЛЯ ЭЛЕМЕНТОВ СТРАНИЦЫ "Дополнительно"

void Configuration::on_comboBoxLvlPre_activated(int index)
{
    qDebug() << "comboBoxLvlPre_activated";   ///LLL!!!
    if (currentTab==3 && !stopSign)
    {
        qDebug() << "comboBoxLvlPre_activated yes";   ///LLL!!!
        applyRegFlag(config_NUMREG_rx_cntr, fl_REG_RX_CNTR_max_Rn_sync_pre, index);
        switchRegisterAsgmt(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_msw), ui->label_20, ui->label_8, ui->label_9,
                  ui->lineEdit_10, ui->lineEdit_11, ui->labelD0D4RO, TXT_lvl_sync_pre_rx_OR_prs_level_max_rn,
                  KEY_lvl_sync_pre_rx_OR_prs_level_max_rn, index);
    }
}

void Configuration::on_comboBoxLvlCor_activated(int index)
{
    if (currentTab==3 && !stopSign)
    {
        applyRegFlag(config_NUMREG_rx_cntr, fl_REG_RX_CNTR_prcs_max_sync, index);

        switchRegisterAsgmt(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_msw), ui->label_26, ui->label_6, ui->label_7,
                  ui->lineEdit_9, ui->lineEdit_12, ui->labelC8CCRO, TXT_lvl_sync_kf_rx_OR_prcs_max_sync,
                  KEY_lvl_sync_kf_rx_OR_prcs_max_sync, index);
    }
}

void Configuration::on_comboBoxRam_activated(int index)
{
    if (currentTab==3 && !stopSign)
    {
        QTableWidgetItem *item = ui->tableWidgetVSK->item(config_NUMREG_ram_tx_rx-config_NUMREG_BEGIN_VSK, rcn_Val);
        word16_t newval = 0;
        if (index > 0)
            newval = 1 << (index-1);
        item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
        item->setTextColor(Qt::black);
        ui->tableWidgetVSK->item(config_NUMREG_ram_tx_rx-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->setCheckState(Qt::Checked);
    }
}

/*
void Configuration::on_lineEdit_11_editingFinished()
{

}

void Configuration::on_lineEdit_10_editingFinished()
{

}

void Configuration::on_lineEdit_12_editingFinished()
{

}

void Configuration::on_lineEdit_9_editingFinished()
{

}

void Configuration::on_lineEditTrCntr_editingFinished()
{

}

void Configuration::on_lineEditCntTrh_editingFinished()
{

}

void Configuration::on_lineEditCntTrl_editingFinished()
{

}

void Configuration::on_lineEditCntReh_editingFinished()
{

}

void Configuration::on_lineEditCntRel_editingFinished()
{

}

void Configuration::on_lineEdit_g_sp_editingFinished()
{

}

void Configuration::on_lineEdit_g1_sph_editingFinished()
{

}

void Configuration::on_lineEdit_g1_spl_editingFinished()
{

}

void Configuration::on_lineEdit_ID_editingFinished()
{

}

void Configuration::on_lineEditAmplitude_editingFinished()
{

}
*/
void Configuration::on_lineEditAmplitude_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_amplitude_signal), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_amplitude_signal), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_amplitude_signal), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_ID_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_id), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_id), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_id), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_g1_sph_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_1_sp_high), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_1_sp_high), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_1_sp_high), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_g1_spl_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_1_sp_low), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_1_sp_low), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_1_sp_low), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_g_sp_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_sp), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_sp), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_g_sp), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEditCntReh_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_rx_msw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_rx_msw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_rx_msw), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEditCntRel_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_rx_lsw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_rx_lsw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_rx_lsw), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEditCntTrh_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_tx_msw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_tx_msw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_tx_msw), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEditCntTrl_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_tx_lsw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_tx_lsw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_cnt_pct_tx_lsw), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEditTrCntr_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_tx_cntr), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_tx_cntr), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_tx_cntr), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_12_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_msw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_msw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_msw), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_9_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_lsw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_lsw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_kf_rx_lsw), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_11_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_lsw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_lsw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_lsw), rcn_Val)->setTextColor(Qt::black);
    }
}

void Configuration::on_lineEdit_10_textChanged(const QString &arg1)
{
    if (currentTab==3 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_msw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_msw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_lvl_sync_pre_rx_msw), rcn_Val)->setTextColor(Qt::black);
    }
}

//////////////////////////////////////////////////////////////
/// СЛОТЫ ДЛЯ ЭЛЕМЕНТОВ СТРАНИЦЫ "Внешние устройства"

void Configuration::on_comboBoxSPI1_currentIndexChanged(int index)
{
    if (currentTab==4 && !stopSign)
    {
        QTableWidgetItem *item = ui->tableWidgetVSK->item(config_NUMREG_cr_spi-config_NUMREG_BEGIN_VSK, rcn_Val);
        word16_t newval = (item->text().toInt(0,16)) & ~FL_REG_CR_SPI_spi_en;
        newval |= index;
        item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
        item->setTextColor(Qt::black);
        ui->tableWidgetVSK->item(config_NUMREG_cr_spi-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->setCheckState(Qt::Checked);
    }
}

void Configuration::on_comboBoxSPI2_currentIndexChanged(int index)
{
    if (currentTab==4 && !stopSign)
    {
        QTableWidgetItem *item = ui->tableWidgetVSK->item(config_NUMREG_cr_spi-config_NUMREG_BEGIN_VSK, rcn_Val);
        word16_t newval = (item->text().toInt(0,16)) & ~FL_REG_CR_SPI_dr8_16_32;
        newval |= (index << 8);
        item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
        item->setTextColor(Qt::black);
        ui->tableWidgetVSK->item(config_NUMREG_cr_spi-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->setCheckState(Qt::Checked);
    }
}

void Configuration::on_checkBoxSPI_clicked(bool checked)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_cr_spi, fl_REG_CR_SPI_spif, checked);
}

void Configuration::on_checkBoxRX_AMPL0_clicked(bool checked)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_amplification_factor, fl_REG_AMPL_FACTOR_rx_ampl0, checked);
}

void Configuration::on_checkBoxRX_AMPL1_clicked(bool checked)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_amplification_factor, fl_REG_AMPL_FACTOR_rx_ampl1, checked);
}

void Configuration::on_checkBoxRX_AMPL2_clicked(bool checked)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_amplification_factor, fl_REG_AMPL_FACTOR_rx_ampl2, checked);
}

void Configuration::on_checkBoxRX_AMPL3_clicked(bool checked)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_amplification_factor, fl_REG_AMPL_FACTOR_rx_ampl3, checked);
}

void Configuration::on_checkBoxRX_OE_AMPL_clicked(bool checked)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_amplification_factor, fl_REG_AMPL_FACTOR_rx_oe_ampl, checked);
}

void Configuration::on_comboBoxFreq_currentIndexChanged(int index)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_pll_reg, fl_REG_PLL_frange, index);
}

void Configuration::on_comboBoxOff_currentIndexChanged(int index)
{
    if (currentTab==4 && !stopSign)
        applyRegFlag(config_NUMREG_pll_reg, fl_REG_PLL_pd, index);
}

void Configuration::on_lineEditSPIh_textChanged(const QString &arg1)
{
    if (currentTab==4 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_dr_spi_msw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_dr_spi_msw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_dr_spi_msw), rcn_Val)->setTextColor(Qt::black);
    }
}
/*
void Configuration::on_lineEditSPIh_editingFinished()
{

}
*/
void Configuration::on_lineEditSPIl_textChanged(const QString &arg1)
{
    if (currentTab==4 && !stopSign)
    {
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_dr_spi_lsw), rcn_Name_Check)->setCheckState(Qt::Checked);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_dr_spi_lsw), rcn_Val)->setText(arg1);
        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_dr_spi_lsw), rcn_Val)->setTextColor(Qt::black);
    }
}
/*
void Configuration::on_lineEditSPIl_editingFinished()
{

}
*/

void Configuration::on_lineEditMult_textChanged(const QString &arg1)
{
    if (currentTab==4 && !stopSign)
    {
        stopSign = true;
        int arg = 0;
        if (!arg1.isEmpty())
            arg = arg1.toInt(0, 16);
        if (arg > 0x3F)
        {
            arg &= 0x3F;
            ui->lineEditMult->setText(QString::number(arg, 16));
        }

        QTableWidgetItem *item = ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_pll_reg), rcn_Val);
        word16_t newval = 0;
        if (!item->text().isEmpty())
            newval = (item->text().toInt(0,16)) & ~FL_REG_PLL_ns;
        newval += arg;

        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_pll_reg), rcn_Name_Check)->setCheckState(Qt::Checked);
        item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
        item->setTextColor(Qt::black);
        stopSign = false;
    }
}

void Configuration::on_lineEditDiv_textChanged(const QString &arg1)
{
    if (currentTab==4 && !stopSign)
    {
        stopSign = true;
        int arg = arg1.toInt(0, 16);
        if (arg > 0x3F)
        {
            arg &= 0x3F;
            ui->lineEditDiv->setText(QString::number(arg, 16));
        }

        QTableWidgetItem *item = ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_pll_reg), rcn_Val);
        word16_t newval = 0;
        if (!item->text().isEmpty())
            newval = (item->text().toInt(0,16)) & ~FL_REG_PLL_ms;
        newval += (arg << 6);

        ui->tableWidgetVSK->item(ADDR_2_VSK_ROW(REG_VSK_pll_reg), rcn_Name_Check)->setCheckState(Qt::Checked);
        item->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
        item->setTextColor(Qt::black);
        stopSign = false;
    }
}
/*
void Configuration::on_lineEditMult_editingFinished()
{

}

void Configuration::on_lineEditDiv_editingFinished()
{

}
*/

////////////////////////////////////////////////////////////////////////
/// ВНЕШНИЕ ФУНКЦИИ

int Configuration::getConfigReg() const
{
    return ui->tableWidgetVSK->item(config_NUMREG_cfg-config_NUMREG_BEGIN_VSK, rcn_Val)->text().toInt(0,16);
}

void Configuration::setChecked(int num, bool ch)
{
    if (num < config_NUMREG_BEGIN_VSK)
        ui->tableWidgetNSK->item(num, rcn_Name_Check)->setCheckState(ch ? Qt::Checked : Qt::Unchecked);
    else
        ui->tableWidgetVSK->item(num-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->setCheckState(ch ? Qt::Checked : Qt::Unchecked);
    /*
    в else:
    {
        QTableWidgetItem *it = ui->tableWidgetVSK->item(num-config_NUMREG_BEGIN_VSK, rcn_Name_Check);
        Qt::CheckState chs = ch ? Qt::Checked : Qt::Unchecked;
        qDebug() << "before chs =  " << chs;
        qDebug() << "before setCheckState  ";
        it->setCheckState(chs);
        qDebug() << "after setCheckState(false)";
    }*/
}

bool Configuration::disInt()
{
    if (ui->tableWidgetVSK->item(config_NUMREG_cfg-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->checkState() == Qt::Checked)
    {
        return (ui->tableWidgetVSK->item(config_NUMREG_cfg-config_NUMREG_BEGIN_VSK, rcn_Val)->text().toInt(0,16) & fl_REG_CFG_en_rt_bc_int) == 0;
    }
    return false;
}

void Configuration::setWritten(int addr, int val)
{
    setRegVal(addr, val);
    //markWritten(addr);
}

void Configuration::markWritten(int addr)
{
    if (addr < REG_VSK_BEGIN_ADDR)
    {
        int row = addr/4;
        if (!registerNSKInfo_read_only(row))
            ui->tableWidgetNSK->item(row, rcn_Val)->setTextColor(Qt::blue);
    }
    else if (addr <= REG_VSK_END_ADDR)
    {
        int row = ADDR_2_VSK_ROW(addr);
        if (!registerVSKInfo_read_only(row))
            ui->tableWidgetVSK->item(row, rcn_Val)->setTextColor(Qt::blue);
    }
}
