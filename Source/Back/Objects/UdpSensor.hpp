#ifndef UDPSENSOR_HPP
#define UDPSENSOR_HPP

#include "Sensor.hpp"
#include <QUdpSocket>

class UdpSensor : public Sensor
{
    Q_OBJECT
public:
    explicit UdpSensor(quint16 port, const QHostAddress& sender, QObject *parent = nullptr);
    explicit UdpSensor(QIODevice* simulatedDevice, QObject *parent = nullptr);

public:
    QUdpSocket* udpSocket() const
    {
        QUdpSocket* socket = qobject_cast<QUdpSocket*>(mDevice);
        Q_ASSERT(socket);
        return socket;
    }

    void setPort(quint16 port) { mPort = port; }
    quint16 port() const { return mPort; }
    void setSenderAddress(const QHostAddress& sender) { mSenderAddress = sender; }
    const QHostAddress& senderAddress() const { return mSenderAddress; }

public:
    virtual bool open(QIODeviceBase::OpenModeFlag flag = QIODeviceBase::ReadOnly) override;

private slots:
    void onUdpSocketErrorReceived(QUdpSocket::SocketError error);

private:
    quint16 mPort = 5000;
    QHostAddress mSenderAddress = {};
};

#endif // UDPSENSOR_HPP
