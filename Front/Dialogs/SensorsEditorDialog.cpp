#include "SensorsEditorDialog.hpp"
#include "SensorData.hpp"

SensorsEditorDialog::SensorsEditorDialog(const QVector<SensorData>& sensors, bool *ok, QWidget* parent) : BaseDialog(ok, tr("Validate"), tr("Cancel"), parent)
, mSensorsEditorWidget(new SensorsEditorWidget(sensors, this))
{
    setWindowTitle(tr("Sensors and ports"));
    resize(600, 400);
    containLayout()->addWidget(mSensorsEditorWidget);
}

QVector<SensorData> SensorsEditorDialog::sensorDataList()
{
    return mSensorsEditorWidget->updatedSensorData();
}
