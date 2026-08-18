#include "SensorsManager.hpp"

SensorsManager::SensorsManager(QObject *parent) : QObject { parent }
{}

bool SensorsManager::registerNewSensor(const QString &serialPortName, const QString &name, QIODevice* simulatedDevice)
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
    mSensors.push_back(sensor);

    connect(sensor, &SerialSensor::dataReceived, this, [sensor, this](const QByteArray& data)
    {
        emit dataReceived(sensor->name(), data);
    });

    emit errorHandled(name, serialPortName, ESensorsManagerError::Success);

    return true;
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

bool SensorsManager::openSensor(const QString &name)
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
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

void SensorsManager::clear()
{
    Q_FOREACH(SerialSensor* sensor, mSensors)
    {
        delete sensor;
    }

    mSensors.clear();
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
