#ifndef SENSORSMANAGER_HPP
#define SENSORSMANAGER_HPP

#include <QObject>
#include <QUuid>
#include <qassert.h>

#include "SerialSensorData.hpp"
#include "SerialSensor.hpp"
#include "Sensor.hpp"

class SensorsManager : public QObject
{
    Q_OBJECT
public:
    enum ESensorsManagerError
    {
        /**
         * @todo More error handling.
         */
        Success,
        InvalidSensorName,
        SensorError,
        UnhandledError
    };
    Q_ENUM(ESensorsManagerError)

public:
    explicit SensorsManager(QObject *parent = nullptr);

public:
    /**
     * @brief Creates and registers a new serial sensor with a name and a serial port.
     * @note Data will be ready to receive after calling openSensor().
     * @warning Each sensor name must be unique in this manager!
     * @param serialPortName
     * @param name
     * @return
     */
    bool registerNewSerialSensor(const QString& serialPortName, const QString& name, QIODevice* simulatedDevice = nullptr);

    /**
     * @brief Used to find a sensor by its unique name.
     * @param name
     * @return Returns the sensor cast to the requested type, or triggers a compile-time error if T does not inherit from Sensor.
     */
    template<typename T>
    T* findSensorByName(const QString& name)
    {
        Q_STATIC_ASSERT_X((std::is_base_of_v<Sensor, T>), "T must inherit from Sensor base class.");

        for (Sensor* sensor : std::as_const(mSensors))
        {
            if (sensor->name() == name)
            {
                return qobject_cast<T*>(sensor);
            }
        }

        return nullptr;
    }
    /**
     * @brief Open the sensor device in Read-only mode.
     * @todo Handle more open mode.
     * @param name
     * @return
     */
    bool openSensor(const QString& name);

    /**
     * @brief Used to delete a sensor by its name.
     * @param name
     * @return true if the sensor was successfully deleted. False otherwise.
     */
    bool deleteSensorByName(const QString& name);

    /**
     * @brief Close connection and delete all sensor objects available in this manager.
     */
    void clear();

    /**
     * @brief Check if the sensor name is available in the global lists.
     * @param sensorTag
     * @return
     */
    bool exists(const QString& sensorTag);

    /**
     * @brief Clear the saved sensor data buffer.
     */
     void resetSavedSerialSensorData() { mSavedSerialSensorsData.clear(); }

    /**
     * @brief Save a list of SerialSensorData.
     * @note Used to save data available in the serial sensors editor UI.
     * @param newSavedSensorsData
     */
     void setSavedSerialSensorsData(const QVector<SerialSensorData>& newSavedSensorsData) { mSavedSerialSensorsData = newSavedSensorsData; }

    /**
     * @brief Get access to the saved serial sensor data buffer.
     * @return
     */
     const QVector<SerialSensorData>& savedSerialSensorData() const { return mSavedSerialSensorsData; }

private:
     /**
      * @brief Delete the sensor and remove it from the suitable lists.
      * @param target
      */
     void deleteSensor(Sensor* target);

signals:
    /**
     * @brief Triggered for each new data received on the device connected to the sensor.
     * @param sensorName
     * @param data
     */
    void dataReceived(const QString& sensorName, const QByteArray& data);

    /**
     * @brief When an error is handled by the manager this signal is triggered.
     * @param sensorName
     * @param serialPortName
     * @param data
     */
    void errorHandled(const QString& sensorName, const QString& message, SensorsManager::ESensorsManagerError error);

private slots:

    /**
     * @brief Handle error and dispatch.
     * @param sensorName
     * @param error
     * @param message
     */
    void onSensorErrorReceived(const QString& sensorName, Sensor::ESensorError error, const QString& message);

private:
    /**
     * @brief Manages the global list of generic sensors.
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
