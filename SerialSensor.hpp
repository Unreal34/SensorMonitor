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
    inline bool isOnline() const { return mSerialPort->isOpen(); }

public:
    void setSerialPort(const QString& newSerialPort);
    inline void setTag(const QString& newTag) { mTag = newTag; };
    inline const QString& tag() const { return mTag; };
    inline QString serialPortName() const { return mSerialPort->portName(); };

private slots:
    void onSerialDataReceived();

signals:
    void dataReceived(const QByteArray& data);
    void error(const QString& message);

private:
    QSerialPort* mSerialPort = nullptr;
    QString mPortName = {};
    QString mTag = {};
};

#endif // SERIALSENSOR_H
