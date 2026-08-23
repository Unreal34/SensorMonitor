#ifndef SENSORSMANAGER_HPP
#define SENSORSMANAGER_HPP

#include <QObject>
#include <QUuid>

#include "SerialSensorData.hpp"
#include "SerialSensor.hpp"
#include "Sensor.hpp"

class SensorsManager : public QObject
{
    Q_OBJECT
public:
    enum ESensorsManagerError
    {
        Success,
        SerialPortIssue,
        InvalidSensorName,
        Error
    };
    Q_ENUM(ESensorsManagerError)

public:
    explicit SensorsManager(QObject *parent = nullptr);

public:
    /**
     * @brief Creates and registers a new serial sensor with a name and a serial port.
     * @note Data are ready to be received after calling this function if the serial port and sensor are available.
     * @warning Each sensor name must be unique!
     * @param serialPortName
     * @param name
     * @return
     */
    bool registerNewSerialSensor(const QString& serialPortName, const QString& name, QIODevice* simulatedDevice = nullptr);

    /**
     * @brief Used to find a sensor by its unique name.
     * @param name
     * @return
     */
    Sensor* findSensorByName(const QString& name);

    /**
     * @brief openSensor
     * @param name
     * @return
     */
    bool openSensor(const QString& name);

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
     void resetSavedSensorData() { mSavedSerialSensorsData.clear(); }

    /**
     * @brief Save a list of SensorData.
     * @param newSavedSensorsData
     */
     void setSavedSensorsData(const QVector<SerialSensorData>& newSavedSensorsData) { mSavedSerialSensorsData = newSavedSensorsData; }

    /**
     * @brief Get access to saved sensor data buffer.
     * @return
     */
     const QVector<SerialSensorData>& savedSensorData() const { return mSavedSerialSensorsData; }

private:
     void deleteSensor(Sensor* target);

signals:
    /**
     * @brief Triggered for each new data received on the serial port connected to the sensor.
     * @param sensorName
     * @param data
     */
    void dataReceived(const QString& sensorName, const QByteArray& data);

    /**
     * @brief When an error is handled by the manager this signal triggered.
     * @param sensorName
     * @param serialPortName
     * @param data
     */
    void errorHandled(const QString& sensorName, const QString& serialPortName, SensorsManager::ESensorsManagerError error);

private:
    /**
     * @brief Manages the global list of sensors.
     */
    QVector<Sensor*> mSensors = {};

    /**
     * @brief Manages a list of serial sensors. Each sensor is connected to a serial port and available for data transfer.
     */
    QVector<SerialSensor*> mSerialSensors = {};

    /**
     * @brief Holds serial sensor information (name and port) updated from the SensorsEditorDialog.
     */
    QVector<SerialSensorData> mSavedSerialSensorsData = {};
};

#endif // SENSORSMANAGER_HPP
