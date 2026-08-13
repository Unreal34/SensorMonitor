#include <QtTest>

#include "SensorUtility.hpp"

class SensorUtilityTest : public QObject
{
    Q_OBJECT

private slots:
    void checkUniqueName();
    void checkUniqueSerialPort();
};

void SensorUtilityTest::checkUniqueName()
{
    QVector<SensorData> testDataList;
    testDataList.push_back(SensorData("Sensor1", "COM1"));
    testDataList.push_back(SensorData("Sensor2", "COM2"));

    SensorData testSensorData1("Sensor2", "COM0");
    SensorData testSensorData2("Sensor3", "COM2");

    QVERIFY(SensorUtility::checkUniqueName(testSensorData1.sensor_name, testDataList, testSensorData1.sensor_guid) == false);
    QVERIFY(SensorUtility::checkUniqueName(testSensorData2.sensor_name, testDataList, testSensorData2.sensor_guid) == true);

    testDataList.clear();
    testDataList.push_back(SensorData("Sensor2", "COM2"));

    SensorData testSensorData3("Sensor2", "COM0");

    QVERIFY(SensorUtility::checkUniqueName(testSensorData3.sensor_name, testDataList, testSensorData3.sensor_guid) == false);

    testDataList.clear();
    testDataList.push_back(testSensorData3);

    QVERIFY(SensorUtility::checkUniqueName(testSensorData3.sensor_name, testDataList, testSensorData3.sensor_guid) == true);
}

void SensorUtilityTest::checkUniqueSerialPort()
{
    QVector<SensorData> testDataList;
    testDataList.push_back(SensorData("Sensor1", "COM1"));
    testDataList.push_back(SensorData("Sensor2", "COM2"));
    testDataList.push_back(SensorData("Sensor3", "COM3"));
    testDataList.push_back(SensorData("Sensor4", INVALID_SERIAL_PORT));

    SensorData testSensorData1("Sensor5", "COM1");
    SensorData testSensorData2("Sensor6", "COM4");
    SensorData testSensorData3("Sensor7", INVALID_SERIAL_PORT);

    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData1.sensor_portName, testDataList, testSensorData1.sensor_guid) == false);
    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData2.sensor_portName, testDataList, testSensorData2.sensor_guid) == true);
    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData3.sensor_portName, testDataList, testSensorData3.sensor_guid) == true);

    testDataList.clear();
    testDataList.push_back(SensorData("Sensor2", "COM2"));

    SensorData testSensorData4("Sensor3", "COM2");

    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData4.sensor_portName, testDataList, testSensorData4.sensor_guid) == false);

    testDataList.clear();
    testDataList.push_back(testSensorData4);

    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData4.sensor_portName, testDataList, testSensorData4.sensor_guid) == true);
}

QTEST_MAIN(SensorUtilityTest)

#include "SensorUtilityTest.moc"
