#ifndef SERIALSENSOR_H
#define SERIALSENSOR_H

#include <QObject>
#include <QSerialPort>

class SerialSensor : public QObject
{
    Q_OBJECT
public:
    SerialSensor(const QString& portName, QObject *parent = nullptr);
    ~SerialSensor();

public:
    inline bool isAvailable() const { return mSerialPort->isOpen(); }

public:
    inline void setName(const QString& newName) { mName = newName; };
    inline const QString& name() const { return mName; };
    inline QString serialPortName() const { return mSerialPort->portName(); };

private slots:
    void onSerialDataReceived();

signals:
    void dataReceived(const QByteArray& data);
    void error(const QString& message);

private:
    QSerialPort* mSerialPort = nullptr;
    QString mPortName = {};
    QString mName = {};
};

#endif // SERIALSENSOR_H
