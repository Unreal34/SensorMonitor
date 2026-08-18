#include <QtTest>

#include "SensorsManager.hpp"
#include "SimulatedSerialDevice.hpp"

class SensorsManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void registerSensor();
    void findSensorByName();
    void deleteSensor();
    void sensorNameMustBeUnique();
    void clearSensors();
    void openSensor();
    void testErrorHandled();
    void testDataReceived();
};

void SensorsManagerTest::registerSensor()
{
    SensorsManager manager;
    bool bSuccess = manager.registerNewSensor("COM3", "TemperatureSensor");

    QVERIFY(bSuccess == true);
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

    bool bCheckSensor1 = manager.registerNewSensor("COM3", "Sensor1");
    bool bCheckSensor2 = manager.registerNewSensor("COM4", "Sensor1");

    QVERIFY(bCheckSensor1 == true);
    QVERIFY(bCheckSensor2 == false);
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

void SensorsManagerTest::openSensor()
{
    SimulatedSerialDevice device;
    SerialSensor sensor(&device);
    bool bCheck = sensor.open();

    QVERIFY(bCheck == true);
}

void SensorsManagerTest::testErrorHandled()
{
    SensorsManager manager;

    QSignalSpy spy(&manager, &SensorsManager::errorHandled);
    manager.registerNewSensor("COM3", "Sensor1");

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.at(0);

    QCOMPARE(arguments.at(0).toByteArray(), QByteArray("Sensor1"));
    QCOMPARE(arguments.at(1).toByteArray(), QByteArray("COM3"));
    QCOMPARE(static_cast<SensorsManager::ESensorsManagerError>(arguments.at(2).toInt()), SensorsManager::ESensorsManagerError::Success);

    manager.registerNewSensor("COM2", "Sensor1");

    QCOMPARE(spy.count(), 2);

    arguments = spy.at(1);

    QCOMPARE(arguments.at(0).toByteArray(), QByteArray("Sensor1"));
    QCOMPARE(arguments.at(1).toByteArray(), QByteArray("COM2"));
    QCOMPARE(static_cast<SensorsManager::ESensorsManagerError>(arguments.at(2).toInt()), SensorsManager::ESensorsManagerError::InvalidSensorName);
}

void SensorsManagerTest::testDataReceived()
{
    SensorsManager manager;
    SimulatedSerialDevice* device = new SimulatedSerialDevice(this);
    manager.registerNewSensor("COM3", "Sensor1", device);

    bool bOpened = manager.openSensor("Sensor1");

    QVERIFY(bOpened == true);

    QSignalSpy spy(&manager, &SensorsManager::dataReceived);

    device->simulateIncomingData("Temperature:22.5\n");

    QCOMPARE(spy.count(), 1);

    const QList<QVariant> arguments = spy.takeFirst();

    QCOMPARE(arguments.at(0).toByteArray(), QByteArray("Sensor1"));
    QCOMPARE(arguments.at(1).toByteArray(), QByteArray("Temperature:22.5"));
}

QTEST_MAIN(SensorsManagerTest)

#include "SensorsManagerTest.moc"
