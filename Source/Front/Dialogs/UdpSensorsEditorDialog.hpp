#ifndef UDPSENSORSEDITORDIALOG_HPP
#define UDPSENSORSEDITORDIALOG_HPP

#include "Back/Structs/UdpSensorData.hpp"
#include "BaseDialog.hpp"
#include "Front/Widgets/UdpSensorsEditorWidget.hpp"

class UdpSensorsEditorDialog : public BaseDialog
{
    Q_OBJECT
public:
    explicit UdpSensorsEditorDialog(const QVector<UdpSensorData>& sensors, bool *ok, QWidget* parent = nullptr);

public:
    /**
     * @brief Return the list of SerialSensorData available.
     * @return
     */
    QVector<UdpSensorData> sensorDataList();

private:
    UdpSensorsEditorWidget* mSensorsEditorWidget = nullptr;
};

#endif // UDPSENSORSEDITORDIALOG_HPP
