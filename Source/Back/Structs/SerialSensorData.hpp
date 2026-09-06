#ifndef SERIALSENSORDATA_H
#define SERIALSENSORDATA_H

#include "Back/Structs/SensorData.hpp"
#include <QString>
#include <quuid.h>

/**
 * @brief Struct used to handle serial sensor data in the serial sensor editor widget for example.
 */
struct SerialSensorData : public SensorData
{
    SerialSensorData() : SensorData()
    {}

    SerialSensorData(const QString& _sensor_name, const QString & _sensor_portName) : SensorData(_sensor_name)
        , sensor_serialPortName(_sensor_portName)
    {}

    bool isValid()
    {
        return sensor_guid != QUuid();
    }

    virtual void reset() override
    {
        SensorData::reset();
        sensor_serialPortName = {};
    }

    QString sensor_serialPortName = {};
};

#endif // SERIALSENSORDATA_H
