#include "codeselect.h"
#include "ui_codeselect.h"
#include "extern_hsc.h"

CodeSelect::CodeSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodeSelect)
{
    ui->setupUi(this);
//    connect(this, SIGNAL(configurate(QString, int*)), parent, SLOT(initFrom(QString, int*)));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    connect(this,SIGNAL(finished(int)),this,SLOT(extractCodes(int)));
}

CodeSelect::~CodeSelect()
{
    delete ui;
}

void CodeSelect::extractCodes(int)
{
    QString codes;
    int num = 0;
    for (int row = 0; row<8; row++)
        for (int col = 0; col<16; col++)
        {
            QChar ch = ui->tableWidget->cellVal(row, col);
            num += ch.digitValue();
            if (row!=7 || col!=15)
            {
                codes.append(ch);
                codes.append(',');
            }
            else
                codes.append(num==0 ? '1' : ch);
        }
    emit applyCodes(codes);
}

void CodeSelect::on_pushButtonInvV0_clicked()
{
    invertCol(0);
}
void CodeSelect::on_pushButtonInvV1_clicked()
{
    invertCol(1);
}
void CodeSelect::on_pushButtonInvV2_clicked()
{
    invertCol(2);
}
void CodeSelect::on_pushButtonInvV3_clicked()
{
    invertCol(3);
}
void CodeSelect::on_pushButtonInvV4_clicked()
{
    invertCol(4);
}
void CodeSelect::on_pushButtonInvV5_clicked()
{
    invertCol(5);
}
void CodeSelect::on_pushButtonInvV6_clicked()
{
    invertCol(6);
}
void CodeSelect::on_pushButtonInvV7_clicked()
{
    invertCol(7);
}
void CodeSelect::on_pushButtonInvV8_clicked()
{
    invertCol(8);
}
void CodeSelect::on_pushButtonInvV9_clicked()
{
    invertCol(9);
}
void CodeSelect::on_pushButtonInvV10_clicked()
{
    invertCol(10);
}
void CodeSelect::on_pushButtonInvV11_clicked()
{
    invertCol(11);
}
void CodeSelect::on_pushButtonInvV12_clicked()
{
    invertCol(12);
}
void CodeSelect::on_pushButtonInvV13_clicked()
{
    invertCol(13);
}
void CodeSelect::on_pushButtonInvV14_clicked()
{
    invertCol(14);
}
void CodeSelect::on_pushButtonInvV15_clicked()
{
    invertCol(15);
}

void CodeSelect::on_pushButtonInvH0_clicked()
{
    invertRow(0);
}
void CodeSelect::on_pushButtonInvH1_clicked()
{
    invertRow(1);
}
void CodeSelect::on_pushButtonInvH2_clicked()
{
    invertRow(2);
}
void CodeSelect::on_pushButtonInvH3_clicked()
{
    invertRow(3);
}
void CodeSelect::on_pushButtonInvH4_clicked()
{
    invertRow(4);
}
void CodeSelect::on_pushButtonInvH5_clicked()
{
    invertRow(5);
}
void CodeSelect::on_pushButtonInvH6_clicked()
{
    invertRow(6);
}
void CodeSelect::on_pushButtonInvH7_clicked()
{
    invertRow(7);
}

void CodeSelect::on_pushButtonDelV0_clicked()
{
    excludeCol(0);
}
void CodeSelect::on_pushButtonDelV1_clicked()
{
    excludeCol(1);
}
void CodeSelect::on_pushButtonDelV2_clicked()
{
    excludeCol(2);
}
void CodeSelect::on_pushButtonDelV3_clicked()
{
    excludeCol(3);
}
void CodeSelect::on_pushButtonDelV4_clicked()
{
    excludeCol(4);
}
void CodeSelect::on_pushButtonDelV5_clicked()
{
    excludeCol(5);
}
void CodeSelect::on_pushButtonDelV6_clicked()
{
    excludeCol(6);
}
void CodeSelect::on_pushButtonDelV7_clicked()
{
    excludeCol(7);
}
void CodeSelect::on_pushButtonDelV8_clicked()
{
    excludeCol(8);
}
void CodeSelect::on_pushButtonDelV9_clicked()
{
    excludeCol(9);
}
void CodeSelect::on_pushButtonDelV10_clicked()
{
    excludeCol(10);
}
void CodeSelect::on_pushButtonDelV11_clicked()
{
    excludeCol(11);
}
void CodeSelect::on_pushButtonDelV12_clicked()
{
    excludeCol(12);
}
void CodeSelect::on_pushButtonDelV13_clicked()
{
    excludeCol(13);
}
void CodeSelect::on_pushButtonDelV14_clicked()
{
    excludeCol(14);
}
void CodeSelect::on_pushButtonDelV15_clicked()
{
    excludeCol(15);
}

void CodeSelect::on_pushButtonDelH0_clicked()
{
    excludeRow(0);
}
void CodeSelect::on_pushButtonDelH1_clicked()
{
    excludeRow(1);
}
void CodeSelect::on_pushButtonDelH2_clicked()
{
    excludeRow(2);
}
void CodeSelect::on_pushButtonDelH3_clicked()
{
    excludeRow(3);
}
void CodeSelect::on_pushButtonDelH4_clicked()
{
    excludeRow(4);
}
void CodeSelect::on_pushButtonDelH5_clicked()
{
    excludeRow(5);
}
void CodeSelect::on_pushButtonDelH6_clicked()
{
    excludeRow(6);
}
void CodeSelect::on_pushButtonDelH7_clicked()
{
    excludeRow(7);
}

void CodeSelect::on_pushButtonYesV0_clicked()
{
    includeCol(0);
}
void CodeSelect::on_pushButtonYesV1_clicked()
{
    includeCol(1);
}
void CodeSelect::on_pushButtonYesV2_clicked()
{
    includeCol(2);
}
void CodeSelect::on_pushButtonYesV3_clicked()
{
    includeCol(3);
}
void CodeSelect::on_pushButtonYesV4_clicked()
{
    includeCol(4);
}
void CodeSelect::on_pushButtonYesV5_clicked()
{
    includeCol(5);
}
void CodeSelect::on_pushButtonYesV6_clicked()
{
    includeCol(6);
}
void CodeSelect::on_pushButtonYesV7_clicked()
{
    includeCol(7);
}
void CodeSelect::on_pushButtonYesV8_clicked()
{
    includeCol(8);
}
void CodeSelect::on_pushButtonYesV9_clicked()
{
    includeCol(9);
}
void CodeSelect::on_pushButtonYesV10_clicked()
{
    includeCol(10);
}
void CodeSelect::on_pushButtonYesV11_clicked()
{
    includeCol(11);
}
void CodeSelect::on_pushButtonYesV12_clicked()
{
    includeCol(12);
}
void CodeSelect::on_pushButtonYesV13_clicked()
{
    includeCol(13);
}
void CodeSelect::on_pushButtonYesV14_clicked()
{
    includeCol(14);
}
void CodeSelect::on_pushButtonYesV15_clicked()
{
    includeCol(15);
}

void CodeSelect::on_pushButtonYesH0_clicked()
{
    includeRow(0);
}
void CodeSelect::on_pushButtonYesH1_clicked()
{
    includeRow(1);
}
void CodeSelect::on_pushButtonYesH2_clicked()
{
    includeRow(2);
}
void CodeSelect::on_pushButtonYesH3_clicked()
{
    includeRow(3);
}
void CodeSelect::on_pushButtonYesH4_clicked()
{
    includeRow(4);
}
void CodeSelect::on_pushButtonYesH5_clicked()
{
    includeRow(5);
}
void CodeSelect::on_pushButtonYesH6_clicked()
{
    includeRow(6);
}
void CodeSelect::on_pushButtonYesH7_clicked()
{
    includeRow(7);
}

void CodeSelect::includeRow(int row)
{
    for (int i=0; i<16; i++)
        setCell(row, i, true);
}

void CodeSelect::excludeRow(int row)
{
    for (int i=0; i<16; i++)
        setCell(row, i, false);
}

void CodeSelect::invertRow(int row)
{
    for (int i=0; i<16; i++)
        ui->tableWidget->invertItem(ui->tableWidget->item(row, i));
}

void CodeSelect::includeCol(int col)
{
    for (int i=0; i<8; i++)
        setCell(i, col, true);
}

void CodeSelect::excludeCol(int col)
{
    for (int i=0; i<8; i++)
        setCell(i, col, false);
}

void CodeSelect::invertCol(int col)
{
    for (int i=0; i<8; i++)
        ui->tableWidget->invertItem(ui->tableWidget->item(i, col));
}

void CodeSelect::labelToCodes(QString str)
{
    int i=0;
    for (; i<=MAX_COMMAND_CODE && i*2<str.length(); i++)
    {
        setCell(i/16, i%16, str.at(i*2) == '1');
    }
    for (; i<=MAX_COMMAND_CODE; i++)
    {
        setCell(i/16, i%16, false);
    }
}

void CodeSelect::setCell(int row, int col, bool b)
{
    ui->tableWidget->item(row, col)->setBackground(b ? CTW8x8C_ColorTrue : CTW8x8C_ColorFalse);
}
