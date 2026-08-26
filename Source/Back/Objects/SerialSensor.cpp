#include "SerialSensor.hpp"
#include "Back/Utility/Utility.hpp"
#include <QUuid>

SerialSensor::SerialSensor(const QString &portName, QObject *parent) : Sensor { parent }
, mPortName(portName)
{
    mType = ESensorType::Serial;

    QSerialPort* serialPort = new QSerialPort(this);
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    mDevice = serialPort;
    Q_ASSERT(mDevice);

    connect(serialPort, &QSerialPort::errorOccurred, this, &SerialSensor::onSerialErrorReceived);
}

SerialSensor::SerialSensor(QIODevice *simulatedDevice, QObject *parent) : Sensor { simulatedDevice, parent }
, mPortName(INVALID_SERIAL_PORT)
{
    mType = ESensorType::Serial;
}

void SerialSensor::onSerialErrorReceived(QSerialPort::SerialPortError error)
{
    QSerialPort* currentSerialPort = serialPort();
    Q_ASSERT(currentSerialPort);

    QString errorString = {};
    ESensorError sensorError = ESensorError::UnhandledError;

    switch (error)
    {
    case QSerialPort::NoError:
        sensorError = ESensorError::Success;
        errorString = tr("No error.");
        break;

    case QSerialPort::DeviceNotFoundError:
        sensorError = ESensorError::Disconnected;
        errorString = tr("The serial device was not found.");
        break;

    case QSerialPort::PermissionError:
        sensorError = ESensorError::OpeningIssue;
        errorString = tr("Permission denied while accessing the serial port.");
        break;

    case QSerialPort::OpenError:
        sensorError = ESensorError::OpeningIssue;
        errorString = tr("The serial port could not be opened.");
        break;

    case QSerialPort::WriteError:
        sensorError = ESensorError::UnhandledError;
        errorString = tr("An error occurred while writing to the serial port.");
        break;

    case QSerialPort::ReadError:
        sensorError = ESensorError::UnhandledError;
        errorString = tr("An error occurred while reading from the serial port.");
        break;

    case QSerialPort::ResourceError:
        sensorError = ESensorError::Disconnected;
        errorString = tr("The serial device has been disconnected or is no longer available.");
        break;

    case QSerialPort::UnsupportedOperationError:
        sensorError = ESensorError::UnhandledError;
        errorString = tr("The requested serial port operation is not supported.");
        break;

    case QSerialPort::UnknownError:
        sensorError = ESensorError::UnhandledError;
        errorString = tr("An unknown serial port error occurred.");
        break;

    case QSerialPort::TimeoutError:
        sensorError = ESensorError::UnhandledError;
        errorString = tr("The serial port operation timed out.");
        break;

    case QSerialPort::NotOpenError:
        sensorError = ESensorError::Disconnected;
        errorString = tr("The serial port is not open.");
        break;
    }

    emit errorHandled(name(), sensorError, errorString);
}
