#ifndef OV767_CAMERA_CPP
#define OV767_CAMERA_CPP

#include "Back/Objects/SerialCamera.hpp"

class OV7670Camera : public SerialCamera
{
public:
    explicit OV7670Camera(const QString& portName, QObject *parent = nullptr);
    explicit OV7670Camera(QIODevice* simulatedDevice, QObject *parent = nullptr);

protected:
    virtual void processBuffer() override;
};

#endif // OV767_CAMERA_CPP
