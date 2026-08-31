#include "SensorsManager.hpp"
#include <qassert.h>

SensorsManager::SensorsManager(QObject *parent) : QObject { parent }
{}

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
        delete sensor;
    }

    mSerialSensors.clear();
    mSensors.clear();
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

        default:
            Q_ASSERT_X(false, __FUNCTION__, "Sensor not handled yet!");
        break;
    }

    // remove from the generic sensor array too.
    bSuccess = mSensors.removeOne(target);
    Q_ASSERT(bSuccess);
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
