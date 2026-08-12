#ifndef SENSORSEDITORDIALOG_H
#define SENSORSEDITORDIALOG_H

#include <QObject>
#include "BaseDialog.hpp"
#include "SensorsEditorWidget.hpp"

class SensorsEditorDialog : public BaseDialog
{
public:
    explicit SensorsEditorDialog(const QVector<SensorData>& sensors, bool *ok, QWidget* parent = nullptr);

public:
    /**
     * @brief Return the list of SensorData available.
     * @return
     */
    QVector<SensorData> sensorDataList();

private:
    SensorsEditorWidget* mSensorsEditorWidget = nullptr;
};

#endif // SENSORSEDITORDIALOG_H
