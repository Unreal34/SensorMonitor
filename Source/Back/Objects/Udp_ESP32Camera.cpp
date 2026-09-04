#include "Udp_ESP32Camera.hpp"
#include "Back/Structs/PacketHeader.hpp"

#include <QNetworkDatagram>
#include <QUdpSocket>
#include <qdebug.h>

Udp_ESP32Camera::Udp_ESP32Camera(quint16 port, const QHostAddress &sender, QObject *parent) : UdpSensor{ port, sender, parent }
{}

Udp_ESP32Camera::Udp_ESP32Camera(QIODevice *simulatedDevice, QObject *parent) : UdpSensor{ simulatedDevice, parent }
{}

void Udp_ESP32Camera::onDataReceived()
{
    QUdpSocket* socket = udpSocket();
    Q_ASSERT(socket);

    constexpr size_t UDP_PAYLOAD_SIZE = 1400;
    constexpr uint32_t MAX_FRAME_SIZE = 1024 * 1024;

    while(socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();

        if(datagram.senderAddress() != senderAddress())
        {
            return;
        }

        QByteArray data = datagram.data();

        if(data.size() < static_cast<int>(sizeof(ESP32_PacketHeader)))
        {
            continue;
        }

        ESP32_PacketHeader header;

        memcpy(&header, data.constData(), sizeof(ESP32_PacketHeader));

        if(header.frameSize == 0 || header.frameSize > MAX_FRAME_SIZE)
        {
            continue;
        }

        if(header.packetCount == 0)
        {
            continue;
        }

        if(header.packetIndex >= header.packetCount)
        {
            continue;
        }

        if(header.payloadSize == 0 || header.payloadSize > UDP_PAYLOAD_SIZE)
        {
            continue;
        }

        if(data.size() != static_cast<int>(sizeof(ESP32_PacketHeader) + header.payloadSize))
        {
            continue;
        }

        if(!mFrameInitialized || header.frameId != mCurrentFrameId)
        {
            mFrameInitialized = true;
            mCurrentFrameId = header.frameId;

            mFrameBuffer.resize(header.frameSize);

            mPacketCount = header.packetCount;
            mReceivedPacketCount = 0;

            mReceivedPackets.resize(header.packetCount);
            mReceivedPackets.fill(false);
        }

        if(header.packetCount != mPacketCount)
        {
            continue;
        }

        const qsizetype offset = static_cast<qsizetype>(header.packetIndex) * UDP_PAYLOAD_SIZE;

        if(offset + header.payloadSize > mFrameBuffer.size())
        {
            continue;
        }

        if(mReceivedPackets[header.packetIndex])
        {
            continue;
        }

        memcpy(mFrameBuffer.data() + offset, data.constData() + sizeof(ESP32_PacketHeader), header.payloadSize);

        mReceivedPackets[header.packetIndex] = true;
        mReceivedPacketCount++;

        // qDebug() << "Frame:" << header.frameId << "Packet:" << header.packetIndex + 1 << "/" << header.packetCount << "Payload:" << header.payloadSize;

        if(mReceivedPacketCount == mPacketCount)
        {
            emit dataReceived(mFrameBuffer);

            mFrameInitialized = false;
        }
    }
}
