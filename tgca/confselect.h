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

signals:
    void configurate(QString, int*);
};

#endif // CONFSELECT_H
