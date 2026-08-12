#ifndef SENSORSITEMDELEGATE_HPP
#define SENSORSITEMDELEGATE_HPP

#include <QItemDelegate>
#include <QObject>

class SensorsItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit SensorsItemDelegate(QObject *parent = nullptr);

public:
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // SENSORSITEMDELEGATE_HPP
