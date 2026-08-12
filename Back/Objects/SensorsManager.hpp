#ifndef SENSORSMANAGER_HPP
#define SENSORSMANAGER_HPP

#include <QObject>
#include <QUuid>

#include "SensorData.hpp"
#include "SerialSensor.hpp"

class SensorsManager : public QObject
{
    Q_OBJECT
public:
    explicit SensorsManager(QObject *parent = nullptr);

public:
    SerialSensor* registerNewSensor(const QString& serialPortName, const QString& name = {});
    SerialSensor* findSensorByName(const QString& name);
    bool deleteSensorByName(const QString& name);
    void clear();
    bool exists(const QString& sensorTag);
    inline void resetSavedSensorsData() { mSavedSensorsData.clear(); }
    inline void setSavedSensorsData(const QVector<SensorData>& newSavedSensorsData) { mSavedSensorsData = newSavedSensorsData; }
    inline const QVector<SensorData>& savedSensorData() const { return mSavedSensorsData; }

signals:
    void dataReceived(const QString& sensorName, const QByteArray& data);

private:
    /**
     * @brief Hold a list of sensors linked to a serial port and available to data transfert.
     */
    QVector<SerialSensor*> mSensors = {};

    /**
     * @brief Hold a list of sensors data (name and port) updated from the SensorsEditorDialog.
     */
    QVector<SensorData> mSavedSensorsData = {};
};

#endif // SENSORSMANAGER_HPP
