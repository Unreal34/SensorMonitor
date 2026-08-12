#include "SerialPortCombobox.hpp"
#include <QSerialPortInfo>

SerialPortCombobox::SerialPortCombobox(QWidget *parent) : QComboBox{parent}
{
    addItem(QIcon("://Icons/RS232.png"), tr("NONE"), QVariant::fromValue(QSerialPortInfo()));

    for (const QSerialPortInfo &portInfo : QSerialPortInfo::availablePorts())
    {
        addItem(QIcon("://Icons/RS232.png"), portInfo.portName(), QVariant::fromValue(portInfo));
    }
}
