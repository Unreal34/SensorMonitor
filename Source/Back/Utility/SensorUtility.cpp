#include "SensorUtility.hpp"
#include "Utility.hpp"
#include <QVariant>
#include <QUuid>
#include <QImage>

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

QImage SensorUtility::createImageFromRGB565(const QByteArray& data, int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return {};
    }

    const qsizetype expectedSize = static_cast<qsizetype>(width) * static_cast<qsizetype>(height) * 2;

    if (data.size() < expectedSize)
    {
        return {};
    }

    QImage image(width, height, QImage::Format_RGB888);

    const uchar* src = reinterpret_cast<const uchar*>(data.constData());

    for (int y = 0; y < height; ++y)
    {
        uchar* dst = image.scanLine(y);

        for (int x = 0; x < width; ++x)
        {
            const qsizetype index =(static_cast<qsizetype>(y) * width + x) * 2;
            const uint8_t byte1 = src[index];
            const uint8_t byte2 = src[index + 1];

            const uint16_t pixel = (static_cast<uint16_t>(byte2) << 8) | byte1;

            const uint8_t r5 = (pixel >> 11) & 0x1F;
            const uint8_t g6 = (pixel >> 5) & 0x3F;
            const uint8_t b5 = pixel & 0x1F;

            // Expand RGB565 components to 8 bits.
            const uint8_t r = (r5 << 3) | (r5 >> 2);
            const uint8_t g = (g6 << 2) | (g6 >> 4);
            const uint8_t b = (b5 << 3) | (b5 >> 2);

            dst[x * 3 + 0] = r;
            dst[x * 3 + 1] = g;
            dst[x * 3 + 2] = b;
        }
    }

    return image;
}

QImage SensorUtility::createGrayscaleImage(const QByteArray& data, int width, int height)
{
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
