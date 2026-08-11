#include "SensorsTableView.hpp"
#include "SensorsTableModel.hpp"

#include <QHeaderView>

SensorsTableView::SensorsTableView(QWidget *parent) : BaseDataTableView(parent)
{
    setModel(new SensorsTableModel(this));
    //setItemDelegate(new RoomsItemDelegate(this));
    verticalHeader()->hide();

    horizontalHeader()->setSectionResizeMode(SensorsTableModel::Column::SensorName, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(SensorsTableModel::Column::SerialPortName, QHeaderView::Stretch);
}

void SensorsTableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTableView::selectionChanged(selected, deselected);

    QModelIndexList indexes = selected.indexes();

    if(indexes.size() > 0 && mClearing == false)
    {
        mSelectedSensor = indexes.first().data(SensorsTableModel::PersonnalDataRole::ValueType).value<SensorData>();
        emit newSensorSelected(mSelectedSensor);
    }
    else
    {
        mSelectedSensor.reset();
    }
}
