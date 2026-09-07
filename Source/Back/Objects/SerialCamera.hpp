#ifndef SERIALCAMERA_HPP
#define SERIALCAMERA_HPP

#include "SerialSensor.hpp"

class SerialCamera : public SerialSensor
{
public:
    explicit SerialCamera(const QString& portName, QObject *parent = nullptr);
    explicit SerialCamera(QIODevice* simulatedDevice, QObject *parent = nullptr);

protected:
    /**
     * @brief Reads and processes the received frame buffer.
     * @note Must be implemented by subclasses.
     */
    virtual void processBuffer() =0;

protected slots:
    virtual void onDataReceived() override;

protected:
    QByteArray mBuffer = {};
};

#endif // SERIALCAMERA_HPP
