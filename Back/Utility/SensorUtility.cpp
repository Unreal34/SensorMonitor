#include "SensorUtility.hpp"
#include <QVariant>

bool SensorUtility::checkUniqueName(const QString &name, const QVector<SensorData> &sensorData, const QUuid &escapeSensor)
{
    Q_FOREACH(const SensorData& sensor, sensorData)
    {
        if(sensor.sensor_guid != escapeSensor && sensor.sensor_name == name)
        {
            return false;
        }
    }

    return true;
}

bool SensorUtility::checkUniqueSerialPort(const QString &serialPort, const QVector<SensorData> &sensorData, const QUuid &escapeSensor)
{
    Q_FOREACH(const SensorData& sensor, sensorData)
    {
        if(sensor.sensor_guid != escapeSensor && sensor.sensor_portName == serialPort && serialPort != INVALID_SERIAL_PORT)
        {
            return false;
        }
    }

    return true;
}

bool SensorUtility::variantListToSensorDataList(const QVariantList &variantList, QVector<SensorData> &sensorData)
{
    sensorData.clear();
    sensorData.resize(variantList.size());

    for(int i = 0; i < variantList.size(); i++)
    {
        if(!variantList[i].canConvert<SensorData>())
        {
            return false;
        }

        // Q_ASSERT(variantList[i].canConvert<SensorData>());
        sensorData[i] = variantList[i].value<SensorData>();
    }

    return true;
}
