#ifndef SERIAL_ESP32CAMERA_H
#define SERIAL_ESP32CAMERA_H

#include <QObject>
#include "Back/Objects/SerialCamera.hpp"

class Serial_ESP32Camera : public SerialCamera
{
public:
    explicit Serial_ESP32Camera(const QString& portName, QObject *parent = nullptr);
    explicit Serial_ESP32Camera(QIODevice* simulatedDevice, QObject *parent = nullptr);

public:
    virtual bool open(QIODeviceBase::OpenModeFlag flag = QIODeviceBase::ReadOnly) override;

protected:
    virtual void processBuffer() override;
};
#endif // SERIAL_ESP32CAMERA_H
