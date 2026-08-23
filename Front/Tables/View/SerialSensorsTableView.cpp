#include "SerialSensorsTableView.hpp"
#include "SensorUtility.hpp"
#include "SerialSensorsItemDelegate.hpp"
#include "SerialSensorsTableModel.hpp"

#include <QHeaderView>

SerialSensorsTableView::SerialSensorsTableView(QWidget *parent) : BaseDataTableView(parent)
{
    setModel(new SerialSensorsTableModel(this));
    setItemDelegate(new SerialSensorsItemDelegate(this));
    verticalHeader()->hide();

    horizontalHeader()->setSectionResizeMode(SerialSensorsTableModel::Column::SensorName, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(SerialSensorsTableModel::Column::SerialPortName, QHeaderView::Stretch);
}

void SerialSensorsTableView::deleteSelectedSensor()
{
    if(mSelectedSensor.isValid())
    {
        QVariantList currentDataList = dataTableModel()->dataList();

        int indexToRemove = -1;

        for(int i = 0; i < currentDataList.size(); i++)
        {
            Q_ASSERT(currentDataList[i].canConvert<SerialSensorData>());
            SerialSensorData data = currentDataList[i].value<SerialSensorData>();

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

QVector<SerialSensorData> SerialSensorsTableView::sensorDataList()
{
    QVariantList currentDataList = dataList();
    QVector<SerialSensorData> sensorDataList;
    bool bSuccess = SensorUtility::variantListToSensorDataList(currentDataList, sensorDataList);
    Q_ASSERT(bSuccess);

    return sensorDataList;
}

void SerialSensorsTableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTableView::selectionChanged(selected, deselected);

    QModelIndexList indexes = selected.indexes();

    if(indexes.size() > 0 && mClearing == false)
    {
        mSelectedSensor = indexes.first().data(SerialSensorsTableModel::PersonnalDataRole::ValueType).value<SerialSensorData>();
        emit newSensorSelected(mSelectedSensor);
    }
    else
    {
        mSelectedSensor.reset();
    }
}
