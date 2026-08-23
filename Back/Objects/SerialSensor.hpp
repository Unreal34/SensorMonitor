#ifndef SERIALSENSOR_H
#define SERIALSENSOR_H

#include "Sensor.hpp"
#include <QObject>
#include <QSerialPort>

class SerialSensor : public Sensor
{
    Q_OBJECT
public:
    SerialSensor(const QString& portName, QObject *parent = nullptr);

    /**
     * @brief For simulating purpose only.
     * @param device
     * @param parent
     */
    SerialSensor(QIODevice* simulatedDevice, QObject *parent = nullptr);

public:
    /**
     * @brief Return the serial port name associated to this sensor.
     * @return
     */
    QString serialPortName() const { return serialPort()->portName(); };

    /**
     * @brief Return serial port object pointer associated with this sensor.
     * @return
     */
    QSerialPort* serialPort() const
    {
        QSerialPort* serialPort = qobject_cast<QSerialPort*>(mDevice);
        Q_ASSERT(serialPort);
        return serialPort;
    }

private:
    /**
     * @brief The serial port used by this sensor.
     */
    QString mPortName = {};
};

#endif // SERIALSENSOR_H
