#ifndef CODESELECT_H
#define CODESELECT_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class CodeSelect;
}

class CodeSelect : public QDialog
{
    Q_OBJECT

public:
    explicit CodeSelect(QWidget *parent = 0);
    ~CodeSelect();
    void labelToCodes(QString str);

private:
    Ui::CodeSelect *ui;

    void includeRow(int row);
    void excludeRow(int row);
    void invertRow(int row);
    void includeCol(int col);
    void excludeCol(int col);
    void invertCol(int col);
    void setCell(int row, int col, bool b);

private slots:
    void on_pushButtonInvH0_clicked();
    void on_pushButtonInvH1_clicked();
    void on_pushButtonInvH2_clicked();
    void on_pushButtonInvH3_clicked();
    void on_pushButtonInvH4_clicked();
    void on_pushButtonInvH5_clicked();
    void on_pushButtonInvH6_clicked();
    void on_pushButtonInvH7_clicked();
    void on_pushButtonInvV0_clicked();
    void on_pushButtonInvV1_clicked();
    void on_pushButtonInvV2_clicked();
    void on_pushButtonInvV3_clicked();
    void on_pushButtonInvV4_clicked();
    void on_pushButtonInvV5_clicked();
    void on_pushButtonInvV6_clicked();
    void on_pushButtonInvV7_clicked();
    void on_pushButtonInvV8_clicked();
    void on_pushButtonInvV9_clicked();
    void on_pushButtonInvV10_clicked();
    void on_pushButtonInvV11_clicked();
    void on_pushButtonInvV12_clicked();
    void on_pushButtonInvV13_clicked();
    void on_pushButtonInvV14_clicked();
    void on_pushButtonInvV15_clicked();

    void on_pushButtonDelH0_clicked();
    void on_pushButtonDelH1_clicked();
    void on_pushButtonDelH2_clicked();
    void on_pushButtonDelH3_clicked();
    void on_pushButtonDelH4_clicked();
    void on_pushButtonDelH5_clicked();
    void on_pushButtonDelH6_clicked();
    void on_pushButtonDelH7_clicked();
    void on_pushButtonDelV0_clicked();
    void on_pushButtonDelV1_clicked();
    void on_pushButtonDelV2_clicked();
    void on_pushButtonDelV3_clicked();
    void on_pushButtonDelV4_clicked();
    void on_pushButtonDelV5_clicked();
    void on_pushButtonDelV6_clicked();
    void on_pushButtonDelV7_clicked();
    void on_pushButtonDelV8_clicked();
    void on_pushButtonDelV9_clicked();
    void on_pushButtonDelV10_clicked();
    void on_pushButtonDelV11_clicked();
    void on_pushButtonDelV12_clicked();
    void on_pushButtonDelV13_clicked();
    void on_pushButtonDelV14_clicked();
    void on_pushButtonDelV15_clicked();

    void on_pushButtonYesH0_clicked();
    void on_pushButtonYesH1_clicked();
    void on_pushButtonYesH2_clicked();
    void on_pushButtonYesH3_clicked();
    void on_pushButtonYesH4_clicked();
    void on_pushButtonYesH5_clicked();
    void on_pushButtonYesH6_clicked();
    void on_pushButtonYesH7_clicked();
    void on_pushButtonYesV0_clicked();
    void on_pushButtonYesV1_clicked();
    void on_pushButtonYesV2_clicked();
    void on_pushButtonYesV3_clicked();
    void on_pushButtonYesV4_clicked();
    void on_pushButtonYesV5_clicked();
    void on_pushButtonYesV6_clicked();
    void on_pushButtonYesV7_clicked();
    void on_pushButtonYesV8_clicked();
    void on_pushButtonYesV9_clicked();
    void on_pushButtonYesV10_clicked();
    void on_pushButtonYesV11_clicked();
    void on_pushButtonYesV12_clicked();
    void on_pushButtonYesV13_clicked();
    void on_pushButtonYesV14_clicked();
    void on_pushButtonYesV15_clicked();

    void extractCodes(int);

signals:
    void applyCodes(QString);
};

#endif // CODESELECT_H
