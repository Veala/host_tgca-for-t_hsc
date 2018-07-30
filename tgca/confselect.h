#ifndef CONFSELECT_H
#define CONFSELECT_H

#include <QDialog>

namespace Ui {
class ConfSelect;
}

class ConfSelect : public QDialog
{
    Q_OBJECT

public:
    explicit ConfSelect(QWidget *parent = 0);
    ~ConfSelect();

private:
    Ui::ConfSelect *ui;

private slots:
    void onRadio(bool);
    void onConfirm();
    void onBrowse();

    void on_radioButtonChooseFile_clicked(bool checked);

    void on_radioButtonChooseFile_pressed();

    void on_radioButtonChooseFile_released();

    void on_radioButtonChooseFile_toggled(bool checked);

signals:
    void configurate(QString);
};

#endif // CONFSELECT_H
