#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configuration.h"
#include "connection.h"
#include "device.h"

#include <QMessageBox>
#include <QInputDialog>
//#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPushConnect()
{
    //qDebug() << "onPushConnect()";
    //setWindowTitle(((QLineEdit*)cf->findChild<QLineEdit*>("lineEditIPDevice1"))->text());
}
/*
bool MainWindow::isConnected(int i)
{
    switch(i)
    {
    case 1:
        return connected1;
    case 2:
        return connected2;
    default:
        return false;
    }
}
*/
void MainWindow::onPushAbout()
{
    QMessageBox::about(this, tr("Справка"), tr("Шифр: \"Обработка 30\"\nВерсия: 1.0\n"));
}

void MainWindow::onPushConfig()
{
    Configuration *conf = new Configuration();
    bool mem[32*4*2*2];
    conf->update(mem);
    conf->show();
}

void MainWindow::onPushLoadTests()
{
}

void MainWindow::onPushProject()
{
}

void MainWindow::onPushReport()
{
}

void MainWindow::addDevice()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Добавить устройство"), tr("Имя устройства:"), QLineEdit::Normal, "", &ok);
    if (!ok) return;
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Добавить устройство"), tr("Не задано имя устройства"));
        return;
    }
    ui->devices->addWidget(new Device(0, name));
}
