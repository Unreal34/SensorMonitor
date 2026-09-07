#ifndef UDPSENSORSTABLEVIEW_HPP
#define UDPSENSORSTABLEVIEW_HPP

#include "Back/Structs/UdpSensorData.hpp"
#include "BaseDataTableView.hpp"

class UdpSensorsTableView : public BaseDataTableView
{
    Q_OBJECT
public:
    explicit UdpSensorsTableView(QWidget *parent = nullptr);

public:
    QVector<UdpSensorData> udpDataList();
    void deleteSelectedSensor();

protected:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

signals:
    void newSensorSelected(const UdpSensorData& sensor);

private:
    UdpSensorData mSelectedSensor;
};

#endif // UDPSENSORSTABLEVIEW_HPP
