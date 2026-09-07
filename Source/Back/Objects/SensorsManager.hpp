#ifndef SENSORSMANAGER_HPP
#define SENSORSMANAGER_HPP

#include <QObject>
#include <QUuid>
#include <qassert.h>

#include "Back/Objects/UdpSensor.hpp"
#include "Back/Structs/SerialSensorData.hpp"
#include "Back/Structs/UdpSensorData.hpp"
#include "Back/Utility/Utility.hpp"
#include "SerialSensor.hpp"
#include "Sensor.hpp"

class SensorsManager : public QObject
{
    Q_OBJECT
public:
    enum ESensorsManagerError
    {
        Success,
        InvalidSensorName,
        SensorError,
        SensorRegistrationError,
        SensorOpeningError,
        EmptySavedBuffer,
        UnhandledError
    };
    Q_ENUM(ESensorsManagerError)

public:
    explicit SensorsManager(QObject *parent = nullptr);

    /**
     * @brief Creates and opens sensors from a list of Sensor data available in the saved buffers.
     * @todo Use an XML save file.
     * @return Return error code.
     */
    ESensorsManagerError registerAndOpenSensorsFromSavedData();

    /**
     * @brief Creates and registers a new serial sensor with a name and a serial port.
     * @note Data will be ready to receive after calling openSensor().
     * @warning Each sensor name must be unique in this manager!
     * @tparam T Serial sensor type. Must inherit from SerialSensor.
     * @param serialPortName
     * @param name
     * @param simulatedDevice
     * @return true if successfully registered or false otherwise.
     */
    template<typename T = SerialSensor>
    bool registerNewSerialSensor(const QString &serialPortName, const QString &name, QIODevice* simulatedDevice = nullptr)
    {
        Q_STATIC_ASSERT_X((std::is_base_of_v<SerialSensor, T>), "T must inherit from SerialSensor.");
        Q_STATIC_ASSERT_X((std::is_constructible_v<T, const QString&, QObject*>), "T must provide a constructor taking const QString& and QObject*.");
        Q_STATIC_ASSERT_X((std::is_constructible_v<T, QIODevice*, QObject*>), "T must provide a constructor taking QIODevice* and QObject*.");

        bool bSuccess = checkSensorName(name);

        if(!bSuccess)
        {
            return false;
        }

        SerialSensor* sensor = nullptr;

        if(simulatedDevice)
        {
            sensor = new T(simulatedDevice, this);
        }
        else
        {
            sensor = new T(serialPortName, this);
        }

        initializeSensor(sensor, name, simulatedDevice);
        mSerialSensors.push_back(sensor);

        return true;
    }

    /**
     * @brief Creates and registers a new udp sensor with a name, a port and a sender IP address.
     * @note Data will be ready to receive after calling openSensor().
     * @param port
     * @param sender
     * @param name
     * @param simulatedDevice
     * @return
     */
    template<typename T = UdpSensor>
    bool registerNewUdpSensor(quint16 port, const QHostAddress& sender, const QString &name, QIODevice* simulatedDevice = nullptr)
    {
        Q_STATIC_ASSERT_X((std::is_base_of_v<UdpSensor, T>), "T must inherit from UdpSensor.");
        Q_STATIC_ASSERT_X((std::is_constructible_v<T, quint16, const QHostAddress&, QObject*>), "T must provide a constructor taking quint16, const QHostAddress& and QObject*.");
        Q_STATIC_ASSERT_X((std::is_constructible_v<T, QIODevice*, QObject*>), "T must provide a constructor taking QIODevice* and QObject*.");
        Q_STATIC_ASSERT_X(!(std::is_same_v<UdpSensor, T>), "UdpSensor cannot be used directly.");

        bool bSuccess = checkSensorName(name);

        if(!bSuccess)
        {
            return false;
        }

        UdpSensor* sensor = nullptr;

        if(simulatedDevice)
        {
            sensor = new T(simulatedDevice, this);
        }
        else
        {
            sensor = new T(port, sender, this);
        }

        initializeSensor(sensor, name, simulatedDevice);
        mUdpSensors.push_back(sensor);

        return true;
    }

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
     * @brief Manually closes the connections and deletes all sensor objects managed by this manager.
     * @note When the application closes, sensors are automatically deleted through the parent-child ownership system, so there is no need to call clear().
     */
    void clear();

    /**
     * @brief Check if the sensor name is available in the global lists.
     * @param sensorTag
     * @return
     */
    bool exists(const QString& sensorTag);

    /**
     * @brief Save a list of SerialSensorData for future retrieval.
     * @note Used to save the Serial sensor configuration from the editor UI.
     * @param newSavedSensorsData The serial sensor configuration to save.
     */
    void setSavedSerialSensorsData(const QVector<SerialSensorData>& newSavedSensorsData) { mSavedSerialSensorsData = newSavedSensorsData; }

    /**
     * @brief Get access to the saved serial sensor data buffer.
     * @return
     */
    const QVector<SerialSensorData>& savedSerialSensorData() const { return mSavedSerialSensorsData; }

    /**
    * @brief Saves a list of UdpSensorData for future retrieval.
    * @note Used to save the UDP sensor configuration from the editor UI.
    * @param newSavedSensorsData The UDP sensor configuration to save.
    */
    void setSavedUdpSensorsData(const QVector<UdpSensorData>& newSavedSensorsData) { mSavedUdpSensorsData = newSavedSensorsData; }

    /**
    * @brief Get access to the saved udp sensor data buffer.
    * @return
    */
    const QVector<UdpSensorData>& savedUdpSensorData() const { return mSavedUdpSensorsData; }

private:
    /**
    * @brief Delete the sensor and remove it from the suitable lists.
    * @param target
    */
    void deleteSensor(Sensor* target);

    /**
    * @brief Helper used in registration function.
    * @param name
    * @return
    */
    bool checkSensorName(const QString& name);

    /**
    * @brief Helper used in registration function.
    * @param sensor
    */
    void initializeSensor(Sensor* sensor, const QString& name, QIODevice* simulatedDevice);

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
     * @brief Manages a list of UDP sensors. Each sensor sends UDP datagrams through a network port to the final application.
     */
    QVector<UdpSensor*> mUdpSensors = {};

    /**
     * @brief Holds serial sensor information (name and port) updated from the editor.
     */
    QVector<SerialSensorData> mSavedSerialSensorsData = {};

    /**
     * @brief Holds udp sensor information (name, port and sender) updated from the editor.
     */
    QVector<UdpSensorData> mSavedUdpSensorsData = {};
};

#endif // SENSORSMANAGER_HPP
