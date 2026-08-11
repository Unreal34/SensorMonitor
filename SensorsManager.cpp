#include "SensorsManager.hpp"

SensorsManager::SensorsManager(QObject *parent)
    : QObject { parent }
{}

SerialSensor* SensorsManager::addNewSensor(const QString &serialPortName, const QString &tag)
{
    Q_ASSERT(!exists(tag));

    SerialSensor* sensor = new SerialSensor(serialPortName, this);
    Q_ASSERT(sensor);

    if(!tag.isNull() && !tag.isEmpty())
    {
        sensor->setTag(tag);
    }

    mSensors.push_back(sensor);

    connect(sensor, &SerialSensor::dataReceived, this, [sensor, this](const QByteArray& data)
    {
        emit dataReceived(sensor->tag(), data);
    });

    return sensor;
}

SerialSensor* SensorsManager::findSensorByTag(const QString &tag)
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
    {
        if(sensor->tag() == tag)
        {
            return sensor;
        }
    }

    return nullptr;
}

bool SensorsManager::deleteSensorByTag(const QString &tag)
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
    {
        if(sensor->tag() == tag)
        {
            bool bSuccess = mSensors.removeOne(sensor);
            Q_ASSERT(bSuccess);
            delete sensor;
            return true;
        }
    }

    return false;
}

bool SensorsManager::exists(const QString &sensorTag)
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
    {
        if(sensor->tag() == sensorTag)
        {
            return true;
        }
    }

    return false;
}
