#include "SensorsManager.hpp"

SensorsManager::SensorsManager(QObject *parent)
    : QObject { parent }
{}

SerialSensor* SensorsManager::addNewSensor(const QString &serialPortName, const QString &name)
{
    Q_ASSERT(!exists(name));

    SerialSensor* sensor = new SerialSensor(serialPortName, this);
    Q_ASSERT(sensor);

    if(!name.isNull() && !name.isEmpty())
    {
        sensor->setName(name);
    }

    mSensors.push_back(sensor);

    connect(sensor, &SerialSensor::dataReceived, this, [sensor, this](const QByteArray& data)
    {
        emit dataReceived(sensor->name(), data);
    });

    return sensor;
}

SerialSensor* SensorsManager::findSensorByName(const QString &name)
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
    {
        if(sensor->name() == name)
        {
            return sensor;
        }
    }

    return nullptr;
}

bool SensorsManager::deleteSensorByName(const QString &name)
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
    {
        if(sensor->name() == name)
        {
            bool bSuccess = mSensors.removeOne(sensor);
            Q_ASSERT(bSuccess);
            delete sensor;
            return true;
        }
    }

    return false;
}

bool SensorsManager::exists(const QString &name)
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
    {
        if(sensor->name() == name)
        {
            return true;
        }
    }

    return false;
}
