#include "BaseDataTableView.hpp"

BaseDataTableView::BaseDataTableView(QWidget *parent) : QTableView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

BaseDataTableModel *BaseDataTableView::dataTableModel() const
{
    BaseDataTableModel* dataTableModel = qobject_cast<BaseDataTableModel*>(model());
    Q_ASSERT(dataTableModel);
    return dataTableModel;
}

void BaseDataTableView::clear()
{
    mClearing = true;
    dataTableModel()->clear();
    mClearing = false;
}

void BaseDataTableView::clearAndFill(const QVariantList &data, int selectedIndex)
{
    mClearing = true;
    dataTableModel()->clearAndFill(data);
    mClearing = false;

    if(selectedIndex >= 0 && selectedIndex < dataTableModel()->dataCount())
    {
        selectRow(selectedIndex);
    }
}

void BaseDataTableView::append(const QVariantList &data, int selectedIndex)
{
    QVariantList current = dataTableModel()->data();
    current += data;
    clearAndFill(current);
}
