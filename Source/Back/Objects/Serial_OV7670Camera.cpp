#include "Serial_OV7670Camera.hpp"

Serial_OV7670Camera::Serial_OV7670Camera(const QString& portName, QObject *parent ) : SerialCamera { portName, parent }
{
    serialPort()->setBaudRate(1'000'000);
}

Serial_OV7670Camera::Serial_OV7670Camera(QIODevice *simulatedDevice, QObject *parent) : SerialCamera { simulatedDevice, parent }
{}

void Serial_OV7670Camera::processBuffer()
{
    static const QByteArray magic = QByteArray::fromHex("AA55AA55");
    constexpr qsizetype headerSize = 8;

    while (true)
    {
        // Find beginning of a frame.
        qsizetype start = mBuffer.indexOf(magic);

        if (start < 0)
        {
            // Keep the last few bytes in case they contain
            // the beginning of AA 55 AA 55.
            if (mBuffer.size() > 3)
            {
                mBuffer.remove(0, mBuffer.size() - 3);
            }

            return;
        }

        // Remove garbage before the frame.
        if (start > 0)
        {
            mBuffer.remove(0, start);
        }

        // We don't have the complete header yet.
        if (mBuffer.size() < headerSize)
        {
            return;
        }

        const uint16_t width = static_cast<uint8_t>(mBuffer[4]) | (static_cast<uint16_t>(static_cast<uint8_t>(mBuffer[5])) << 8);
        const uint16_t height = static_cast<uint8_t>(mBuffer[6]) | (static_cast<uint16_t>(static_cast<uint8_t>(mBuffer[7])) << 8);
        const qsizetype imageSize = static_cast<qsizetype>(width) * static_cast<qsizetype>(height);
        const qsizetype packetSize = headerSize + imageSize;

        // Wait for the rest of the frame.
        if (mBuffer.size() < packetSize)
        {
            return;
        }

        // Extract GrayScale bytes.
        QByteArray imageData = mBuffer.mid(headerSize, imageSize);

        // Remove the complete packet.
        mBuffer.remove(0, packetSize);

        emit dataReceived(imageData);
    }
}
