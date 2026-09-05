#include "Udp_ESP32Camera.hpp"
#include "Back/Structs/ESP32_PacketHeader.hpp"

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

    // Process all UDP datagrams currently waiting in the socket.
    while(socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();

        // Ignore datagrams that do not come from the configured camera IP address.
        if(datagram.senderAddress() != senderAddress())
        {
            continue;
        }

        QByteArray data = datagram.data();

        // Ignore datagrams that are too small to contain a complete packet header.
        if(data.size() < static_cast<int>(sizeof(ESP32_PacketHeader)))
        {
            continue;
        }

        // Extract the packet header from the received datagram.
        ESP32_PacketHeader header;
        memcpy(&header, data.constData(), sizeof(ESP32_PacketHeader));

        // Check that the announced frame size is valid.
        if(header.frameSize == 0 || header.frameSize > MAX_FRAME_SIZE)
        {
            continue;
        }

        // Check that the frame contains at least one packet.
        if(header.packetCount == 0)
        {
            continue;
        }

        // Check that the packet index is inside the announced packet range.
        if(header.packetIndex >= header.packetCount)
        {
            continue;
        }

        // Check that the announced payload size is valid.
        if(header.payloadSize == 0 || header.payloadSize > UDP_PAYLOAD_SIZE)
        {
            continue;
        }

        // Check that the datagram contains exactly the announced payload size.
        if(data.size() != static_cast<int>(sizeof(ESP32_PacketHeader) + header.payloadSize))
        {
            continue;
        }

        // Initialize the reconstruction buffers when receiving a new frame.
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

        // Ignore packets whose packet count does not match the current frame.
        if(header.packetCount != mPacketCount)
        {
            continue;
        }

        // Compute the position of the packet payload inside the complete frame buffer.
        const qsizetype offset = static_cast<qsizetype>(header.packetIndex) * UDP_PAYLOAD_SIZE;

        // Make sure the payload does not exceed the allocated frame buffer.
        if(offset + header.payloadSize > mFrameBuffer.size())
        {
            continue;
        }

        // Ignore duplicated UDP packets already received for the current frame.
        if(mReceivedPackets[header.packetIndex])
        {
            continue;
        }

        // Copy the packet payload to its corresponding position inside the frame buffer.
        memcpy(mFrameBuffer.data() + offset, data.constData() + sizeof(ESP32_PacketHeader), header.payloadSize);

        // Mark the packet as received.
        mReceivedPackets[header.packetIndex] = true;
        mReceivedPacketCount++;

        // qDebug() << "Frame:" << header.frameId << "Packet:" << header.packetIndex + 1 << "/" << header.packetCount << "Payload:" << header.payloadSize;

        // Emit the reconstructed frame once all its packets have been received.
        if(mReceivedPacketCount == mPacketCount)
        {
            emit dataReceived(mFrameBuffer);

            mFrameInitialized = false;
        }
    }
}
