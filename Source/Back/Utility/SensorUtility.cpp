#include "SensorUtility.hpp"
#include "Back/Structs/UdpSensorData.hpp"
#include "Utility.hpp"
#include <QVariant>
#include <QUuid>
#include <QImage>

bool SensorUtility::checkUniqueSerialPort(const QString &serialPort, const QVector<SerialSensorData> &sensorData, const QUuid &escapeSensor)
{
    Q_FOREACH(const SerialSensorData& sensor, sensorData)
    {
        if(sensor.sensor_guid != escapeSensor && sensor.sensor_serialPortName == serialPort && serialPort != INVALID_SERIAL_PORT)
        {
            return false;
        }
    }

    return true;
}

bool SensorUtility::checkUniqueSenderIpAddress(const QHostAddress &sender, const QVector<UdpSensorData> &sensorData, const QUuid &escapeSensor)
{
    Q_FOREACH(const UdpSensorData& sensor, sensorData)
    {
        if(sensor.sensor_guid != escapeSensor && sensor.sensor_sender_ipAddress == sender && sensor.sensor_sender_ipAddress != QHostAddress(ANY_IP_ADDRESS))
        {
            return false;
        }
    }

    return true;
}

QString SensorUtility::randomSensorName()
{
    return QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);
}

QImage SensorUtility::createGrayscaleImage(const QByteArray& data, int width, int height)
{
    if(width <= 0 || height <= 0)
    {
        return {};
    }

    if(data.size() < width * height)
    {
        return {};
    }

    QImage image(width, height, QImage::Format_Grayscale8);

    for(int y = 0; y < height; ++y)
    {
        memcpy(image.scanLine(y),data.constData() + y * width, width);
    }

    return image;
}
