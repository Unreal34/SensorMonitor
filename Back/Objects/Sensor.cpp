#include "Sensor.hpp"

#include <QUuid>
#include "SensorUtility.hpp"

Sensor::Sensor(QObject *parent) : QObject { parent }
, mName(SensorUtility::randomSensorName())
{}

Sensor::Sensor(QIODevice *simulatedDevice, QObject *parent) : QObject { parent }
, mName(SensorUtility::randomSensorName())
{
    mDevice = simulatedDevice;
    Q_ASSERT(mDevice);
}

Sensor::~Sensor()
{
    Q_ASSERT(mDevice);

    disconnect(mDevice, nullptr, this, nullptr);

    if (mDevice->isOpen())
    {
        mDevice->close();
    }
}

bool Sensor::open(QIODevice::OpenModeFlag flag)
{
    Q_ASSERT(mDevice);

    bool bIsOpened = mDevice->open(flag);

    if(bIsOpened)
    {
        connect(mDevice, &QIODevice::readyRead, this, &Sensor::onDataReceived);
    }

    return bIsOpened;
}


void Sensor::onDataReceived()
{
    Q_ASSERT(mDevice);

    while (mDevice->canReadLine())
    {
        // Returns a copy of this byte array with spacing characters removed from the start and end.
        // Spacing characters list: tabulation '\t', line feed '\n', carriage return '\r', vertical tabulation '\v', form feed '\f', and space ' '.
        const QByteArray data = mDevice->readLine().trimmed();
        emit dataReceived(data);
    }
}
