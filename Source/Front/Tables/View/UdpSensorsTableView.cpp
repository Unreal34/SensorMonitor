#include "UdpSensorsTableView.hpp"
#include "Front/Tables/Delegate/UdpSensorsItemDelegate.hpp"
#include "Front/Tables/Model/UdpSensorsTableModel.hpp"
#include <qheaderview.h>
#include <Back/Utility/SensorUtility.hpp>

UdpSensorsTableView::UdpSensorsTableView(QWidget *parent) : BaseDataTableView { parent }
{
    setModel(new UdpSensorsTableModel(this));
    setItemDelegate(new UdpSensorsItemDelegate(this));
    verticalHeader()->hide();

    horizontalHeader()->setSectionResizeMode(UdpSensorsTableModel::Column::SensorName, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(UdpSensorsTableModel::Column::UdpPort, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(UdpSensorsTableModel::Column::SenderIpAddress, QHeaderView::Stretch);
}

QVector<UdpSensorData> UdpSensorsTableView::udpDataList()
{
    QVariantList currentDataList = dataList();
    QVector<UdpSensorData> sensorDataList;
    bool bSuccess = SensorUtility::variantListToUdpDataList(currentDataList, sensorDataList);
    Q_ASSERT(bSuccess);

    return sensorDataList;
}

void UdpSensorsTableView::deleteSelectedSensor()
{
    if(mSelectedSensor.isValid())
    {
        QVariantList currentDataList = dataTableModel()->dataList();

        int indexToRemove = -1;

        for(int i = 0; i < currentDataList.size(); i++)
        {
            Q_ASSERT(currentDataList[i].canConvert<UdpSensorData>());
            UdpSensorData data = currentDataList[i].value<UdpSensorData>();

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

void UdpSensorsTableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTableView::selectionChanged(selected, deselected);

    QModelIndexList indexes = selected.indexes();

    if(indexes.size() > 0 && mClearing == false)
    {
        mSelectedSensor = indexes.first().data(BaseDataTableModel::PersonnalDataRole::ValueType).value<UdpSensorData>();
        emit newSensorSelected(mSelectedSensor);
    }
    else
    {
        mSelectedSensor.reset();
    }
}
