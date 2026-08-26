#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QIODevice>

class Sensor : public QObject
{
    Q_OBJECT
public:
    enum ESensorType
    {
        None,
        Serial,
        Tcp,
        Udp,
        MqttTopic
    };
    Q_ENUM(ESensorType)

    enum ESensorError
    {
        Success,
        Disconnected,
        OpeningIssue,
        UnhandledError
    };
    Q_ENUM(ESensorError)

public:
    Sensor(QObject *parent = nullptr);
    Sensor(QIODevice* simulatedDevice, QObject *parent = nullptr);
    ~Sensor();

public:
    /**
     * @brief Set a name for the sensor.
     * @param newName
     */
    void setName(const QString& newName) { mName = newName; };

    /**
     * @brief Return the name of the sensor.
     * @return
     */
    const QString& name() const { return mName; };

    /**
     * @brief Check if the device is opened.
     * @return
     */
    bool isOpened() const { return mDevice->isOpen(); }

    /**
     * @brief Return the sensor type.
     * @return
     */
    ESensorType type() const { return mType; }

public:
    /**
     * @brief Open the device in read-only mode by default.
     * @return
     */
    bool open(QIODeviceBase::OpenModeFlag flag = QIODeviceBase::ReadOnly);

private slots:
    /**
     * @brief Data received from the device is handled by this slot.
     */
    void onDataReceived();

signals:
    /**
     * @brief Triggered when a new data is available on the device associated with this sensor.
     * @param data
     */
    void dataReceived(const QByteArray& data);

    /**
     * @brief Triggered when an error code is available for this sensor.
     * @param data
     */
    void errorHandled(const QString& sensorName, Sensor::ESensorError error, const QString& message);

protected:
    /**
     * @brief The unique name of the sensor. Used to link a sensor with a serial port for example.
     * @warning By design sensor name must be unique whend dealing with managers.
     */
    QString mName = {};

    /**
     * @brief Hold the device
     * @note Should be a real device such as a serial port, a tcp socket... or a simulated device.
     */
    QIODevice* mDevice = nullptr;

    /**
     * @brief Handle the type of this sensor.
     */
    ESensorType mType = ESensorType::None;
};

#endif // SENSOR_H
