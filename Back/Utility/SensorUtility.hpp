#ifndef SENSORUTILITY_HPP
#define SENSORUTILITY_HPP

#include "SensorData.hpp"
#include <QObject>


#define INVALID_SERIAL_PORT "NONE"

class SensorUtility : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Use this function to check if the sensor name is unique in the provided sensor list.
     * @return
     */
    static bool checkUniqueName(const QString& name, const QVector<SensorData>& sensorData, const QUuid &escapeSensor);

    /**
     * @brief Use this function to check if the serial port name is already used by a sensor in the provided sensor list.
     * @return
     */
    static bool checkUniqueSerialPort(const QString& serialPort, const QVector<SensorData> &sensorData, const QUuid &escapeSensor);

    /**
     * @brief Convert the QVariantList into list of SensorData
     * @return Return true if conversion succeed false otherwise.
     * @warning Trigger assert in debug mode if the conversion fails.
     */
    static bool variantListToSensorDataList(const QVariantList& variantList, QVector<SensorData> &sensorData);
};

#endif // SENSORUTILITY_HPP
