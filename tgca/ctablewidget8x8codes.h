#ifndef CTABLEWIDGET2X2CODES_H
#define CTABLEWIDGET2X2CODES_H

#include <QTableWidget>

#define CTW8x8C_ColorTrue Qt::green
#define CTW8x8C_ColorFalse Qt::lightGray

class CTableWidget8x8Codes : public QTableWidget
{

public:
    explicit CTableWidget8x8Codes(QWidget *parent = 0);
    ~CTableWidget8x8Codes();

    void invertItem(QTableWidgetItem*);
    QChar cellVal(int row, int col);
protected:
    //void mouseDoubleClickEvent(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
private:
    bool isColorTrue(QTableWidgetItem* pItem) { return pItem->backgroundColor().blue() == 0; }
};


#endif // CTABLEWIDGET2X2CODES_H

