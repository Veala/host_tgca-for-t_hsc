#include "confselect.h"
#include "ui_confselect.h"

#include <QFileDialog>

ConfSelect::ConfSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfSelect)
{
    ui->setupUi(this);
//    connect(this, SIGNAL(configurate(QString, int*)), parent, SLOT(initFrom(QString, int*)));
}

ConfSelect::~ConfSelect()
{
    delete ui;
}

void ConfSelect::onRadioChoose(bool b)
{
    ui->comboBoxConfigurations->setEnabled(b);
    ui->lineEditPathToConf->setEnabled(!b);
    ui->pushButtonBrowse->setEnabled(!b);
}

void ConfSelect::onConfirm()
{
    QString confname;
    if (ui->radioButtonChoose->isChecked())
        confname = ui->comboBoxConfigurations->currentText();
    else
        confname = ui->lineEditPathToConf->text();
    if (!confname.isEmpty())
    {
        int waiting = 1;
        emit configurate(confname, &waiting);
        while (waiting == 1);

        if (waiting == 0)
            accept();
    }
}

void ConfSelect::onBrowse()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл конфигурации"), "/home/", tr("Файлы конфигурации (*.cnf)\nВсе файлы (*.*)"));

    if (!fileName.isEmpty())
    {
        ui->lineEditPathToConf->setText(fileName);
    }
}
