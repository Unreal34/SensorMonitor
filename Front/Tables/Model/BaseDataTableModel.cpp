#include "BaseDataTableModel.hpp"

BaseDataTableModel::BaseDataTableModel(QObject *parent) : QAbstractTableModel{parent}
{}


size_t BaseDataTableModel::dataCount() const
{
    return mData.size();
}

int BaseDataTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mData.count();
}

void BaseDataTableModel::clear()
{
    unsigned int rowIndex = mData.count() - 1;

    while(!mData.isEmpty())
    {
        removeFromRow(rowIndex, QModelIndex());
        rowIndex--;
    }
}

void BaseDataTableModel::fill(const QList<QVariant> &dataList)
{
    Q_FOREACH(const QVariant& data, dataList)
    {
        addData(data);
    }
}

void BaseDataTableModel::clearAndFill(const QList<QVariant> &dataList)
{
    clear();
    fill(dataList);
}

bool BaseDataTableModel::removeFromRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    mData.removeAt(row);
    endRemoveRows();

    return true;
}

void BaseDataTableModel::addData(const QVariant &data)
{
    int count = mData.count();

    beginInsertRows(QModelIndex(), count, count);
    mData.push_back(data);
    endInsertRows();
}

void BaseDataTableModel::setIsEditable(bool newIsSelectable)
{
    mIsEditable = newIsSelectable;
}
