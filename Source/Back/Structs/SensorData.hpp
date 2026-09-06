#ifndef SENSORDATA_HPP
#define SENSORDATA_HPP

#include <QString>
#include <quuid.h>

struct SensorData
{
    SensorData()
        : sensor_guid(QUuid::createUuid())
    {}

    SensorData(const QString& _sensor_name)
        : sensor_guid(QUuid::createUuid())
        , sensor_name(_sensor_name)
    {}

    bool isValid()
    {
        return sensor_guid != QUuid();
    }

    virtual void reset()
    {
        sensor_guid = QUuid();
        sensor_name = {};
    }

    QUuid sensor_guid = {};
    QString sensor_name = {};
};

#endif // SENSORDATA_HPP
