#include "ESP32Camera.hpp"

ESP32Camera::ESP32Camera(const QString &portName, QObject *parent) : SerialCamera { portName, parent }
{
    setBaudRate(921600);
}

ESP32Camera::ESP32Camera(QIODevice *simulatedDevice, QObject *parent) : SerialCamera { simulatedDevice, parent }
{
}

void ESP32Camera::processBuffer()
{
    static const QByteArray frameHeader = "FRAME";

    while(true)
    {
        int headerIndex = mBuffer.indexOf(frameHeader);

        if(headerIndex < 0)
        {
            return;
        }

        if(headerIndex > 0)
        {
            mBuffer.remove(0, headerIndex);
        }

        constexpr int frameHeaderSize = 5;
        constexpr int frameSizeFieldSize = sizeof(uint32_t);

        if(mBuffer.size() < frameHeaderSize + frameSizeFieldSize)
        {
            return;
        }

        uint32_t frameSize = 0;

        memcpy(&frameSize, mBuffer.constData() + frameHeaderSize, sizeof(frameSize));

        constexpr uint32_t MAX_FRAME_SIZE = 1024 * 1024;

        // Sanity check to prevent invalid/corrupted frame sizes.
        if(frameSize == 0 || frameSize > MAX_FRAME_SIZE)
        {
            mBuffer.remove(0, frameHeaderSize);
            continue;
        }

        int completeFrameSize = frameHeaderSize + frameSizeFieldSize + static_cast<int>(frameSize);

        if(mBuffer.size() < completeFrameSize)
        {
            return;
        }

        QByteArray jpegData = mBuffer.mid(frameHeaderSize + frameSizeFieldSize, static_cast<int>(frameSize));
        mBuffer.remove(0, completeFrameSize);

        emit dataReceived(jpegData);
    }
}
