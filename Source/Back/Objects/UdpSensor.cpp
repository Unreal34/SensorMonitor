#include "UdpSensor.hpp"
#include <qdebug.h>
#include <qudpsocket.h>

UdpSensor::UdpSensor(quint16 port, const QHostAddress &sender, QObject *parent) : Sensor { parent }
{
    mType = ESensorType::Udp;

    QUdpSocket* udpSocket = new QUdpSocket(this);
    mDevice = udpSocket;
    Q_ASSERT(mDevice);

    setPort(port);
    setSenderAddress(sender);

    connect(udpSocket, &QUdpSocket::errorOccurred, this, &UdpSensor::onUdpSocketErrorReceived);
}

UdpSensor::UdpSensor(QIODevice *simulatedDevice, QObject *parent) : Sensor{ simulatedDevice, parent }
{
    mType = ESensorType::Udp;
}

bool UdpSensor::open(QIODeviceBase::OpenModeFlag flag)
{
    Q_UNUSED(flag);
    Q_ASSERT(mDevice);
    QUdpSocket* socket = udpSocket();
    Q_ASSERT(socket);

    bool bSuccess = socket->bind(QHostAddress::AnyIPv4, port());

    if(bSuccess)
    {
        connect(mDevice, &QIODevice::readyRead, this, &UdpSensor::onDataReceived);
    }

    return bSuccess;
}

void UdpSensor::onUdpSocketErrorReceived(QAbstractSocket::SocketError error)
{
    QUdpSocket* currentUdpSocket = udpSocket();
    Q_ASSERT(currentUdpSocket);

    QString errorString = {};
    ESensorError sensorError = ESensorError::UnhandledError;

    switch (error)
    {
        case QAbstractSocket::ConnectionRefusedError:
            sensorError = ESensorError::Disconnected;
            errorString = tr("The connection was refused by the remote host.");
            break;

        case QAbstractSocket::RemoteHostClosedError:
            sensorError = ESensorError::Disconnected;
            errorString = tr("The remote host closed the connection.");
            break;

        case QAbstractSocket::HostNotFoundError:
            sensorError = ESensorError::Disconnected;
            errorString = tr("The remote host was not found.");
            break;

        case QAbstractSocket::SocketAccessError:
            sensorError = ESensorError::OpeningIssue;
            errorString = tr("Permission denied while accessing the UDP socket.");
            break;

        case QAbstractSocket::SocketResourceError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The system does not have enough resources to create or use the UDP socket.");
            break;

        case QAbstractSocket::SocketTimeoutError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The UDP socket operation timed out.");
            break;

        case QAbstractSocket::DatagramTooLargeError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The UDP datagram is too large to be sent.");
            break;

        case QAbstractSocket::NetworkError:
            sensorError = ESensorError::Disconnected;
            errorString = tr("A network error occurred.");
            break;

        case QAbstractSocket::AddressInUseError:
            sensorError = ESensorError::OpeningIssue;
            errorString = tr("The UDP port is already in use by another application.");
            break;

        case QAbstractSocket::SocketAddressNotAvailableError:
            sensorError = ESensorError::OpeningIssue;
            errorString = tr("The requested local address is not available.");
            break;

        case QAbstractSocket::UnsupportedSocketOperationError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The requested UDP socket operation is not supported.");
            break;

        case QAbstractSocket::ProxyAuthenticationRequiredError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The proxy requires authentication.");
            break;

        case QAbstractSocket::SslHandshakeFailedError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The SSL handshake failed.");
            break;

        case QAbstractSocket::UnfinishedSocketOperationError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The socket operation could not be completed.");
            break;

        case QAbstractSocket::ProxyConnectionRefusedError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The connection to the proxy server was refused.");
            break;

        case QAbstractSocket::ProxyConnectionClosedError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The proxy server closed the connection.");
            break;

        case QAbstractSocket::ProxyConnectionTimeoutError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The connection to the proxy server timed out.");
            break;

        case QAbstractSocket::ProxyNotFoundError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The proxy server was not found.");
            break;

        case QAbstractSocket::ProxyProtocolError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("A proxy protocol error occurred.");
            break;

        case QAbstractSocket::OperationError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("The UDP socket operation is not permitted in its current state.");
            break;

        case QAbstractSocket::SslInternalError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("An internal SSL error occurred.");
            break;

        case QAbstractSocket::SslInvalidUserDataError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("Invalid SSL user data was provided.");
            break;

        case QAbstractSocket::TemporaryError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("A temporary UDP socket error occurred.");
            break;

        case QAbstractSocket::UnknownSocketError:
            sensorError = ESensorError::UnhandledError;
            errorString = tr("An unknown UDP socket error occurred.");
            break;
    }

    emit errorHandled(name(), sensorError, errorString);
}


