#ifndef SENSORSEDITORDIALOG_H
#define SENSORSEDITORDIALOG_H

#include <QObject>
#include "BaseDialog.hpp"
#include "SerialSensorsEditorWidget.hpp"

class SensorsEditorDialog : public BaseDialog
{
public:
    explicit SensorsEditorDialog(const QVector<SerialSensorData>& sensors, bool *ok, QWidget* parent = nullptr);

public:
    /**
     * @brief Return the list of SerialSensorData available.
     * @return
     */
    QVector<SerialSensorData> sensorDataList();

private:
    SerialSensorsEditorWidget* mSensorsEditorWidget = nullptr;
};

#endif // SENSORSEDITORDIALOG_H
