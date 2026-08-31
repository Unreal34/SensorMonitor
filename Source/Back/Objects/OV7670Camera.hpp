#ifndef OV767_CAMERA_CPP
#define OV767_CAMERA_CPP

#include "SerialSensor.hpp"

class OV7670Camera : public SerialSensor
{
public:
    explicit OV7670Camera(const QString& portName, QObject *parent = nullptr);
    explicit OV7670Camera(QIODevice* simulatedDevice, QObject *parent = nullptr);

private:
    void processBuffer();

protected slots:
    virtual void onDataReceived() override;

private:
    QByteArray mBuffer;
};

#endif // OV767_CAMERA_CPP
