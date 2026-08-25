#ifndef SERIALSENSORSTABLEVIEW_H
#define SERIALSENSORSTABLEVIEW_H

#include <QObject>
#include "BaseDataTableView.hpp"
#include "SerialSensorData.hpp"

class SerialSensorsTableView : public BaseDataTableView
{
    Q_OBJECT
public:
    explicit SerialSensorsTableView(QWidget *parent = nullptr);

public:
    /**
     * @brief Delete the current selected entry or do nothing if the selected sensor is invalid.
     */
    void deleteSelectedSensor();

    /**
     * @brief Return a list of SerialSensorData struct. SerialSensorData are converted from the QVariant struct available in the model.
     * @return
     */
    QVector<SerialSensorData> sensorDataList();

protected:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

signals:
    void newSensorSelected(const SerialSensorData& sensor);

private:
    /**
     * @brief Hold the selected sensor.
     * @note Sensor guid is null and isValid() return false if none of the sensors are selected.
     */
    SerialSensorData mSelectedSensor;
};

#endif // SERIALSENSORSTABLEVIEW_H
