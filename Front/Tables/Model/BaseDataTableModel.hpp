#ifndef BASEDATATABLEMODEL_HPP
#define BASEDATATABLEMODEL_HPP

#include <QAbstractTableModel>
#include <QObject>

class BaseDataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum PersonnalDataRole
    {
        ValueType = Qt::UserRole + 1,
    };

public:
    explicit BaseDataTableModel(QObject *parent = nullptr);

public:
    size_t dataCount() const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QList<QVariant> getDataList();
    void setIsEditable(bool newIsSelectable);

public slots :

    void clear();
    void fill(const QList<QVariant>& dataList);
    void clearAndFill(const QList<QVariant>& dataList);

private:

    bool removeFromRow(int row, const QModelIndex &parent);
    void addData(const QVariant& data);

protected:

    QList<QVariant> mData;
    bool mIsEditable;
};

#endif // BASEDATATABLEMODEL_HPP
