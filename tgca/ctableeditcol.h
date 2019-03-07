#ifndef CTABLEEDITCOL_H
#define CTABLEEDITCOL_H

#include <QTableWidget>
#include <QItemDelegate>
#include <QMouseEvent>
#include <QLineEdit>

class LineEditDelegate;

class CHexDelegate : public QItemDelegate
{
public:
    CHexDelegate (QObject *parent = 0): QItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index);
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

class CTableEditCol : public QTableWidget
{
    CHexDelegate * delegate;

public:
    explicit CTableEditCol(QWidget *parent = 0);
    ~CTableEditCol();

private:
    int columnAtX(int x) { return QTableWidget::columnAt(x); }
protected:
    void mouseDoubleClickEvent(QMouseEvent* ev);
};

class LineEditDelegate : public QItemDelegate { Q_OBJECT
    private:    int m_limit;
//--------------------------------------------------------------------------------------------------
    public:     LineEditDelegate(QObject * parent = 0, int limit = 1024) : QItemDelegate(parent) {
                    setLimit(limit);
                }
//--------------------------------------------------------------------------------------------------
    public:     void setLimit(int limit) {
                    m_limit = limit;
                }
//--------------------------------------------------------------------------------------------------
    public:     QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const {
                    QLineEdit * le = new QLineEdit(parent);
                    le->setMaxLength(m_limit);
                    return le;
                }
//--------------------------------------------------------------------------------------------------
    public:     void setEditorData(QWidget * editor, const QModelIndex& index) const {
                    if (QLineEdit * le = static_cast<QLineEdit *>(editor)) {
                        QString value = index.data(Qt::EditRole).toString();
                        le->setText(value);
                    }
                }
//--------------------------------------------------------------------------------------------------
    public:     void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex& index) const {
                    if (QLineEdit * le = static_cast<QLineEdit *>(editor)) {
                        QString value = le->text();
                        model->setData(index, value, Qt::EditRole);
                    }
                }
//--------------------------------------------------------------------------------------------------
};

#endif // CTABLEEDITCOL_H

