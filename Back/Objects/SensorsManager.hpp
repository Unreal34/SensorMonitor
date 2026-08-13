#ifndef SENSORSMANAGER_HPP
#define SENSORSMANAGER_HPP

#include <QObject>
#include <QUuid>

#include "SensorData.hpp"
#include "SerialSensor.hpp"

class SensorsManager : public QObject
{
    Q_OBJECT
public:
    explicit SensorsManager(QObject *parent = nullptr);

public:
    /**
     * @brief Creates and registers a new sensor with a name and a serial port.
     * @note Data are ready to be received after calling this function if the serial port and sensor are available.
     * @warning Each sensor name must be unique!
     * @param serialPortName
     * @param name
     * @return
     */
    SerialSensor* registerNewSensor(const QString& serialPortName, const QString& name = {});

    /**
     * @brief Used to find a sensor by its unique name.
     * @param name
     * @return
     */
    SerialSensor* findSensorByName(const QString& name);

    /**
     * @brief Used to delete a sensor by its name.
     * @param name
     * @return
     */
    bool deleteSensorByName(const QString& name);

    /**
     * @brief Close connection and delete all sensor objects available in this manager.
     */
    void clear();

    /**
     * @brief Check if the sensor name is available in the sensors array.
     * @param sensorTag
     * @return
     */
    bool exists(const QString& sensorTag);

    /**
     * @brief Clear the saved sensor data buffer.
     */
    inline void resetSavedSensorData() { mSavedSensorsData.clear(); }

    /**
     * @brief Save a list of SensorData.
     * @param newSavedSensorsData
     */
    inline void setSavedSensorsData(const QVector<SensorData>& newSavedSensorsData) { mSavedSensorsData = newSavedSensorsData; }

    /**
     * @brief Get access to saved sensor data buffer.
     * @return
     */
    inline const QVector<SensorData>& savedSensorData() const { return mSavedSensorsData; }

signals:
    /**
     * @brief Triggered for each new data received on the serial port connected to the sensor.
     * @param sensorName
     * @param data
     */
    void dataReceived(const QString& sensorName, const QByteArray& data);

private:
    /**
     * @brief Manages a list of sensors. Each sensor is connected to a serial port and available for data transfer.
     */
    QVector<SerialSensor*> mSensors = {};

    /**
     * @brief Holds sensor information (name and port) updated from the SensorsEditorDialog.
     */
    QVector<SensorData> mSavedSensorsData = {};
};

#endif // SENSORSMANAGER_HPP
