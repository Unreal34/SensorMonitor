#include <QtTest>

#include "SerialSensor.hpp"
#include "SimulatedDevice.hpp"

class SerialSensorTest : public QObject
{
    Q_OBJECT

private slots:
    void testName();
    void testPortName();
    void testDataReceived();
};

void SerialSensorTest::testName()
{
    SerialSensor sensor("COM3");

    sensor.setName("Temperature");

    QCOMPARE(sensor.name(), QString("Temperature"));
}

void SerialSensorTest::testPortName()
{
    SerialSensor sensor("COM3");

    QCOMPARE(sensor.serialPortName(), QString("COM3"));
}

void SerialSensorTest::testDataReceived()
{
    SimulatedDevice device;
    SerialSensor sensor(&device);
    sensor.open();

    QSignalSpy spy(&sensor, &SerialSensor::dataReceived);

    device.simulateIncomingData("Temperature:22.5\n");

    QCOMPARE(spy.count(), 1);

    const QList<QVariant> arguments = spy.takeFirst();

    QCOMPARE(arguments.at(0).toByteArray(), QByteArray("Temperature:22.5"));
}

QTEST_MAIN(SerialSensorTest)

#include "SerialSensorTest.moc"
