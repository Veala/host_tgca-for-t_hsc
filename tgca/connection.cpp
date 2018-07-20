#include "connection.h"
#include "ui_connection.h"

#include <QDebug>

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);

    connect(ui->pushButtonConnect, SIGNAL(clicked(bool)), this, SIGNAL(connectTry(bool)));
    connect(ui->pushButtonDisconnect, SIGNAL(clicked(bool)), this, SIGNAL(disconnectTry(bool)));
    //ui->pushButtonCheck->setEnabled(false);
}

Connection::~Connection()
{
    delete ui;
}

QString Connection::getServerIP() const
{
    return ui->lineEditIPDevice->text();
}

QString Connection::getServerPORT() const
{
    return ui->lineEditPortIODevice->text();
}

QString Connection::getHostIP() const
{
    return ui->lineEditIPPCNet->text();
}

void Connection::setName(QString name)
{
    ui->labelTune->setText(tr("Настройки устройства %1").arg(name));
}

void Connection::doError(QString err)
{
    QDateTime local(QDateTime::currentDateTime());
    ui->textBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss\n")) + tr("Ошибка: %1").arg(err));
}

void Connection::doMessage(QString mess)
{
    QDateTime local(QDateTime::currentDateTime());
    ui->textBrowser->append(local.toString(tr("dd.MM.yyyy hh:mm:ss\n")) + mess);
}
