
#include "configuration.h"
#include "ui_configuration.h"

#include <QMessageBox>
#include <QDebug>

/// Функции чтения/записи значений регистров из окна конфигурации в устройство.

bool Configuration::onPushWrite()
{
    int count = 0;
    bool bRet = true;
    QByteArray pack;
    QByteArray array;

    for (int i=0; i<ui->tableWidgetNSK->rowCount(); i++)
    {
        ui->tableWidgetNSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        if (ui->tableWidgetNSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
        {
            count++;
            int addr = ROW_REG_NSK_2_ADDR(i);
            QString strval = ui->tableWidgetNSK->item(i, rcn_Val)->text();
            int val = strval.isEmpty() ? 0 : strval.toInt(0,16);
            array.append((char*)&addr, 4);
            array.append((char*)&val, 4);
            qDebug() << addr << "  " << val;
        }
    }
    if (ui->tableWidgetVSK->item(config_NUMREG_creg-config_NUMREG_BEGIN_VSK, rcn_Name_Check)->checkState() == Qt::Checked)
    {
        count++;
        int addr = REG_VSK_creg;
        QString strval = ui->tableWidgetVSK->item(config_NUMREG_creg-config_NUMREG_BEGIN_VSK, rcn_Val)->text();
        int val = strval.isEmpty() ? 0 : strval.toInt(0,16);
        array.append((char*)&addr, 4);
        array.append((char*)&val, 4);
        qDebug() << addr << "  " << val;
    }
    for (int i=0; i<ui->tableWidgetVSK->rowCount(); i++)
    {
        ui->tableWidgetVSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        if (ui->tableWidgetVSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked &&
            i != config_NUMREG_creg-config_NUMREG_BEGIN_VSK)
        {
            count ++;
            int addr = ROW_REG_VSK_2_ADDR(i);
            QString strval = ui->tableWidgetVSK->item(i, rcn_Val)->text();
            int val = strval.isEmpty() ? 0 : strval.toInt(0,16);
            array.append((char*)&addr, 4);
            array.append((char*)&val, 4);
            qDebug() << addr << "  " << val;
        }
    }

    if (count == 0)
    {
        QMessageBox::warning(this, tr("Запись регистров"), tr("Нет отмеченных регистров"), tr("Вернуться"));
    }
    else
    {
        /// формирование команды записи формата 1
        int cmd = 1;
        count *= 8; // 4 байта на адрес и 4 байта на значение
        pack.append((char*)&cmd, 4);
        pack.append((char*)&count, 4);
        qDebug() << count;
        pack.append(array);

        emit doWriteReg(pack);
    }
    return bRet;
}

void Configuration::doneWriteReg(int err)
{
    if (err == 0)
    {
        for (int i=0; i<ui->tableWidgetNSK->rowCount(); i++)
        {
            if (ui->tableWidgetNSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked &&
                    !registerNSKInfo_read_only(i))
                ui->tableWidgetNSK->item(i, rcn_Val)->setTextColor(Qt::blue);
        }
        for (int i=0; i<ui->tableWidgetVSK->rowCount(); i++)
        {
            if (ui->tableWidgetVSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked &&
                    !registerVSKInfo_read_only(i))
                ui->tableWidgetVSK->item(i, rcn_Val)->setTextColor(Qt::blue);
        }
    }
    qDebug() << "Register write error = " << err;
/*    setDisabled(false);
    blockSignals(false);
    setEnabled(true);
    qDebug() << "setDisabled(false)";
    qDebug() << "blockSignals(false)";
*/
}

bool Configuration::onPushRead()
{
    int count = 0;
    bool bRet = true;
    QByteArray pack;
    QByteArray array;

    for (int i=0; i<ui->tableWidgetNSK->rowCount(); i++)
    {
        ui->tableWidgetNSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        if (ui->tableWidgetNSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
        {
            count++;
            int addr = ROW_REG_NSK_2_ADDR(i);
            array.append((char*)&addr, 4);
        }
    }
    for (int i=0, j=config_NUMREG_BEGIN_VSK; i<ui->tableWidgetVSK->rowCount(); i++, j++)
    {
        ui->tableWidgetVSK->item(i, rcn_Val)->setTextColor(Qt::gray);
        if (ui->tableWidgetVSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
        {
            count ++;
            int addr = ROW_REG_VSK_2_ADDR(i);
            array.append((char*)&addr, 4);
        }
    }

    if (count == 0)
    {
        QMessageBox::warning(this, tr("Чтение регистров"), tr("Нет отмеченных регистров"), tr("Вернуться"));
    }
    else
    {
        /// формирование команды чтения формата 1
        int cmd = 3;
        count *= 4;
        pack.append((char*)&cmd, 4);
        pack.append((char*)&count, 4);
        pack.append(array);

        emit doReadReg(pack);
    }
    return bRet;
}

/// Обработка ответа cjvfyls xntybz htubcnhjd.
/// Значения запрошенных регистров находятся в массиве pack.
bool Configuration::doneReadReg(int err, QByteArray pack)
{
    char *pt = pack.data();
    bool bRet = false;
    if (err == 0)
    {
        int* pti = (int*)pt;
        int cmd = *pti;
        qDebug() << cmd;
        if (cmd == 3)
        {
            int countNSK = 0, countVSK = 0;
            for (int i=0; i<ui->tableWidgetVSK->rowCount(); i++)
            {
                if (ui->tableWidgetNSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
                    countNSK++;
            }
            for (int i=0; i<ui->tableWidgetVSK->rowCount(); i++)
            {
                if (ui->tableWidgetNSK->item(i, rcn_Name_Check)->checkState() == Qt::Checked)
                    countNSK++;
            }
            pt += 4;
            pti = (int*)pt;
            if ((countNSK + countVSK)*4 == *pti)
            {
                stopSign = true;
                for (int row=0; row<countNSK; row++)
                {
                    if (ui->tableWidgetNSK->item(row, rcn_Name_Check)->checkState() == Qt::Checked)
                    {
                        pt +=4;
                        pti = (int*)pt;
                        int newval = *pti;
                        //int row = reg_numbers[i];
                            ui->tableWidgetNSK->item(row, rcn_Val)->setTextColor(Qt::blue);
                            ui->tableWidgetNSK->item(row, rcn_Val)->setText((QString("%1").arg(newval, 4, 16, QChar('0'))).toUpper());
                    }
                }
                for (int row=0; row<countVSK; row++)
                {
                    if (ui->tableWidgetVSK->item(row, rcn_Name_Check)->checkState() == Qt::Checked)
                    {
                        pt +=4;
                        pti = (int*)pt;
                        int newval = *pti;
                        //int row = reg_numbers[i];
                        QString value = QString("%1").arg(newval, 4, 16, QChar('0')).toUpper();
                        ui->tableWidgetVSK->item(row, rcn_Val)->setText(value);
                        ui->tableWidgetVSK->item(row, rcn_Val)->setTextColor(Qt::blue);
                        int addr = NUM_REG_2_ADDR(row);
                        adaptRegVSK(addr, newval, value);
                    }
                }
                stopSign = false;
                bRet = true;
            }
        }
    }
    setDisabled(false);
    blockSignals(false);
    return bRet;
}
