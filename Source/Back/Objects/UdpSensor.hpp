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

    /**
     * @brief Set the UDP port where datagrams will be transfered.
     * @param port
     */
    void setPort(quint16 port) { mPort = port; }

    /**
     * @brief Return the UDP port.
     * @return
     */
    quint16 port() const { return mPort; }

    /**
     * @brief Set the sender address (the address of the sensor itself)
     * @param sender
     */
    void setSenderAddress(const QHostAddress& sender) { mSenderAddress = sender; }

    /**
     * @brief Get the sender address.
     * @return
     */
    const QHostAddress& senderAddress() const { return mSenderAddress; }

public:
    virtual bool open(QIODeviceBase::OpenModeFlag flag = QIODeviceBase::ReadOnly) override;

private slots:
    /**
     * @brief Called when the udp socket triggers an error.
     * @param error
     */
    void onUdpSocketErrorReceived(QUdpSocket::SocketError error);

private:

    /**
     * @brief mPort
     */
    quint16 mPort = 0;

    /**
     * @brief The IP address of the UDP sensor.
     * @note Used to filter incoming UDP packets and only process packets received from this sensor.
     */
    QHostAddress mSenderAddress = {};
};

#endif // UDPSENSOR_HPP
