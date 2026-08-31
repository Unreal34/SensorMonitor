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
     * @brief For simulating and testing purposes only.
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
     * @brief Return the serial port object pointer associated with this sensor.
     * @return
     */
    QSerialPort* serialPort() const
    {
        QSerialPort* serialPort = qobject_cast<QSerialPort*>(mDevice);
        Q_ASSERT(serialPort);
        return serialPort;
    }

    /**
     * @brief setBaudRate
     * @param baudRate
     * @param directions
     */
    void setBaudRate(qint32 baudRate, QSerialPort::Directions directions = QSerialPort::AllDirections);

private slots:
    /**
     * @brief Handle errors received by the serial port.
     * @param error
     */
    void onSerialErrorReceived(QSerialPort::SerialPortError error);

private:
    /**
     * @brief The serial port name used by this sensor.
     */
    QString mPortName = {};
};

#endif // SERIALSENSOR_H
