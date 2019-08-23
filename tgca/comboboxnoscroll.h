#ifndef COMBOBOXNOSCROLL_H
#define COMBOBOXNOSCROLL_H

#include <QComboBox>
#include <QWheelEvent>
#include <QDebug>

class ComboBoxNoScroll : public QComboBox
{
public:
    explicit ComboBoxNoScroll(QWidget *parent=0);
    ~ComboBoxNoScroll() {}

   // void wheelEvent(QWheelEvent *e) { qDebug() << "WheelEvent";}
};

#endif // COMBOBOXNOSCROLL_H
