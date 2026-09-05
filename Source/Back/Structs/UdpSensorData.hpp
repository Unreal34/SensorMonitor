#ifndef UDPSENSORDATA_HPP
#define UDPSENSORDATA_HPP

#include "SensorData.hpp"
#include <qhostaddress.h>

struct UdpSensorData : public SensorData
{
    UdpSensorData()
        : SensorData()
    {}

    UdpSensorData(const QString& _sensor_name, quint16 _sensor_udpPort, const QHostAddress& _sensor_sender_ipAddress)
        : SensorData()
        , sensor_udpPort(_sensor_udpPort)
        , sensor_sender_ipAddress(_sensor_sender_ipAddress)
    {}

    bool isValid()
    {
        return sensor_guid != QUuid();
    }

    virtual void reset() override
    {
        sensor_udpPort = 0;
        sensor_sender_ipAddress = QHostAddress();
    }

    quint16 sensor_udpPort = {};
    QHostAddress sensor_sender_ipAddress = {};
};

#endif // UDPSENSORDATA_HPP
