#ifndef SERIALPORTCOMBOBOX_HPP
#define SERIALPORTCOMBOBOX_HPP

#include <QCombobox>

/**
 * @brief Retrieve a list of serial port available on this machine then fill the combobox.
 */
class SerialPortCombobox : public QComboBox
{
    Q_OBJECT
public:
    SerialPortCombobox(QWidget* parent = nullptr);
};

#endif // SERIALPORTCOMBOBOX_HPP
