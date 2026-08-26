#ifndef SIMULATEDDEVICE_H
#define SIMULATEDDEVICE_H

#include <QIODevice>
#include <QObject>

class SimulatedDevice : public QIODevice
{
    Q_OBJECT

public:
    explicit SimulatedDevice(QObject* parent = nullptr);
    ~SimulatedDevice();

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

#endif // SIMULATEDDEVICE_H
