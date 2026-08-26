#include "SerialPortCombobox.hpp"
#include "Back/Utility/Utility.hpp"
#include <QSerialPortInfo>

SerialPortCombobox::SerialPortCombobox(QWidget *parent) : QComboBox{parent}
{
    QIcon icon = QIcon("://Icons/RS232.png");

    addItem(icon, INVALID_SERIAL_PORT, QVariant::fromValue(QSerialPortInfo()));

    for (const QSerialPortInfo &portInfo : QSerialPortInfo::availablePorts())
    {
        addItem(icon, portInfo.portName(), QVariant::fromValue(portInfo));
    }
}
