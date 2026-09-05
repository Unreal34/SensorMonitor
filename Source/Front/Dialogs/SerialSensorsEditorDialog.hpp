#ifndef SERIALSENSORSEDITORDIALOG_H
#define SERIALSENSORSEDITORDIALOG_H

#include <QObject>
#include "BaseDialog.hpp"
#include "Front/Widgets/SerialSensorsEditorWidget.hpp"

class SerialSensorsEditorDialog : public BaseDialog
{
public:
    explicit SerialSensorsEditorDialog(const QVector<SerialSensorData>& sensors, bool *ok, QWidget* parent = nullptr);

public:
    /**
     * @brief Return the list of SerialSensorData available.
     * @return
     */
    QVector<SerialSensorData> sensorDataList();

private:
    SerialSensorsEditorWidget* mSensorsEditorWidget = nullptr;
};

#endif // SERIALSENSORSEDITORDIALOG_H
