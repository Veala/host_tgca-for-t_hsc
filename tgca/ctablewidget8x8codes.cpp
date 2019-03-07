
#include "ctablewidget8x8codes.h"

#include <QMouseEvent>

CTableWidget8x8Codes::CTableWidget8x8Codes(QWidget* parent):
    QTableWidget(parent)
{
}

CTableWidget8x8Codes::~CTableWidget8x8Codes()
{
}

void CTableWidget8x8Codes::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        QPoint pos = event->pos();
        QTableWidgetItem * pItem = item(rowAt(pos.ry()), columnAt(pos.rx()));
        if (pItem)
            invertItem(pItem);
    }
}
/*
void CTableWidget8x8Codes::mouseDoubleClickEvent(QMouseEvent* ev)
{
    qDebug () << "mouseDoubleClickEvent";
    QPoint pos = ev->pos();
    QTableWidgetItem * pItem = item(rowAt(pos.ry()), columnAt(pos.rx()));
    if (pItem)
        invertItem(pItem);
}
*/
void CTableWidget8x8Codes::invertItem(QTableWidgetItem* pItem)
{
    pItem->setBackgroundColor(isColorTrue(pItem) ? CTW8x8C_ColorFalse : CTW8x8C_ColorTrue);
}

QChar CTableWidget8x8Codes::cellVal(int row, int col)
{
    return (isColorTrue(item(row, col)) ? '1' : '0');
}
