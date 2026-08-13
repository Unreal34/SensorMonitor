#include "SerialPortCombobox.hpp"
#include <QSerialPortInfo>

SerialPortCombobox::SerialPortCombobox(QWidget *parent) : QComboBox{parent}
{
    QIcon icon = QIcon("://Icons/RS232.png");

    addItem(icon, tr("NONE"), QVariant::fromValue(QSerialPortInfo()));

    for (const QSerialPortInfo &portInfo : QSerialPortInfo::availablePorts())
    {
        addItem(icon, portInfo.portName(), QVariant::fromValue(portInfo));
    }
}
