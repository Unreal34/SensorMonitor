#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QString>

struct SensorData
{
    SensorData()
    {}

    void reset()
    {
        sensor_name = {};
        sensor_portName = {};
    }

    QString sensor_name = {};
    QString sensor_portName = {};
};

#endif // SENSORDATA_H
