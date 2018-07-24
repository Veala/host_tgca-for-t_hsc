#ifndef CTABLEEDITCOL_H
#define CTABLEEDITCOL_H

#include <QTableWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>

class CTableEditCol : public QTableWidget
{
public:
    CTableEditCol(QWidget* parent=0):QTableWidget(parent){}
   private:
    int columnAtX(int x) { return QTableWidget::columnAt(x); }
    void mouseDoubleClickEvent(QMouseEvent* ev) { QPoint pos = ev->pos(); int x = pos.rx(); int col = columnAtX(x); qDebug() << col;
                                                if (col==3) QTableWidget::mouseDoubleClickEvent(ev);}
};

#endif // CTABLEEDITCOL_H

