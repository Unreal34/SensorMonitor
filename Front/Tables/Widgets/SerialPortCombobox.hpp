#ifndef SERIALPORTCOMBOBOX_HPP
#define SERIALPORTCOMBOBOX_HPP

#include <QCombobox>

class SerialPortCombobox : public QComboBox
{
    Q_OBJECT
public:
    SerialPortCombobox(QWidget* parent = nullptr);
};

#endif // SERIALPORTCOMBOBOX_HPP
