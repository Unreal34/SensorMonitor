#ifndef SERIALSENSORDATA_H
#define SERIALSENSORDATA_H

#include <QString>
#include <quuid.h>

/**
 * @brief Struct used to handle new sensor in the sensor editor widget fort example.
 */
struct SerialSensorData
{
    SerialSensorData()
        : sensor_guid(QUuid::createUuid())
    {}

    SerialSensorData(const QString& _sensor_name, const QString & _sensor_portName)
        : sensor_guid(QUuid::createUuid())
        , sensor_name(_sensor_name)
        , sensor_portName(_sensor_portName)
    {}

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

#endif // SERIALSENSORDATA_H
