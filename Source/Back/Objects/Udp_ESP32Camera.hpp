#ifndef UDP_ESP32CAMERA_HPP
#define UDP_ESP32CAMERA_HPP

#include "UdpSensor.hpp"

class Udp_ESP32Camera : public UdpSensor
{
    Q_OBJECT
public:
    explicit Udp_ESP32Camera(quint16 port, const QHostAddress& sender, QObject *parent = nullptr);
    explicit Udp_ESP32Camera(QIODevice* simulatedDevice, QObject *parent = nullptr);

protected slots:
    virtual void onDataReceived();

private:
    void processDatagram(const QNetworkDatagram &datagram);

private:
    /**
     * @brief Identifier of the frame currently being received.
     */
    uint32_t mCurrentFrameId = 0;

    /**
     * @brief Buffer containing the data of the frame currently being received.
     */
    QByteArray mFrameBuffer = {};

    /**
     * @brief Total number of packets expected for the current frame.
     */
    uint16_t mPacketCount = 0;

    /**
     * @brief Number of packets successfully received for the current frame.
     */
    uint16_t mReceivedPacketCount = 0;

    /**
     * @brief Tracks which packets of the current frame have already been received.
     *
     * Each element corresponds to a packet index. A value of @c true indicates
     * that the corresponding packet has been received.
     */
    QVector<bool> mReceivedPackets = {};

    /**
     * @brief Indicates whether the current frame has been initialized.
     *
     * A frame is initialized when the first packet belonging to a new frame
     * is received.
     */
    bool mFrameReceptionInitialized = false;
};

#endif // UDP_ESP32CAMERA_HPP
