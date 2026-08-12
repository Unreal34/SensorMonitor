#include "SensorsItemDelegate.hpp"
#include "SensorsTableModel.hpp"
#include "SerialPortCombobox.hpp"
#include <QTextOption>
#include <QPainter>
#include <qserialportinfo.h>

SensorsItemDelegate::SensorsItemDelegate(QObject *parent) : QItemDelegate{parent}
{}

QWidget *SensorsItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget* editor = nullptr;

    if(index.column() == SensorsTableModel::Column::SerialPortName)
    {
        QWidget* editor = new SerialPortCombobox(parent);

        SerialPortCombobox* combobox = qobject_cast<SerialPortCombobox*>(editor);
        Q_ASSERT(combobox);

        editor->setAutoFillBackground(true);
        return editor;
    }
    else
    {
        editor = QItemDelegate::createEditor(parent, option, index);
    }

    return editor;
}

void SensorsItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == SensorsTableModel::Column::SerialPortName)
    {
        SerialPortCombobox* combobox = qobject_cast<SerialPortCombobox*>(editor);
        Q_ASSERT(combobox);

        QString portName = index.model()->data(index, Qt::EditRole).toString();

        combobox->setCurrentText(portName);
    }
    else
    {
        QItemDelegate::setEditorData(editor,index);
    }
}

void SensorsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == SensorsTableModel::Column::SerialPortName)
    {
        SerialPortCombobox* combobox = qobject_cast<SerialPortCombobox*>(editor);
        Q_ASSERT(combobox);

        model->setData(index, combobox->currentText(), Qt::EditRole);
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void SensorsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}

void SensorsItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool SensorsItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QItemDelegate::editorEvent(event, model, option, index);
}
