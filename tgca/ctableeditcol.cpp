
#include "ctableeditcol.h"
#include "configuration.h"

#include <QDebug>

CTableEditCol::CTableEditCol(QWidget* parent):
    QTableWidget(parent)
{
    delegate = new CHexDelegate(this);
    setItemDelegateForColumn(rcn_Val, delegate);
    qDebug() << "delegate";
}

CTableEditCol::~CTableEditCol()
{
    delete delegate;
}

void CTableEditCol::mouseDoubleClickEvent(QMouseEvent* ev)
{
    QPoint pos = ev->pos();
    int x = pos.rx();
    if (columnAtX(x)==3)
        QTableWidget::mouseDoubleClickEvent(ev);
}

QWidget *CHexDelegate::createEditor(QWidget *parent,
              const QStyleOptionViewItem &option, const QModelIndex &index)
{
    qDebug() << "create editor";
    QLineEdit *editor = new QLineEdit(parent);
    editor->setValidator(new QRegExpValidator(QRegExp("^([1-9a-fA-F]{4}"), this));
    return editor;
}

void CHexDelegate::setModelData(QWidget *editor,
               QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    if (!edit)
        return;
    model->setData(index, edit->text());
}

 void CHexDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index).toString(); //получаем что уже есть в модели
    QLineEdit *fileDialog = static_cast<QLineEdit*> (editor);
    fileDialog->setText(value);
}
