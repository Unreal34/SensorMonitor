#include "SensorsTableView.hpp"
#include "SensorUtility.hpp"
#include "SensorsItemDelegate.hpp"
#include "SensorsTableModel.hpp"

#include <QHeaderView>

SensorsTableView::SensorsTableView(QWidget *parent) : BaseDataTableView(parent)
{
    setModel(new SensorsTableModel(this));
    setItemDelegate(new SensorsItemDelegate(this));
    verticalHeader()->hide();

    horizontalHeader()->setSectionResizeMode(SensorsTableModel::Column::SensorName, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(SensorsTableModel::Column::SerialPortName, QHeaderView::Stretch);
}

void SensorsTableView::deleteSelectedSensor()
{
    if(mSelectedSensor.isValid())
    {
        QVariantList currentDataList = dataTableModel()->dataList();

        int indexToRemove = -1;

        for(int i = 0; i < currentDataList.size(); i++)
        {
            Q_ASSERT(currentDataList[i].canConvert<SensorData>());
            SensorData data = currentDataList[i].value<SensorData>();

            if(data.sensor_guid == mSelectedSensor.sensor_guid)
            {
                indexToRemove = i;
                i = currentDataList.size();
            }
        }

        Q_ASSERT(indexToRemove >= 0);

        currentDataList.removeAt(indexToRemove);

        clearAndFill(currentDataList, 0);
    }
}

QVector<SensorData> SensorsTableView::sensorDataList()
{
    QVariantList currentDataList = dataList();
    QVector<SensorData> sensorDataList;
    bool bSuccess = SensorUtility::variantListToSensorDataList(currentDataList, sensorDataList);
    Q_ASSERT(bSuccess);

    return sensorDataList;
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
