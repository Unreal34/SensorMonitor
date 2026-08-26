#include <QtTest>

#include "Back/Objects/SensorsManager.hpp"
#include "Back/Objects/SimulatedDevice.hpp"

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
    void errorHandled();
    void dataReceived();
};

void SensorsManagerTest::registerSensor()
{
    SensorsManager manager;
    bool bSuccess = manager.registerNewSerialSensor("COM3", "TemperatureSensor");

    QVERIFY(bSuccess == true);
}

void SensorsManagerTest::findSensorByName()
{
    SensorsManager manager;

    manager.registerNewSerialSensor("COM3", "Sensor1");

    SerialSensor* serialSensor = manager.findSensorByName<SerialSensor>("Sensor1");

    QVERIFY(serialSensor != nullptr);
    QCOMPARE(serialSensor->name(), QString("Sensor1"));

    QVERIFY(serialSensor != nullptr);
    QCOMPARE(serialSensor->serialPortName(), QString("COM3"));
}

void SensorsManagerTest::deleteSensor()
{
    SensorsManager manager;

    manager.registerNewSerialSensor("COM3", "Sensor1");
    QVERIFY(manager.exists("Sensor1"));

    const bool result = manager.deleteSensorByName("Sensor1");

    QVERIFY(result);
    QVERIFY(!manager.exists("Sensor1"));
}

void SensorsManagerTest::sensorNameMustBeUnique()
{
    SensorsManager manager;

    bool bCheckSensor1 = manager.registerNewSerialSensor("COM3", "Sensor1");
    bool bCheckSensor2 = manager.registerNewSerialSensor("COM4", "Sensor1");

    QVERIFY(bCheckSensor1 == true);
    QVERIFY(bCheckSensor2 == false);
}

void SensorsManagerTest::clearSensors()
{
    SensorsManager manager;

    manager.registerNewSerialSensor("COM3", "Sensor1");
    manager.registerNewSerialSensor("COM4", "Sensor2");

    manager.clear();

    QVERIFY(!manager.exists("Sensor1"));
    QVERIFY(!manager.exists("Sensor2"));
}

void SensorsManagerTest::openSensor()
{
    SimulatedDevice device;
    SerialSensor sensor(&device);
    bool bCheck = sensor.open();

    QVERIFY(bCheck == true);
}

void SensorsManagerTest::errorHandled()
{
    SensorsManager manager;
    SimulatedDevice* device = new SimulatedDevice(this);

    QSignalSpy spy(&manager, &SensorsManager::errorHandled);
    manager.registerNewSerialSensor("COM3", "Sensor1", device);

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.at(0);

    QCOMPARE(arguments.at(0).toByteArray(), QByteArray("Sensor1"));;
    QCOMPARE(static_cast<SensorsManager::ESensorsManagerError>(arguments.at(2).toInt()), SensorsManager::ESensorsManagerError::Success);

    manager.registerNewSerialSensor("COM2", "Sensor1");

    QCOMPARE(spy.count(), 2);

    arguments = spy.at(1);

    QCOMPARE(arguments.at(0).toByteArray(), QByteArray("Sensor1"));
    QCOMPARE(static_cast<SensorsManager::ESensorsManagerError>(arguments.at(2).toInt()), SensorsManager::ESensorsManagerError::InvalidSensorName);
}

void SensorsManagerTest::dataReceived()
{
    SensorsManager manager;
    SimulatedDevice* device = new SimulatedDevice(this);
    manager.registerNewSerialSensor("COM3", "Sensor1", device);

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
