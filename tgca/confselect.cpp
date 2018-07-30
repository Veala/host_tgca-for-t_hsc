#include "confselect.h"
#include "ui_confselect.h"

#include <QFileDialog>
#include <QDebug>

ConfSelect::ConfSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfSelect)
{
    ui->setupUi(this);
    connect(this, SIGNAL(configurate(QString)), parent, SLOT(initFrom(QString)));
}

ConfSelect::~ConfSelect()
{
    delete ui;
}

void ConfSelect::onRadio(bool b)
{
    qDebug() << b;
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
        emit configurate(confname);
    accept();
}

void ConfSelect::onBrowse()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл конфигурации"), "/home/", tr("Текстовые файлы (*.txt)\nВсе файлы (*.*)"));

    if (!fileName.isEmpty())
    {
        ui->lineEditPathToConf->setText(fileName);
    }
}

void ConfSelect::on_radioButtonChooseFile_clicked(bool checked)
{
    qDebug() << "clicked   " << checked;
}

void ConfSelect::on_radioButtonChooseFile_pressed()
{
    qDebug() << "pressed";

}

void ConfSelect::on_radioButtonChooseFile_released()
{
    qDebug() << "released";

}

void ConfSelect::on_radioButtonChooseFile_toggled(bool checked)
{
    qDebug() << "toggled   " << checked;

}
