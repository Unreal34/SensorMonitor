#ifndef SENSORSTABLEVIEW_HPP
#define SENSORSTABLEVIEW_HPP

#include <QObject>
#include "BaseDataTableView.hpp"
#include "SensorData.hpp"

class SensorsTableView : public BaseDataTableView
{
    Q_OBJECT
public:
    explicit SensorsTableView(QWidget *parent = nullptr);

public:
    inline SensorData selectedSensor() const { return mSelectedSensor; };

protected:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

signals:
    void newSensorSelected(const SensorData& sensor);

private:

    SensorData mSelectedSensor;
};

#endif // SENSORSTABLEVIEW_HPP
