#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QString>
#include <quuid.h>

struct SensorData
{
    SensorData()
    {
        sensor_guid = QUuid::createUuid();
    }

    bool isValid()
    {
        return sensor_guid != QUuid();
    }

    void reset()
    {
        sensor_guid = QUuid();
        sensor_name = {};
        sensor_portName = {};
    }

    QUuid sensor_guid = {};
    QString sensor_name = {};
    QString sensor_portName = {};
};

#endif // SENSORDATA_H
