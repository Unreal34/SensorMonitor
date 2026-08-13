#ifndef SERIALSENSOR_H
#define SERIALSENSOR_H

#include <QObject>
#include <QSerialPort>

class SerialSensor : public QObject
{
    Q_OBJECT
public:
    SerialSensor(const QString& portName, QObject *parent = nullptr);
    ~SerialSensor();

public:
    inline bool isAvailable() const { return mSerialPort->isOpen(); }

public:
    inline void setName(const QString& newName) { mName = newName; };
    inline const QString& name() const { return mName; };
    inline QString serialPortName() const { return mSerialPort->portName(); };

private slots:

    /**
     * @brief Data received from the connected serial port is forwarded through this signal.
     */
    void onSerialDataReceived();

signals:
    /**
     * @brief Triggered when a new data is available on the serial port associated with this sensor.
     * @param data
     */
    void dataReceived(const QByteArray& data);

private:

    /**
     * @brief The serial object used to handle the hardware connection between the sensor and this application.
     */
    QSerialPort* mSerialPort = nullptr;

    /**
     * @brief The serial port used by this sensor.
     */
    QString mPortName = {};

    /**
     * @brief The unique name of the sensor. Used to link a sensor with a serial port for example.
     */
    QString mName = {};
};

#endif // SERIALSENSOR_H
