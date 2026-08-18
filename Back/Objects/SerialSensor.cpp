#include "SerialSensor.hpp"
#include "SensorUtility.hpp"
#include <QUuid>

SerialSensor::SerialSensor(const QString &portName, QObject *parent) : QObject { parent }
, mPortName(portName)
, mName(QUuid().toString(QUuid::StringFormat::WithoutBraces))
, mSerialPort(new QSerialPort(this))
, mDevice(mSerialPort)
{
    Q_ASSERT(mSerialPort);

    mSerialPort->setPortName(portName);
    mSerialPort->setBaudRate(QSerialPort::Baud9600);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);
}

SerialSensor::SerialSensor(QIODevice *device, QObject *parent) : QObject { parent }
, mPortName(INVALID_SERIAL_PORT)
, mName(QUuid().toString(QUuid::StringFormat::WithoutBraces))
, mDevice(device)
{
}

SerialSensor::~SerialSensor()
{
    Q_ASSERT(mDevice);
    mDevice->close();
}

bool SerialSensor::open()
{
    Q_ASSERT(mDevice);

    bool bIsOpened = mDevice->open(QIODevice::ReadOnly);

    if(bIsOpened)
    {
        connect(mDevice, &QSerialPort::readyRead, this, &SerialSensor::onSerialDataReceived);
    }

    return bIsOpened;
}

void SerialSensor::onSerialDataReceived()
{
    while (mDevice->canReadLine())
    {
        // Returns a copy of this byte array with spacing characters removed from the start and end.
        // Spacing characters list: tabulation '\t', line feed '\n', carriage return '\r', vertical tabulation '\v', form feed '\f', and space ' '.
        const QByteArray data = mDevice->readLine().trimmed();
        emit dataReceived(data);
    }
}
