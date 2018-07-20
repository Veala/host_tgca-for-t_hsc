#include "vskregview.h"
#include "ui_vskregview.h"

VSKRegView::VSKRegView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VSKRegView)
{
    ui->setupUi(this);
}

VSKRegView::~VSKRegView()
{
    delete ui;
}

void VSKRegView::configRegView(/*enum REG_PROC_TYPE type,*/ bool ro)
{
    if (ro)
    {
        ui->pushButtonCancel->setVisible(false);
        ui->pushButtonCancel->setEnabled(false);
        ui->pushButtonSave->setVisible(false);
        ui->pushButtonSave->setEnabled(false);
    }
}

void VSKRegView::rememberReg()
{
}
