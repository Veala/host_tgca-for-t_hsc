#include "confselect.h"
#include "ui_confselect.h"

ConfSelect::ConfSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfSelect)
{
    ui->setupUi(this);
}

ConfSelect::~ConfSelect()
{
    delete ui;
}
