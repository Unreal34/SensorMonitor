#include "SerialSensorsEditorWidget.hpp"
#include "Back/Utility/Utility.hpp"
#include <quuid.h>
#include <Source/Front/Widgets/ui_BaseSensorsEditorWidget.h>

SerialSensorsEditorWidget::SerialSensorsEditorWidget(const QVector<SerialSensorData>& sensors, QWidget *parent) : BaseSensorsEditorWidget { parent }
, mSensorsTableView(new SerialSensorsTableView(this))
{
    ui->mMainLayout->insertWidget(0, mSensorsTableView);

    QVariantList datalist;

    Q_FOREACH(const SerialSensorData& data, sensors)
    {
        datalist.push_back(QVariant::fromValue(data));
    }

    mSensorsTableView->clearAndFill(datalist, 0);
}

QVector<SerialSensorData> SerialSensorsEditorWidget::updatedSensorData()
{
    return mSensorsTableView->sensorDataList();
}

void SerialSensorsEditorWidget::addNewSensor()
{
    QVariantList data;

    SerialSensorData s1;
    s1.sensor_name = s1.sensor_guid.toString(QUuid::WithoutBraces);
    s1.sensor_serialPortName = INVALID_SERIAL_PORT;

    data.push_back(QVariant::fromValue(s1));

    mSensorsTableView->append(data, 0);
}

void SerialSensorsEditorWidget::deleteCurrentSelectedSensor()
{
    mSensorsTableView->deleteSelectedSensor();
}
