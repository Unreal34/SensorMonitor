#ifndef ESP32CAMERA_HPP
#define ESP32CAMERA_HPP

#include <QObject>
#include "Back/Objects/SerialCamera.hpp"

class ESP32Camera : public SerialCamera
{
public:
    explicit ESP32Camera(const QString& portName, QObject *parent = nullptr);
    explicit ESP32Camera(QIODevice* simulatedDevice, QObject *parent = nullptr);

protected:
    virtual void processBuffer() override;
};
#endif // ESP32CAMERA_HPP
