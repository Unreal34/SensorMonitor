#include "SensorsManager.hpp"
#include <qassert.h>

SensorsManager::SensorsManager(QObject *parent) : QObject { parent }
{}

bool SensorsManager::registerNewSerialSensor(const QString &serialPortName, const QString &name, QIODevice* simulatedDevice)
{
    if(exists(name))
    {
        emit errorHandled(name, QString(tr("Sensor name %1 already exists.")).arg(name), ESensorsManagerError::InvalidSensorName);
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
        emit errorHandled(name, QString(tr("Invalid sensor name %1.")).arg(name), ESensorsManagerError::InvalidSensorName);
        delete sensor;
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

    // connect to sensor error handler.
    connect(sensor, &Sensor::errorHandled, this, &SensorsManager::onSensorErrorReceived);

    if(simulatedDevice)
    {
        emit errorHandled(name, QString(tr("Sensor %1 is ready!")).arg(name), ESensorsManagerError::Success);
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
