#include "SerialSensor.hpp"
#include <QUuid>

SerialSensor::SerialSensor(const QString &portName, QObject *parent)
    : QObject { parent }
    , mPortName(portName)
    , mTag(QUuid().toString(QUuid::StringFormat::WithoutBraces))
    , mSerialPort(new QSerialPort(this))
{
    Q_ASSERT(mSerialPort);

    mSerialPort->setPortName(portName);
    mSerialPort->setBaudRate(QSerialPort::Baud9600);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    if(mSerialPort->open(QIODevice::ReadOnly))
    {
        connect(mSerialPort, &QSerialPort::readyRead, this, &SerialSensor::onSerialDataReceived);
    }
}

SerialSensor::~SerialSensor()
{
    mSerialPort->close();
}

void SerialSensor::setSerialPort(const QString &newSerialPort)
{
    Q_ASSERT(mSerialPort);

    if(mSerialPort->isOpen())
    {
        mSerialPort->close();
    }

    mSerialPort->setPortName(newSerialPort);
    mSerialPort->setBaudRate(QSerialPort::Baud9600);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);
}

void SerialSensor::onSerialDataReceived()
{
    while (mSerialPort->canReadLine())
    {
        const QByteArray data = mSerialPort->readLine().trimmed();
        emit dataReceived(data);
    }
}
