#include "SensorsManager.hpp"
#include "Back/Objects/Udp_ESP32Camera.hpp"
#include "Serial_ESP32Camera.hpp"
#include "Serial_OV7670Camera.hpp"
#include "Back/Utility/Utility.hpp"
#include <qassert.h>

SensorsManager::SensorsManager(QObject *parent) : QObject { parent }
{}

bool SensorsManager::registerSensorsFromSavedData()
{
    if(savedSerialSensorData().size() <= 0 && savedUdpSensorData().size() <= 0)
    {
        return false;
    }

    Q_FOREACH(const SerialSensorData& current, savedSerialSensorData())
    {
        if(current.sensor_name.contains(OV7670_CAMERA))
        {
            registerNewSerialSensor<Serial_OV7670Camera>(current.sensor_serialPortName, current.sensor_name);
        }
        else if(current.sensor_name.contains(ESP32_CAMERA))
        {
            registerNewSerialSensor<Serial_ESP32Camera>(current.sensor_serialPortName, current.sensor_name);
        }
        else
        {
            registerNewSerialSensor(current.sensor_serialPortName, current.sensor_name);
        }

        openSensor(current.sensor_name);
    }

    Q_FOREACH(const UdpSensorData& current, savedUdpSensorData())
    {
        if(current.sensor_name.contains(ESP32_CAMERA))
        {
            registerNewUdpSensor<Udp_ESP32Camera>(current.sensor_udpPort, current.sensor_sender_ipAddress, current.sensor_name);
        }

        openSensor(current.sensor_name);
    }

    return true;
}

bool SensorsManager::openSensor(const QString &name)
{
    Q_FOREACH(Sensor* sensor, mSensors)
    {
        if(sensor->name() == name)
        {
            return sensor->open();
        }
    }

    return false;
}

bool SensorsManager::deleteSensorByName(const QString &name)
{
    Q_FOREACH(Sensor* sensor, mSensors)
    {
        if(sensor->name() == name)
        {
            deleteSensor(sensor);
            return true;
        }
    }

    return false;
}

void SensorsManager::clear()
{
    Q_FOREACH(Sensor* sensor, mSensors)
    {
        deleteSensor(sensor);
    }
}

bool SensorsManager::exists(const QString &name)
{
    Q_FOREACH(Sensor* sensor, mSensors)
    {
        if(sensor->name() == name)
        {
            return true;
        }
    }

    return false;
}

void SensorsManager::deleteSensor(Sensor *target)
{
    bool bSuccess = false;

    switch(target->type())
    {
        case Sensor::Serial:
            // remove the sensor from the serial sensors list first.
            bSuccess = mSerialSensors.removeOne(target);
            Q_ASSERT(bSuccess);
        break;

        case Sensor::Udp:
            // remove the sensor from the udp sensors list first.
            bSuccess = mUdpSensors.removeOne(target);
            Q_ASSERT(bSuccess);
            break;

        default:
            Q_ASSERT_X(false, __FUNCTION__, "Sensor not handled yet!");
        break;
    }

    // remove from the generic sensor array too.
    bSuccess = mSensors.removeOne(target);
    Q_ASSERT(bSuccess);

    // delete the sensor from memory.
    delete target;
}

void SensorsManager::onSensorErrorReceived(const QString& sensorName, Sensor::ESensorError error, const QString &message)
{
    switch (error)
    {
        case Sensor::Success:
            emit errorHandled(sensorName, QString(tr("Sensor %1 is ready!")).arg(sensorName), ESensorsManagerError::Success);
            break;
        default:
            emit errorHandled(sensorName, QString(tr("Error occured on sensor %1 - what: %2")).arg(sensorName, message), ESensorsManagerError::SensorError);
            break;
    }
}
