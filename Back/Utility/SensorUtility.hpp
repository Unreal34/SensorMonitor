#ifndef SENSORUTILITY_HPP
#define SENSORUTILITY_HPP

#include "SensorData.hpp"
#include <QObject>


#define INVALID_PORT "NONE"

class SensorUtility : public QObject
{
    Q_OBJECT
public:
    static bool checkUniqueName(const QString& name, const QVector<SensorData>& sensorData);
    static bool checkUniqueSerialPort(const QString& serialPort, const QVector<SensorData> &sensorData);
    static bool variantListToSensorDataList(const QVariantList& variantList, QVector<SensorData> &sensorData);
};

#endif // SENSORUTILITY_HPP
