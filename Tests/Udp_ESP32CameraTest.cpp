#include "Back/Objects/Udp_ESP32Camera.hpp"
#include "Back/Structs/ESP32_PacketHeader.hpp"
#include <QSignalSpy>
#include <QTest>
#include <QUdpSocket>

class Udp_ESP32CameraTest : public QObject
{
    Q_OBJECT
private slots:
    void receiveSinglePacket();
    void receiveMultiplePackets();
    void receivePacketsOutOfOrder();
    void ignorePacketFromWrongSender();
    void ignorePacketWithInvalidFrameSize();
    void ignorePacketWithInvalidPacketCount();
    void ignorePacketWithInvalidPacketIndex();
    void ignorePacketWithInvalidPayloadSize();
    void ignorePacketWithInvalidDatagramSize();
    void ignoreDuplicatePacket();

private:
    QByteArray createPacket(uint32_t frameId, uint32_t frameSize, uint16_t packetIndex, uint16_t packetCount, const QByteArray& payload);
    void sendPacket(QUdpSocket& socket, quint16 port, const QByteArray& packet);
};

QByteArray Udp_ESP32CameraTest::createPacket(uint32_t frameId, uint32_t frameSize, uint16_t packetIndex, uint16_t packetCount, const QByteArray& payload)
{
    UDP_ESP32_PacketHeader header = {};

    header.frameId = frameId;
    header.frameSize = frameSize;
    header.packetIndex = packetIndex;
    header.packetCount = packetCount;
    header.payloadSize = static_cast<uint16_t>(payload.size());

    QByteArray packet;
    packet.resize(sizeof(UDP_ESP32_PacketHeader) + payload.size());
    memcpy(packet.data(), &header, sizeof(UDP_ESP32_PacketHeader));
    memcpy(packet.data() + sizeof(UDP_ESP32_PacketHeader), payload.constData(), payload.size());

    return packet;
}

void Udp_ESP32CameraTest::sendPacket(QUdpSocket& socket, quint16 port, const QByteArray& packet)
{
    const qint64 bytesSent = socket.writeDatagram(packet, QHostAddress::LocalHost, port);

    QCOMPARE(bytesSent, packet.size());
}

///////////////// TESTS ////////////////////////////////

void Udp_ESP32CameraTest::receiveSinglePacket()
{
    const QByteArray payload = QByteArray::fromHex("0102030405060708");

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    const QByteArray packet = createPacket(1, payload.size(), 0, 1, payload);

    sendPacket(sender, camera.udpSocket()->localPort(), packet);

    QTRY_COMPARE(dataReceivedSpy.count(), 1);

    const QList<QVariant> arguments = dataReceivedSpy.takeFirst();

    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments.at(0).toByteArray(), payload);
}

void Udp_ESP32CameraTest::receiveMultiplePackets()
{
    const QByteArray payload1(1400, '\x01');
    const QByteArray payload2(1400, '\x02');
    const QByteArray payload3(100, '\x03');

    const QByteArray frame = payload1 + payload2 + payload3;

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, frame.size(), 0, 3, payload1));
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, frame.size(), 1, 3, payload2));
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, frame.size(), 2, 3, payload3));

    QTRY_COMPARE(dataReceivedSpy.count(), 1);

    const QList<QVariant> arguments = dataReceivedSpy.takeFirst();

    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments.at(0).toByteArray(), frame);
}

void Udp_ESP32CameraTest::receivePacketsOutOfOrder()
{
    const QByteArray payload1(1400, '\x01');
    const QByteArray payload2(1400, '\x02');
    const QByteArray payload3(100, '\x03');

    const QByteArray frame = payload1 + payload2 + payload3;

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    // Send packet 2 first.
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, frame.size(), 1, 3, payload2));

    // Send packet 3.
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, frame.size(), 2, 3, payload3));

    // Send packet 1 last.
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, frame.size(), 0, 3, payload1));

    QTRY_COMPARE(dataReceivedSpy.count(), 1);

    const QList<QVariant> arguments = dataReceivedSpy.takeFirst();

    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments.at(0).toByteArray(), frame);
}

void Udp_ESP32CameraTest::ignorePacketFromWrongSender()
{
    const QByteArray payload(100, '\x01');

    Udp_ESP32Camera camera(0, QHostAddress("127.0.0.1"));

    QVERIFY(camera.open());

    QUdpSocket sender;

    QVERIFY(sender.bind(QHostAddress("127.0.0.2"), 0));

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    const QByteArray packet = createPacket(1, payload.size(), 0, 1, payload);

    sendPacket(sender, camera.udpSocket()->localPort(), packet);

    QTest::qWait(100);

    QCOMPARE(dataReceivedSpy.count(), 0);
}

void Udp_ESP32CameraTest::ignorePacketWithInvalidFrameSize()
{
    const QByteArray payload = QByteArray::fromHex("01020304");

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    // frameSize == 0 is invalid.
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, 0, 0, 1, payload));

    QTest::qWait(100);

    QCOMPARE(dataReceivedSpy.count(), 0);
}

void Udp_ESP32CameraTest::ignorePacketWithInvalidPacketCount()
{
    const QByteArray payload = QByteArray::fromHex("01020304");

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    // packetCount == 0 is invalid.
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, payload.size(), 0, 0, payload));

    QTest::qWait(100);

    QCOMPARE(dataReceivedSpy.count(), 0);
}

void Udp_ESP32CameraTest::ignorePacketWithInvalidPacketIndex()
{
    const QByteArray payload = QByteArray::fromHex("01020304");

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    // packetIndex == packetCount is outside the valid range.
    sendPacket(sender, camera.udpSocket()->localPort(), createPacket(1, payload.size(), 1, 1, payload));

    QTest::qWait(100);

    QCOMPARE(dataReceivedSpy.count(), 0);
}

void Udp_ESP32CameraTest::ignorePacketWithInvalidPayloadSize()
{
    const QByteArray payload = QByteArray::fromHex("01020304");

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    // Create a valid packet first.
    QByteArray packet = createPacket(1, payload.size(), 0, 1, payload);

    UDP_ESP32_PacketHeader corruptedHeader = {};

    // copy header data from packet to corrupted header
    // memcpy(destination, source, numberOfBytes);
    memcpy(&corruptedHeader, packet.constData(), sizeof(UDP_ESP32_PacketHeader));

    // Replace the payload size with zero.
    corruptedHeader.payloadSize = 0;

    // Replace the main header with the corrupted one.
    // memcpy(destination, source, numberOfBytes);
    memcpy(packet.data(), &corruptedHeader, sizeof(UDP_ESP32_PacketHeader));

    sendPacket(sender, camera.udpSocket()->localPort(), packet);

    QTest::qWait(100);

    QCOMPARE(dataReceivedSpy.count(), 0);
}

void Udp_ESP32CameraTest::ignorePacketWithInvalidDatagramSize()
{
    const QByteArray payload = QByteArray::fromHex("01020304");

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    QByteArray packet = createPacket(1, payload.size(), 0, 1, payload);

    // Add an unexpected byte to the datagram.
    packet.append('\xFF');

    sendPacket(sender, camera.udpSocket()->localPort(), packet);

    QTest::qWait(100);

    QCOMPARE(dataReceivedSpy.count(), 0);
}

void Udp_ESP32CameraTest::ignoreDuplicatePacket()
{
    const QByteArray payload1(1400, '\x01');
    const QByteArray payload2(100, '\x02');
    const QByteArray frame = payload1 + payload2;

    Udp_ESP32Camera camera(0, QHostAddress::LocalHost);

    QVERIFY(camera.open());

    QUdpSocket sender;

    QSignalSpy dataReceivedSpy(&camera, &Udp_ESP32Camera::dataReceived);

    const QByteArray packet1 = createPacket(1, frame.size(), 0, 2, payload1);
    const QByteArray packet2 = createPacket(1, frame.size(), 1, 2, payload2);

    // Send packet 1.
    sendPacket(sender, camera.udpSocket()->localPort(), packet1);

    // Send packet 1 again.
    sendPacket(sender, camera.udpSocket()->localPort(), packet1);

    // Send packet 2.
    sendPacket(sender, camera.udpSocket()->localPort(), packet2);

    QTRY_COMPARE(dataReceivedSpy.count(), 1);

    const QList<QVariant> arguments = dataReceivedSpy.takeFirst();

    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments.at(0).toByteArray(), frame);

    // The duplicate packet must not cause a second frame.
    QCOMPARE(dataReceivedSpy.count(), 0);
}

QTEST_MAIN(Udp_ESP32CameraTest)

#include "Udp_ESP32CameraTest.moc"
