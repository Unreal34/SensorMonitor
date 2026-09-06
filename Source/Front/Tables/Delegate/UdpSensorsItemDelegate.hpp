#ifndef UDPSENSORSITEMDELEGATE_HPP
#define UDPSENSORSITEMDELEGATE_HPP

#include <QItemDelegate>

class UdpSensorsItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit UdpSensorsItemDelegate(QObject *parent = nullptr);

public:
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // UDPSENSORSITEMDELEGATE_HPP
