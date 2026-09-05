#include <QtTest>

#include "Back/Utility/SensorUtility.hpp"
#include "Back/Utility/Utility.hpp"

class SensorUtilityTest : public QObject
{
    Q_OBJECT

private slots:
    void testUniqueName();
    void testUniqueSerialPort();
    void testVariantListToSensorDataList();
    void testRandomSensorName();
};

void SensorUtilityTest::testUniqueName()
{
    QVector<SerialSensorData> testDataList;
    testDataList.push_back(SerialSensorData("Sensor1", "COM1"));
    testDataList.push_back(SerialSensorData("Sensor2", "COM2"));

    SerialSensorData testSensorData1("Sensor2", "COM0");
    SerialSensorData testSensorData2("Sensor3", "COM2");

    QVERIFY(SensorUtility::checkUniqueName(testSensorData1.sensor_name, testDataList, testSensorData1.sensor_guid) == false);
    QVERIFY(SensorUtility::checkUniqueName(testSensorData2.sensor_name, testDataList, testSensorData2.sensor_guid) == true);

    testDataList.clear();
    testDataList.push_back(SerialSensorData("Sensor2", "COM2"));

    SerialSensorData testSensorData3("Sensor2", "COM0");

    QVERIFY(SensorUtility::checkUniqueName(testSensorData3.sensor_name, testDataList, testSensorData3.sensor_guid) == false);

    testDataList.clear();
    testDataList.push_back(testSensorData3);

    QVERIFY(SensorUtility::checkUniqueName(testSensorData3.sensor_name, testDataList, testSensorData3.sensor_guid) == true);
}

void SensorUtilityTest::testUniqueSerialPort()
{
    QVector<SerialSensorData> testDataList;
    testDataList.push_back(SerialSensorData("Sensor1", "COM1"));
    testDataList.push_back(SerialSensorData("Sensor2", "COM2"));
    testDataList.push_back(SerialSensorData("Sensor3", "COM3"));
    testDataList.push_back(SerialSensorData("Sensor4", INVALID_SERIAL_PORT));

    SerialSensorData testSensorData1("Sensor5", "COM1");
    SerialSensorData testSensorData2("Sensor6", "COM4");
    SerialSensorData testSensorData3("Sensor7", INVALID_SERIAL_PORT);

    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData1.sensor_serialPortName, testDataList, testSensorData1.sensor_guid) == false);
    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData2.sensor_serialPortName, testDataList, testSensorData2.sensor_guid) == true);
    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData3.sensor_serialPortName, testDataList, testSensorData3.sensor_guid) == true);

    testDataList.clear();
    testDataList.push_back(SerialSensorData("Sensor2", "COM2"));

    SerialSensorData testSensorData4("Sensor3", "COM2");

    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData4.sensor_serialPortName, testDataList, testSensorData4.sensor_guid) == false);

    testDataList.clear();
    testDataList.push_back(testSensorData4);

    QVERIFY(SensorUtility::checkUniqueSerialPort(testSensorData4.sensor_serialPortName, testDataList, testSensorData4.sensor_guid) == true);
}

void SensorUtilityTest::testVariantListToSensorDataList()
{
    QVariantList variantList;
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor1", "COM1")));
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor2", "COM2")));
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor3", "COM3")));
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor4", "COM4")));

    QVector<SerialSensorData> dataList;
    QVERIFY(SensorUtility::variantListToSensorDataList(variantList, dataList) == true);
    QVERIFY(dataList.size() == 4);

    variantList.push_back(QVariant::fromValue(QString("Test")));
    QVERIFY(SensorUtility::variantListToSensorDataList(variantList, dataList) == false);
}

void SensorUtilityTest::testRandomSensorName()
{
    QString name = SensorUtility::randomSensorName();

    QVERIFY(!QUuid::fromString(name).isNull());
}

QTEST_MAIN(SensorUtilityTest)

#include "SensorUtilityTest.moc"
