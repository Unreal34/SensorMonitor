#include <QtTest>

#include "Back/Utility/SensorUtility.hpp"
#include "Back/Utility/Utility.hpp"

class SensorUtilityTest : public QObject
{
    Q_OBJECT

private slots:
    void uniqueName();
    void uniqueSerialPort();
    void uniqueIpAddress();
    void variantListToSensorDataList();
    void variantListToUdpDataList();
    void randomSensorName();
    void createGrayscaleImage_pixelData();
    void createGrayscaleImage();
    void createGrayscaleImage_data();
};

void SensorUtilityTest::uniqueName()
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

void SensorUtilityTest::uniqueSerialPort()
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

void SensorUtilityTest::uniqueIpAddress()
{
    QVector<UdpSensorData> testDataList;
    testDataList.push_back(UdpSensorData("Sensor1", 5000, QHostAddress("192.168.1.62")));
    testDataList.push_back(UdpSensorData("Sensor2", 5001, QHostAddress("192.168.1.63")));
    testDataList.push_back(UdpSensorData("Sensor3", 5002, QHostAddress("192.168.1.64")));
    testDataList.push_back(UdpSensorData("Sensor4", 5003, QHostAddress(ANY_IP_ADDRESS)));

    UdpSensorData testSensorData1("Sensor5", 5004, QHostAddress("192.168.1.62"));
    UdpSensorData testSensorData2("Sensor6", 5005, QHostAddress("192.168.1.65"));
    UdpSensorData testSensorData3("Sensor7", 5006, QHostAddress(ANY_IP_ADDRESS));

    QVERIFY(SensorUtility::checkUniqueSenderIpAddress(testSensorData1.sensor_sender_ipAddress, testDataList, testSensorData1.sensor_guid) == false);
    QVERIFY(SensorUtility::checkUniqueSenderIpAddress(testSensorData2.sensor_sender_ipAddress, testDataList, testSensorData2.sensor_guid) == true);
    QVERIFY(SensorUtility::checkUniqueSenderIpAddress(testSensorData3.sensor_sender_ipAddress, testDataList, testSensorData3.sensor_guid) == true);

    testDataList.clear();
    testDataList.push_back(UdpSensorData("Sensor2", 5001, QHostAddress("192.168.1.63")));

    UdpSensorData testSensorData4("Sensor3", 5001, QHostAddress("192.168.1.63"));

    QVERIFY(SensorUtility::checkUniqueSenderIpAddress(testSensorData4.sensor_sender_ipAddress, testDataList, testSensorData4.sensor_guid) == false);

    testDataList.clear();
    testDataList.push_back(testSensorData4);

    QVERIFY(SensorUtility::checkUniqueSenderIpAddress(testSensorData4.sensor_sender_ipAddress, testDataList, testSensorData4.sensor_guid) == true);
}

void SensorUtilityTest::variantListToSensorDataList()
{
    QVariantList variantList;
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor1", "COM1")));
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor2", "COM2")));
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor3", "COM3")));
    variantList.push_back(QVariant::fromValue(SerialSensorData("Sensor4", "COM4")));

    QVector<SerialSensorData> dataList;
    QVERIFY(SensorUtility::variantListToSensorDataList<SerialSensorData>(variantList, dataList) == true);
    QVERIFY(dataList.size() == 4);

    variantList.push_back(QVariant::fromValue(QString("Test")));
    QVERIFY(SensorUtility::variantListToSensorDataList<SerialSensorData>(variantList, dataList) == false);
}

void SensorUtilityTest::variantListToUdpDataList()
{
    QVariantList variantList;
    variantList.push_back(QVariant::fromValue(UdpSensorData("Sensor1", 5000, QHostAddress("192.168.1.62"))));
    variantList.push_back(QVariant::fromValue(UdpSensorData("Sensor2", 5001, QHostAddress("192.168.1.63"))));
    variantList.push_back(QVariant::fromValue(UdpSensorData("Sensor3", 5002, QHostAddress("192.168.1.64"))));
    variantList.push_back(QVariant::fromValue(UdpSensorData("Sensor4", 5003, QHostAddress("192.168.1.65"))));

    QVector<UdpSensorData> dataList;
    QVERIFY(SensorUtility::variantListToSensorDataList<UdpSensorData>(variantList, dataList) == true);
    QVERIFY(dataList.size() == 4);

    variantList.push_back(QVariant::fromValue(QString("Test")));
    QVERIFY(SensorUtility::variantListToSensorDataList<UdpSensorData>(variantList, dataList) == false);
}

void SensorUtilityTest::randomSensorName()
{
    QString name = SensorUtility::randomSensorName();

    QVERIFY(!QUuid::fromString(name).isNull());
}

// use _data suffix to work with createGrayscaleImage()
void SensorUtilityTest::createGrayscaleImage_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("height");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("valid data") << QByteArray(4, '\x80') << 2 << 2 << true;
    QTest::newRow("data too small") << QByteArray(3, '\x80') << 2 << 2 << false;
}

void SensorUtilityTest::createGrayscaleImage()
{
    QFETCH(QByteArray, data);
    QFETCH(int, width);
    QFETCH(int, height);
    QFETCH(bool, expectedValid);

    QImage image = SensorUtility::createGrayscaleImage(data, width, height);

    QCOMPARE(!image.isNull(), expectedValid);

    if(expectedValid)
    {
        QCOMPARE(image.width(), width);
        QCOMPARE(image.height(), height);
        QCOMPARE(image.format(), QImage::Format_Grayscale8);
    }
}

void SensorUtilityTest::createGrayscaleImage_pixelData()
{
    const QByteArray data = QByteArray::fromRawData("\x00\x40\x80\xFF", 4);
    const QImage image = SensorUtility::createGrayscaleImage(data, 2, 2);

    QVERIFY(!image.isNull());

    QCOMPARE(image.pixel(0, 0), qRgb(0, 0, 0));
    QCOMPARE(image.pixel(1, 0), qRgb(64, 64, 64));
    QCOMPARE(image.pixel(0, 1), qRgb(128, 128, 128));
    QCOMPARE(image.pixel(1, 1), qRgb(255, 255, 255));
}

QTEST_MAIN(SensorUtilityTest)

#include "SensorUtilityTest.moc"
