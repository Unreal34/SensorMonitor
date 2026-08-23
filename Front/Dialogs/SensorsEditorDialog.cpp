#include "SensorsEditorDialog.hpp"
#include "SerialSensorData.hpp"

SensorsEditorDialog::SensorsEditorDialog(const QVector<SerialSensorData>& sensors, bool *ok, QWidget* parent) : BaseDialog(ok, tr("Validate"), tr("Cancel"), parent)
, mSensorsEditorWidget(new SerialSensorsEditorWidget(sensors, this))
{
    setWindowTitle(tr("Sensors and ports"));
    resize(600, 400);
    containLayout()->addWidget(mSensorsEditorWidget);
}

QVector<SerialSensorData> SensorsEditorDialog::sensorDataList()
{
    return mSensorsEditorWidget->updatedSensorData();
}
