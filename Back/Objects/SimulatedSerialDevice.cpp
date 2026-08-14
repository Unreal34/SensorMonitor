#include "SimulatedSerialDevice.hpp"

SimulatedSerialDevice::SimulatedSerialDevice(QObject *parent)
    : QIODevice{parent}
{}

void SimulatedSerialDevice::simulateIncomingData(const QByteArray& data)
{
    mData.append(data);
    emit readyRead();
}

qint64 SimulatedSerialDevice::readData(char* data, qint64 maxSize)
{
    const qint64 size = qMin(maxSize, static_cast<qint64>(mData.size()));

    // Copy size bytes from mData into the memory pointed to by data.
    memcpy(data, mData.constData(), size);
    mData.remove(0, size);
    return size;
}

qint64 SimulatedSerialDevice::writeData(const char* data, qint64 maxSize)
{
    Q_UNUSED(data);
    return maxSize;
}

bool SimulatedSerialDevice::canReadLine() const
{
    return mData.contains('\n');
}
