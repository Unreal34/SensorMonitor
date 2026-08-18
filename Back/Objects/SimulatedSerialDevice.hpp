#ifndef SIMULATEDSERIALDEVICE_HPP
#define SIMULATEDSERIALDEVICE_HPP

#include <QIODevice>
#include <QObject>

class SimulatedSerialDevice : public QIODevice
{
    Q_OBJECT

public:
    explicit SimulatedSerialDevice(QObject* parent = nullptr);
    ~SimulatedSerialDevice();

public:
    void simulateIncomingData(const QByteArray& data);

protected:

    /**
     * @brief readData
     * @param data must be filled from the mData QByteArray.
     * @param maxSize
     * @return
     */
    virtual qint64 readData(char* data, qint64 maxSize) override;
    virtual qint64 writeData(const char* data, qint64 maxSize) override;
    virtual bool canReadLine() const override;

private:
    QByteArray mData;
};

#endif // SIMULATEDSERIALDEVICE_HPP
