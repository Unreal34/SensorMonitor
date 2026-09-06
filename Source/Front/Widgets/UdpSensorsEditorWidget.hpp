#ifndef UDPSENSORSEDITORWIDGET_HPP
#define UDPSENSORSEDITORWIDGET_HPP

#include "Back/Structs/UdpSensorData.hpp"
#include "BaseSensorsEditorWidget.hpp"
#include "Front/Tables/View/UdpSensorsTableView.hpp"
#include <QWidget>


class UdpSensorsEditorWidget : public BaseSensorsEditorWidget
{
    Q_OBJECT
public:
    explicit UdpSensorsEditorWidget(const QVector<UdpSensorData>& sensors, QWidget *parent = nullptr);

public:
    QVector<UdpSensorData> updatedSensorData();

protected slots:
    virtual void addNewSensor() override;
    virtual void deleteCurrentSelectedSensor() override;

private:
    UdpSensorsTableView* mSensorsTableView = nullptr;
};

#endif // UDPSENSORSEDITORWIDGET_HPP
