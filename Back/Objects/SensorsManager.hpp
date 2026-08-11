#ifndef SENSORSMANAGER_HPP
#define SENSORSMANAGER_HPP

#include <QObject>
#include <QUuid>

#include "SerialSensor.hpp"

class SensorsManager : public QObject
{
    Q_OBJECT
public:
    explicit SensorsManager(QObject *parent = nullptr);

public:
    SerialSensor* addNewSensor(const QString& serialPortName, const QString& name = {});
    SerialSensor* findSensorByName(const QString& name);
    bool deleteSensorByName(const QString& name);
    bool exists(const QString& sensorTag);

signals:
    void dataReceived(const QString& sensorName, const QByteArray& data);

private:
    QVector<SerialSensor*> mSensors = {};

};

#endif // SENSORSMANAGER_HPP
