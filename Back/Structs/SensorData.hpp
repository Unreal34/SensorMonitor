#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QString>
#include <quuid.h>

/**
 * @brief Struct used to handle new sensor in the sensor editor widget fort example.
 */
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
