#include "monitor.h"
#include "ui_monitor.h"
#include "configuration.h"
#include "ctableeditcol.h"
//#include "tests/abstracttest.h"

void initVSK(CTableEditCol* tab)
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
                    throw("Внутренняя ошибка в конструкторе Monitor");
                tab->setItem(row, col, pItem);
            }
        }
        tab->item(row, REG_COL_NUM::rcn_Name_Check)->setCheckState(Qt::Unchecked);
    }
    tab->setColumnWidth(REG_COL_NUM::rcn_Addr,55);
    tab->setColumnWidth(REG_COL_NUM::rcn_Name_Check,120);
    tab->setColumnWidth(REG_COL_NUM::rcn_Descr,300);
    tab->setColumnWidth(REG_COL_NUM::rcn_Val,100);
    tab->setColumnWidth(REG_COL_NUM::rcn_Val+1,100);
}

Monitor::Monitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint);
    //setModal(true);
}

Monitor::~Monitor()
{
    delete ui;
}

static void cmdHead(QByteArray* head, int cmd, int dsz)
{
    head->append((char*)&cmd, 4);
    head->append((char*)&dsz, 4);
}

void Monitor::setDevice(Device *dev1, Device *dev2)
{
    device1 = dev1;
    device2 = dev2;
    if (dev2)
    {
        QObject::connect(ui->checkBoxSelect2, SIGNAL(stateChanged(int)), this, SLOT(onCheckSelect2()));
        QObject::connect(ui->comboBox2, SIGNAL(activated(int)), this, SLOT(onExpand2(int)));
        initVSK(ui->tableWidgetVSK2);
        connect(ui->tableWidgetVSK2, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged2(int,int)));
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
            ui->pushButtonRead->setEnabled(false);
            ui->pushButtonWrite->setEnabled(false);
        }
    }
    if (dev1)
    {
        QObject::connect(ui->checkBoxSelect1, SIGNAL(stateChanged(int)), this, SLOT(onCheckSelect1()));
        QObject::connect(ui->comboBox1, SIGNAL(activated(int)), this, SLOT(onExpand1(int)));
        initVSK(ui->tableWidgetVSK1);

        if (dev2)
        {
            QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onChangeTab(int)));
            ui->tabWidget->setCurrentIndex(0);
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

void Monitor::onChangeTab(int)
{

}

bool Monitor::onPushRead()
{
    return false;
}

bool Monitor::onPushWrite()
{
    int addr = 0x194;
    int val = 1;

    cmdHead(arr, 1, 8);
    arr->append((char*)&addr, 4);
    arr->append((char*)&val, 4);

    *writeBC = 1;
    return true;
}

void Monitor::onPushExit()
{
    //emit monitorClosed();
    //hide();
    reject();
}

void Monitor::onSignalError(int err)
{
    //reject();
}

bool Monitor::onPushStore()
{
    return false;
}

bool Monitor::onPushLoad()
{
    return false;
}

void Monitor::onExpand1(int)
{

}

void Monitor::onExpand2(int)
{

}

void Monitor::onCheckSelect1()
{

}

void Monitor::onCheckSelect2()
{

}

void Monitor::onCellChanged1(int, int)
{

}

void Monitor::onCellChanged2(int, int)
{

}

void Monitor::onExit(int)
{
}

void Monitor::doneWriteBC()
{
    for (int i=0; i<ui->tableWidgetVSK1->rowCount(); i++)
    {
        if (ui->tableWidgetVSK1->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
            ui->tableWidgetVSK1->item(i, rcn_Val)->setTextColor(Qt::blue);
    }
}

void Monitor::forceExit()
{
    qDebug() << "monitor finished";
    *force_exit = true;
}
