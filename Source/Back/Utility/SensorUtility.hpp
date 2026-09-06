#ifndef SENSORUTILITY_HPP
#define SENSORUTILITY_HPP

#include "Back/Structs/SerialSensorData.hpp"
#include "Back/Structs/UdpSensorData.hpp"
#include <QObject>
#include <concepts>
#include <qvariant.h>

class SensorUtility : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Use this function to check if the sensor name is unique in the provided sensor list.
     * @return
     */
    template<typename T> requires std::derived_from<T, SensorData>
    static bool checkUniqueName(const QString& name, const QVector<T>& sensorData, const QUuid& escapeSensor)
    {
        Q_FOREACH(const T& sensor, sensorData)
        {
            if(sensor.sensor_guid != escapeSensor && sensor.sensor_name == name)
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Converts a QVariantList into a QVector of SensorData-derived structs.
     * @param variantList The list of QVariant objects to convert.
     * @param sensorData The output vector containing the converted sensor data.
     * @return True if all elements are successfully converted; false otherwise.
     */
    template<typename T> requires std::derived_from<T, SensorData>
    static bool variantListToSensorDataList(const QVariantList &variantList, QVector<T> &sensorData)
    {
        sensorData.clear();
        sensorData.resize(variantList.size());

        for(int i = 0; i < variantList.size(); i++)
        {
            if(!variantList[i].canConvert<T>())
            {
                return false;
            }

            sensorData[i] = variantList[i].value<T>();
        }

        return true;
    }

    /**
     * @brief Use this function to check if the serial port name is already used by a sensor in the provided sensor list.
     * @return
     */
    static bool checkUniqueSerialPort(const QString& serialPort, const QVector<SerialSensorData> &sensorData, const QUuid &escapeSensor);

    /**
     * @brief Generate a random sensor name.
     * @note Use Guid for random name generation.
     * @return
     */
    static QString randomSensorName();

    /**
     * @brief Function used with OV7670 to convert the streamed data to a grayscale image.
     * @param data
     * @param width
     * @param height
     * @return
     */
    static QImage createGrayscaleImage(const QByteArray& data, int width, int height);

    /**
     * @brief checkUniqueSenderIpAddress
     * @param sender
     * @param sensorData
     * @param escapeSensor
     * @return
     */
    static bool checkUniqueSenderIpAddress(const QHostAddress &sender, const QVector<UdpSensorData> &sensorData, const QUuid &escapeSensor);
};

#endif // SENSORUTILITY_HPP
