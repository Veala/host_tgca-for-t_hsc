#include "monitor.h"
#include "ui_monitor.h"
#include "configuration.h"
#include "ctableeditcol.h"
//#include "tests/abstracttest.h"
#include "confselect.h"
#include <QMessageBox>

extern bool regVSKUseGlobal[NUMOFREGVSK];

Monitor::Monitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint);
    //setModal(true);

    ui->pushButtonRead->setEnabled(false);
    ui->pushButtonWrite->setEnabled(false);
    connection[0] = connection[1] = false;
}

Monitor::~Monitor()
{
    delete ui;
}

static void initTab(CTableEditCol* tab)
{
    for (int row=0; row < tab->rowCount(); row++)
    {
        for (int col=0; col < REG_COL_NUM::rcn_End+2; col++)
        {
            QTableWidgetItem *pItem = tab->item(row, col);
            if (pItem == NULL)
            {
                pItem = new QTableWidgetItem();
                if (pItem == NULL)
                    throw("Внутренняя ошибка в начальной настройке окна Monitor");
                tab->setItem(row, col, pItem);
            }
        }
        tab->item(row, REG_COL_NUM::rcn_Name_Check)->setCheckState(Qt::Unchecked);
    }
    tab->setColumnWidth(REG_COL_NUM::rcn_Addr,40);
    tab->setColumnWidth(REG_COL_NUM::rcn_Name_Check,130);
    tab->setColumnWidth(REG_COL_NUM::rcn_Descr,290);
    tab->setColumnWidth(REG_COL_NUM::rcn_Val,103);
    tab->setColumnWidth(REG_COL_NUM::rcn_Val+1,103);
}

static void fillTab(CTableEditCol* tab, Device *dev)
{
    CTableEditCol* tabDev = dev->configuration.getTableVSK();
    for (int row=0; row < tabDev->rowCount(); row++)
    {
        QTableWidgetItem *pItem = tabDev->item(row, REG_COL_NUM::rcn_Val);
        if (pItem != NULL && !pItem->text().isEmpty())
        {
            tab->item(row, REG_COL_NUM::rcn_Val)->setText(pItem->text());
            tab->item(row, REG_COL_NUM::rcn_Val)->setTextColor(Qt::black);
        }
        tab->item(row, REG_COL_NUM::rcn_Name_Check)->setCheckState(tabDev->item(row, REG_COL_NUM::rcn_Name_Check)->checkState());
    }
}

void Monitor::setDevices(Device *dev1, Device *dev2)
{
    device1 = dev1;
    device2 = dev2;
    if (dev2)
    {
        QObject::connect(ui->checkBoxSelect2, SIGNAL(stateChanged(int)), this, SLOT(onCheckSelect2()));
        QObject::connect(ui->comboBox2, SIGNAL(activated(int)), this, SLOT(onExpand2(int)));
        initTab(ui->tableWidgetVSK2);
        fillTab(ui->tableWidgetVSK2, dev2);
        QObject::connect(ui->tableWidgetVSK2, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged2(int,int)));
    }
    else
    {
        ui->tabWidget->removeTab(1);
        ui->checkBoxSelect2->setEnabled(false);
        ui->checkBoxSelect2->setVisible(false);
        ui->comboBox2->setEnabled(false);
        ui->comboBox2->setVisible(false);
        if (dev1 == 0)
        {
            ui->labelDevice->setText(tr("Не задано ни одного устройства"));
            ui->labelDevice->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));

            ui->pushButtonApply->setEnabled(false);
            ui->pushButtonChoose->setEnabled(false);
        }
    }
    if (dev1)
    {
        QObject::connect(ui->checkBoxSelect1, SIGNAL(stateChanged(int)), this, SLOT(onCheckSelect1()));
        QObject::connect(ui->comboBox1, SIGNAL(activated(int)), this, SLOT(onExpand1(int)));
        initTab(ui->tableWidgetVSK1);
        fillTab(ui->tableWidgetVSK1, dev1);
        connect(ui->tableWidgetVSK1, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged1(int,int)));

        if (dev2)
        {
            QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onChangeTab(int)));
            ui->tabWidget->setCurrentIndex(0);
            ui->comboBox2->setVisible(false);
            ui->comboBox2->setEnabled(false);
            ui->checkBoxSelect2->setVisible(false);
            ui->checkBoxSelect2->setEnabled(false);

        }
        connect(ui->tableWidgetVSK1, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged1(int,int)));
    }
    else
    {
        ui->tabWidget->removeTab(0);
        ui->checkBoxSelect1->setEnabled(false);
        ui->checkBoxSelect1->setVisible(false);
        ui->comboBox1->setEnabled(false);
        ui->comboBox1->setVisible(false);
    }
  /*  for (int row=0; row < NUMOFREGVSK; row++)
    {
        regVSKRO[row] = false;
        regVSKUse[row] = regVSKUseGlobal[row];
    }
    regVSKRO[2] = regVSKRO[3] = regVSKRO[27] = true;
    regVSKRes[1] = regVSKRes[6] = regVSKRes[12] = true;*/
    ui->comboBox1->setCurrentIndex(0);
    ui->comboBox2->setCurrentIndex(0);
}

void Monitor::onChangeTab(int num)
{
    ui->comboBox1->setVisible(num == 0);
    ui->comboBox1->setEnabled(num == 0);
    ui->comboBox2->setVisible(num == 1);
    ui->comboBox2->setEnabled(num == 1);
    ui->checkBoxSelect1->setVisible(num == 0);
    ui->checkBoxSelect1->setEnabled(num == 0);
    ui->checkBoxSelect2->setVisible(num == 1);
    ui->checkBoxSelect2->setEnabled(num == 1);
    if(num==1)
        onExpand2(ui->comboBox2->currentIndex());
    else if(num==0)
        onExpand1(ui->comboBox1->currentIndex());

    ui->pushButtonRead->setEnabled(connection[num]);
    ui->pushButtonWrite->setEnabled(connection[num]);
}

void Monitor::onPushExit()
{
    reject();
}

void Monitor::onConnect(int num)
{
    connection[num] = true;
    if (ui->tabWidget->currentIndex() == num)
    {
        ui->pushButtonRead->setEnabled(true);
        ui->pushButtonWrite->setEnabled(true);
    }
}

bool Monitor::onPushRead()
{
    bool bRet = false;
    setDisabled(true);
    if (*signalToMonitorTest != wait)
    {
        qDebug() << "Something is going wrong";
    }
    else
    {
        arr->clear();
        *arr = getRegistersToRead();
        if (arr->size() > 0)
        {
            *signalToMonitorTest = isSecondPage() ? readRT : readBC;
            bRet = true;
        }
    }
    if (!bRet)
        setEnabled(true);
    return bRet;
}

bool Monitor::onPushWrite()
{
    bool bRet = false;
    setDisabled(true);
    if (*signalToMonitorTest != wait)
    {
        qDebug() << "Something is going wrong";
    }
    else
    {
        arr->clear();
        *arr = getRegistersToWrite();
        if (arr->size() > 0)
        {
            *signalToMonitorTest = isSecondPage() ? writeRT : writeBC;
            bRet = true;
        }
    }
    if (!bRet)
        setEnabled(true);
    return bRet;
}

bool Monitor::isSecondPage()
{
    return (ui->tabWidget->currentIndex() != 0);
}

QByteArray Monitor::getRegistersToWrite()
{
    QByteArray array;
    CTableEditCol *tab = isSecondPage() ? ui->tableWidgetVSK2 : ui->tableWidgetVSK1;

    for (int i=0; i<tab->rowCount(); i++)
    {
        tab->item(i, rcn_Val)->setTextColor(Qt::gray);
        if (tab->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
        {
            int addr = ROW_REG_VSK_2_ADDR(i);
            QString strval = tab->item(i, rcn_Val)->text();
            int val = strval.isEmpty() ? 0 : strval.toInt(0,16);
            array.append((char*)&addr, 4);
            array.append((char*)&val, 4);
        }
    }
    return array;
}

QByteArray Monitor::getRegistersToRead()
{
    QByteArray array;
    CTableEditCol *tab = isSecondPage() ? ui->tableWidgetVSK2 : ui->tableWidgetVSK1;

    for (int i=0; i<tab->rowCount(); i++)
    {
        tab->item(i, rcn_Val+1)->setTextColor(Qt::gray);
        tab->item(i, rcn_Val+2)->setTextColor(Qt::gray);
        if (tab->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
        {
            int addr = ROW_REG_VSK_2_ADDR(i);
            array.append((char*)&addr, 4);
        }
    }
    return array;
}

static void doneWriteReg(CTableEditCol *tab)
{
    for (int i=0; i<tab->rowCount(); i++)
    {
        if (tab->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
            tab->item(i, rcn_Val)->setTextColor(Qt::blue);
    }
}

void Monitor::doneWriteBC()
{
    doneWriteReg(ui->tableWidgetVSK1);
    setEnabled(true);
}

void Monitor::doneWriteRT()
{
    doneWriteReg(ui->tableWidgetVSK2);
    setEnabled(true);
}

void Monitor::doneRead(CTableEditCol *tab, QByteArray readArray)
{
    if(readArray.size() != arr->size())
    {
        qDebug() << "Unknown error different arrays";
    }
    else
    {
        for (int i=0; i<tab->rowCount(); i++)
        {
            tab->item(i, rcn_Val+2)->setTextColor(Qt::gray);
        }
        for (int i=0; i<readArray.size(); i+=4)
        {
            int addr = *(int*)(arr->data()+i);
            int newval = *(int*)(readArray.data()+i);
            int row = ADDR_2_VSK_ROW(addr);
            QString newstrval = QString::number(newval, 16);
            QString oldstrval = tab->item(row, rcn_Val+2)->text();
            tab->item(row, rcn_Val+2)->setText(newstrval);
            tab->item(row, rcn_Val+2)->setTextColor(Qt::blue);
            if (!oldstrval.isEmpty())
            {
                tab->item(row, rcn_Val+1)->setText(oldstrval);
                if (newstrval != oldstrval)
                    tab->item(row, rcn_Val+2)->setTextColor(Qt::red);
            }
        }
    }
    setEnabled(true);
}

void Monitor::doneReadBC(QByteArray readArray)
{
    doneRead(ui->tableWidgetVSK1, readArray);
}

void Monitor::doneReadRT(QByteArray readArray)
{
    doneRead(ui->tableWidgetVSK2, readArray);
}

void Monitor::forceExit()
{
    qDebug() << "Monitor finished";
    *force_exit = true;
}

void Monitor::onPushLoad()
{
    ConfSelect cs(this);
    connect(&cs, SIGNAL(configurate(QString, int*)), this, SLOT(initFrom(QString, int*)));
    cs.exec();
}

bool Monitor::initFrom(QString name, int* err)
{
    bool bRet = false;
    CTableEditCol *tab = isSecondPage() ? ui->tableWidgetVSK2 : ui->tableWidgetVSK1;
    if (!name.isEmpty())
    {
        tab->blockSignals(true);
        for (int i=0; i<tab->rowCount(); i++)
        {
            tab->item(i, rcn_Name_Check)->setCheckState(Qt::Unchecked);
            tab->item(i, rcn_Val)->setTextColor(Qt::gray);
        }
        tab->blockSignals(false);

    word16_t param;
    CONFIG_DEFAULT_TYPE ret = parsConfigKey(name, param);

    if (ret != cdt_Error)
        bRet = initDefault(ret, param, tab);
    else
        bRet = initFromFile(name, tab);
    }
    err = 0;
    return bRet;
}

static void setRegVal(CTableEditCol *tab, int numreg, int val)
{
    int row = numreg - NUMOFREGNSK;
    QString value = QString("%1").arg(val, 4, 16, QChar('0')).toUpper();
    tab->item(row, rcn_Val)->setText(value);
    tab->item(row, rcn_Val)->setTextColor(Qt::black);
    tab->item(row, rcn_Name_Check)->setCheckState(Qt::Checked);
}

bool setConfigDefault0(CTableEditCol *tab, word16_t reg_config, bool all)
{
    setRegVal(tab, config_NUMREG_cfg, reg_config);
    setRegVal(tab, config_NUMREG_creg, REG_VSK_creg_INIT_RESET);
    setRegVal(tab, config_NUMREG_time_rsp, REG_VSK_time_rsp_INIT_DEFAULT);

    setRegVal(tab, config_NUMREG_lvl_sync_kf_rx_msw, REG_VSK_lvl_sync_kf_rx_msw_INIT_DEFAULT);
    setRegVal(tab, config_NUMREG_lvl_sync_kf_rx_lsw, REG_VSK_lvl_sync_kf_rx_lsw_INIT_DEFAULT);
    setRegVal(tab, config_NUMREG_lvl_sync_pre_rx_msw, REG_VSK_lvl_sync_pre_rx_msw_INIT_DEFAULT);
    setRegVal(tab, config_NUMREG_lvl_sync_pre_rx_lsw, REG_VSK_lvl_sync_pre_rx_lsw_INIT_DEFAULT);

    int man_type = manType(reg_config);
    if (all || man_type == val_REG_CFG_type_man_QAM16)
        setRegVal(tab, config_NUMREG_lvl_qam16, REG_VSK_lvl_qam16_INIT_DEFAULT);
    if (all || man_type == val_REG_CFG_type_man_QAM64)
    {
        setRegVal(tab, config_NUMREG_lvl_qam64_low, REG_VSK_lvl_qam64_low_INIT_DEFAULT);
        setRegVal(tab, config_NUMREG_lvl_qam64_middle, REG_VSK_lvl_qam64_midle_INIT_DEFAULT);
        setRegVal(tab, config_NUMREG_lvl_qam64_high, REG_VSK_lvl_qam64_high_INIT_DEFAULT);
    }
    if (all)
        setRegVal(tab, REG_VSK_rx_cntr, REG_VSK_rx_cntr_INIT_DEFAULT);

    return true;
}

bool Monitor::initDefault(CONFIG_DEFAULT_TYPE key, word16_t param, CTableEditCol *tab)
{
    if (key == cdt_Default)
        return setConfigDefault0(tab, param, true);
    else if (key == cdt_Default_S)
        return setConfigDefault0(tab, param, false);
    else
        return false;
}

bool Monitor::initFromFile(QString name, CTableEditCol *tab)
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

    for (int i=NUMOFREGNSK; i< NUMOFREGNSK + NUMOFREGVSK; i++)
        if (checked[i])
            setRegVal(tab, i, values[i]);

    return true;
}

static void onExpand(CTableEditCol *tab, int mode)
{
    for (int i=0; i<tab->rowCount(); ++i)
    {
        bool b = false;
        if (mode == 1)
            b = (tab->item(i, REG_COL_NUM::rcn_Name_Check)->checkState() == Qt::Unchecked);
        else if (mode == 2)
            b = !regVSKUseGlobal[i];
        tab->setRowHidden(i, b);
    }
}

void Monitor::onExpand1(int mode)
{
    onExpand(ui->tableWidgetVSK1, mode);
}

void Monitor::onExpand2(int mode)
{
    onExpand(ui->tableWidgetVSK2, mode);
}

static void onCheckSelect(CTableEditCol *tab, QCheckBox *box)
{
    if (box->checkState() == Qt::PartiallyChecked)
    {
        box->blockSignals(true);
        box->setCheckState(Qt::Checked);
        box->blockSignals(false);
    }
    Qt::CheckState b = box->checkState();
    tab->blockSignals(true);
    for (int i=0; i<tab->rowCount();i++)
        tab->item(i, rcn_Name_Check)->setCheckState(b);
    tab->blockSignals(false);
}

void Monitor::onCheckSelect1()
{
    onCheckSelect(ui->tableWidgetVSK1, ui->checkBoxSelect1);
}

void Monitor::onCheckSelect2()
{
    onCheckSelect(ui->tableWidgetVSK2, ui->checkBoxSelect2);
}





void Monitor::onCellChanged1(int, int)
{

}

void Monitor::onCellChanged2(int, int)
{

}

bool Monitor::onPushStore()
{
    setDisabled(true);
    return false;
}

