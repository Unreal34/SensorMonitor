#include "SerialCamera.hpp"


SerialCamera::SerialCamera(const QString &portName, QObject *parent) : SerialSensor{ portName, parent }
{
}

SerialCamera::SerialCamera(QIODevice *simulatedDevice, QObject *parent) : SerialSensor{ simulatedDevice, parent }
{
}

void SerialCamera::onDataReceived()
{
    Q_ASSERT(mDevice);
    mBuffer.append(mDevice->readAll());
    processBuffer();
}
