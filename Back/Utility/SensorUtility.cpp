#include "SensorUtility.hpp"
#include "Utility.hpp"
#include <QVariant>
#include <QUuid>

bool SensorUtility::checkUniqueName(const QString &name, const QVector<SerialSensorData> &sensorData, const QUuid &escapeSensor)
{
    Q_FOREACH(const SerialSensorData& sensor, sensorData)
    {
        if(sensor.sensor_guid != escapeSensor && sensor.sensor_name == name)
        {
            return false;
        }
    }

    return true;
}

bool SensorUtility::checkUniqueSerialPort(const QString &serialPort, const QVector<SerialSensorData> &sensorData, const QUuid &escapeSensor)
{
    Q_FOREACH(const SerialSensorData& sensor, sensorData)
    {
        if(sensor.sensor_guid != escapeSensor && sensor.sensor_portName == serialPort && serialPort != INVALID_SERIAL_PORT)
        {
            return false;
        }
    }

    return true;
}

bool SensorUtility::variantListToSensorDataList(const QVariantList &variantList, QVector<SerialSensorData> &sensorData)
{
    sensorData.clear();
    sensorData.resize(variantList.size());

    for(int i = 0; i < variantList.size(); i++)
    {
        if(!variantList[i].canConvert<SerialSensorData>())
        {
            return false;
        }

        sensorData[i] = variantList[i].value<SerialSensorData>();
    }

    return true;
}

QString SensorUtility::randomSensorName()
{
    return QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);
}
