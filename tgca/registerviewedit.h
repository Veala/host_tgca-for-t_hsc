#ifndef REGISTERVIEWEDIT_H
#define REGISTERVIEWEDIT_H

#include <QWidget>

class RegisterViewEdit : public QTableView
{
    Q_OBJECT

public:
    explicit RegisterViewEdit(QWidget *parent = 0);
    ~RegisterViewEdit();


};

 #endif   // REGISTERVIEWEDIT_H
