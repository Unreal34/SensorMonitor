#include <QtTest>

#include "SensorsManager.hpp"

class SensorsManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void registerSensor();
    void findSensorByName();
    void deleteSensor();
    void sensorNameMustBeUnique();
    void clearSensors();
};

void SensorsManagerTest::registerSensor()
{
    SensorsManager manager;

    SerialSensor* sensor = manager.registerNewSensor("COM3", "TemperatureSensor");

    QVERIFY(sensor != nullptr);

    QCOMPARE(sensor->name(), QString("TemperatureSensor"));
}

void SensorsManagerTest::findSensorByName()
{
    SensorsManager manager;

    manager.registerNewSensor("COM3", "Sensor1");

    SerialSensor* sensor = manager.findSensorByName("Sensor1");

    QVERIFY(sensor != nullptr);
    QCOMPARE(sensor->name(), QString("Sensor1"));
    QCOMPARE(sensor->serialPortName(), QString("COM3"));
}

void SensorsManagerTest::deleteSensor()
{
    SensorsManager manager;

    manager.registerNewSensor("COM3", "Sensor1");
    QVERIFY(manager.exists("Sensor1"));

    const bool result = manager.deleteSensorByName("Sensor1");

    QVERIFY(result);
    QVERIFY(!manager.exists("Sensor1"));
}

void SensorsManagerTest::sensorNameMustBeUnique()
{
    SensorsManager manager;

    SerialSensor* sensor1 = manager.registerNewSensor("COM3", "Sensor1");
    SerialSensor* sensor2 = manager.registerNewSensor("COM4", "Sensor1");

    QVERIFY(sensor1 != nullptr);
    QVERIFY(sensor2 == nullptr);
}

void SensorsManagerTest::clearSensors()
{
    SensorsManager manager;

    manager.registerNewSensor("COM3", "Sensor1");
    manager.registerNewSensor("COM4", "Sensor2");

    manager.clear();

    QVERIFY(!manager.exists("Sensor1"));
    QVERIFY(!manager.exists("Sensor2"));
}

QTEST_MAIN(SensorsManagerTest)

#include "SensorsManagerTest.moc"
