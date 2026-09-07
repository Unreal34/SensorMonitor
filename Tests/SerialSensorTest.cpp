#include <QtTest>

#include "Back/Objects/SerialSensor.hpp"
#include "Back/Objects/SimulatedDevice.hpp"

class SerialSensorTest : public QObject
{
    Q_OBJECT

private slots:
    void name();
    void portName();
    void dataReceived();
    void baudRate();
};

void SerialSensorTest::name()
{
    SerialSensor sensor("COM3");

    sensor.setName("Temperature");

    QCOMPARE(sensor.name(), QString("Temperature"));
}

void SerialSensorTest::portName()
{
    SerialSensor sensor("COM3");

    QCOMPARE(sensor.serialPortName(), QString("COM3"));
}

void SerialSensorTest::dataReceived()
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

void SerialSensorTest::baudRate()
{
    SerialSensor sensor("COM3");
    sensor.setBaudRate(QSerialPort::BaudRate::Baud115200);
    QVERIFY(sensor.serialPort());
    QCOMPARE(sensor.baudRate(), QSerialPort::BaudRate::Baud115200);
}

QTEST_MAIN(SerialSensorTest)

#include "SerialSensorTest.moc"
