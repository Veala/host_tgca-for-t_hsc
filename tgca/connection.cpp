#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);

    connect(ui->pushButtonCheckDevice, SIGNAL(clicked(bool)), this, SIGNAL(checkDevice(bool)));
    QShortcut* sc = new QShortcut(QKeySequence("Esc"), this);
    connect(sc,SIGNAL(activated()),this,SLOT(hide()));
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    //setWindowFlags(Qt::WindowCloseButtonHint);
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

QString Connection::getHostPORT() const
{
    return ui->lineEditPortIOPCNet->text();
}

void Connection::setServerIP(QString serverIp)
{
    ui->lineEditIPDevice->setText(serverIp);
}

void Connection::setServerPORT(QString PORT)
{
    ui->lineEditPortIODevice->setText(PORT);
}

void Connection::setHostIP(QString hostIp)
{
    ui->lineEditIPPCNet->setText(hostIp);
}

void Connection::setHostPORT(QString hostPort)
{
    ui->lineEditPortIOPCNet->setText(hostPort);
}

void Connection::setName(QString name)
{
    ui->labelTune->setText(tr("Настройки устройства %1").arg(name));
}
