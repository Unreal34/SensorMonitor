#include "SensorsEditorDialog.hpp"
#include "Back/Structs/SerialSensorData.hpp"

SerialSensorsEditorDialog::SerialSensorsEditorDialog(const QVector<SerialSensorData>& sensors, bool *ok, QWidget* parent) : BaseDialog(ok, tr("Validate"), tr("Cancel"), parent)
, mSensorsEditorWidget(new SerialSensorsEditorWidget(sensors, this))
{
    setWindowTitle(tr("Sensors and ports"));
    resize(600, 400);
    containLayout()->addWidget(mSensorsEditorWidget);
}

QVector<SerialSensorData> SerialSensorsEditorDialog::sensorDataList()
{
    return mSensorsEditorWidget->updatedSensorData();
}
