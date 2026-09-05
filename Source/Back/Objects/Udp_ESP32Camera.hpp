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
    uint32_t mCurrentFrameId = 0;
    QByteArray mFrameBuffer = {};
    uint16_t mPacketCount = 0;
    uint16_t mReceivedPacketCount = 0;
    QVector<bool> mReceivedPackets = {};
    bool mFrameInitialized = false;
};

#endif // UDP_ESP32CAMERA_HPP
