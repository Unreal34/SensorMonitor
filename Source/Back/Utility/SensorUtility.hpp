#ifndef SENSORUTILITY_HPP
#define SENSORUTILITY_HPP

#include "Back/Structs/SerialSensorData.hpp"
#include <QObject>

class SensorUtility : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Use this function to check if the sensor name is unique in the provided sensor list.
     * @return
     */
    static bool checkUniqueName(const QString& name, const QVector<SerialSensorData>& sensorData, const QUuid &escapeSensor);

    /**
     * @brief Use this function to check if the serial port name is already used by a sensor in the provided sensor list.
     * @return
     */
    static bool checkUniqueSerialPort(const QString& serialPort, const QVector<SerialSensorData> &sensorData, const QUuid &escapeSensor);

    /**
     * @brief Convert the QVariantList into list of SensorData
     * @return Return true if conversion succeed false otherwise.
     * @warning Trigger assert in debug mode if the conversion fails.
     */
    static bool variantListToSensorDataList(const QVariantList& variantList, QVector<SerialSensorData> &sensorData);

    /**
     * @brief Generate a random sensor name.
     * @note Use Guid for random name generation.
     * @return
     */
    static QString randomSensorName();
};

#endif // SENSORUTILITY_HPP
