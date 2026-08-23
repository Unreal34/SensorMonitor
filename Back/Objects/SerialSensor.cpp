#include "SerialSensor.hpp"
#include "SensorUtility.hpp"
#include <QUuid>

SerialSensor::SerialSensor(const QString &portName, QObject *parent) : Sensor { parent }
, mPortName(portName)
{
    mType = ESensorType::Serial;

    QSerialPort* serialPort = new QSerialPort(this);
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    mDevice = serialPort;
    Q_ASSERT(mDevice);
}

SerialSensor::SerialSensor(QIODevice *simulatedDevice, QObject *parent) : Sensor { simulatedDevice, parent }
, mPortName(INVALID_SERIAL_PORT)
{
    mType = ESensorType::Serial;
}

