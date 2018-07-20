#ifndef VSKREGVIEW_H
#define VSKREGVIEW_H

#include <QDialog>

namespace Ui {
class VSKRegView;
}
/*
enum REG_PROC_TYPE
{
    rpt_NONE,
    rpt_BIT,
    rpt_INT,
    rpt_RAM,
    rpt_STATUS,
    rpt_CONFIG,
    rpt_RX_CNTR,
    rpt_CREG,
    rpt_CR_SPI,
    rpt_AMPLIFICATION,
    rpt_PLL,
    rpt_NSK,
    rpt_END
};
*/
class VSKRegView : public QDialog
{
    Q_OBJECT

public:
    explicit VSKRegView(QWidget *parent = 0);
    ~VSKRegView();

    void configRegView(/*enum REG_PROC_TYPE type,*/ bool ro);

private:
    Ui::VSKRegView *ui;

private slots:
    void rememberReg();

signals:
    void remember(int);

};

#endif // VSKREGVIEW_H
