#include "UdpSensorsEditorDialog.hpp"
#include "Back/Structs/UdpSensorData.hpp"

UdpSensorsEditorDialog::UdpSensorsEditorDialog(const QVector<UdpSensorData>& sensors, bool *ok, QWidget* parent) : BaseDialog(ok, tr("Validate"), tr("Cancel"), parent)
, mSensorsEditorWidget(new UdpSensorsEditorWidget(sensors, this))
{
    setWindowTitle(tr("Udp sensors and ports"));
    resize(640, 480);
    containLayout()->addWidget(mSensorsEditorWidget);
}

QVector<UdpSensorData> UdpSensorsEditorDialog::sensorDataList()
{
    return mSensorsEditorWidget->updatedSensorData();
}
