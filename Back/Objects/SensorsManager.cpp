#include "SensorsManager.hpp"
#include <qassert.h>

SensorsManager::SensorsManager(QObject *parent) : QObject { parent }
{}

bool SensorsManager::registerNewSerialSensor(const QString &serialPortName, const QString &name, QIODevice* simulatedDevice)
{
    if(exists(name))
    {
        emit errorHandled(name, serialPortName, ESensorsManagerError::InvalidSensorName);
        return false;
    }

    SerialSensor* sensor = nullptr;

    if(simulatedDevice)
    {
        sensor = new SerialSensor(simulatedDevice, this);
    }
    else
    {
        sensor = new SerialSensor(serialPortName, this);
    }

    Q_ASSERT(sensor);

    if(name.isNull() || name.isEmpty())
    {
        delete sensor;
        emit errorHandled(name, serialPortName, ESensorsManagerError::InvalidSensorName);
        return false;
    }

    sensor->setName(name);

    // add the new sensor in the suitable arrays.
    mSensors.push_back(sensor);
    mSerialSensors.push_back(sensor);

    connect(sensor, &Sensor::dataReceived, this, [sensor, this](const QByteArray& data)
    {
        emit dataReceived(sensor->name(), data);
    });

    emit errorHandled(name, serialPortName, ESensorsManagerError::Success);

    return true;
}

Sensor* SensorsManager::findSensorByName(const QString &name)
{
    Q_FOREACH(Sensor* sensor, mSensors)
    {
        if(sensor->name() == name)
        {
            return sensor;
        }
    }

    return nullptr;
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
            bSuccess = mSerialSensors.removeOne(target);
            Q_ASSERT(bSuccess);
        break;

        default:
            Q_ASSERT_X(false, __FUNCTION__, "Sensor not handled yet!");
        break;
    }

    bSuccess = mSensors.removeOne(target);
    Q_ASSERT(bSuccess);
}
