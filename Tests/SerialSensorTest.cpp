#include <QtTest>

#include "SerialSensor.hpp"

class SimulatedSerialDevice : public QIODevice
{
    Q_OBJECT

public:
    explicit SimulatedSerialDevice(QObject* parent = nullptr)
        : QIODevice(parent)
    {
    }

    void simulateIncomingData(const QByteArray& data)
    {
        mData.append(data);
        emit readyRead();
    }

protected:
    virtual qint64 readData(char* data, qint64 maxSize) override
    {
        const qint64 size = qMin(maxSize, static_cast<qint64>(mData.size()));
        memcpy(data, mData.constData(), size);
        mData.remove(0, size);
        return size;
    }

    virtual qint64 writeData(const char* data, qint64 maxSize) override
    {
        Q_UNUSED(data);
        return maxSize;
    }

    virtual bool canReadLine() const override
    {
        return mData.contains('\n');
    }

private:
    QByteArray mData;
};

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
    SimulatedSerialDevice device;
    SerialSensor sensor(&device);

    QSignalSpy spy(&sensor, &SerialSensor::dataReceived);

    device.simulateIncomingData("Temperature:22.5\n");

    QCOMPARE(spy.count(), 1);

    const QList<QVariant> arguments = spy.takeFirst();

    QCOMPARE(arguments.at(0).toByteArray(), QByteArray("Temperature:22.5"));
}

QTEST_MAIN(SerialSensorTest)

#include "SerialSensorTest.moc"
