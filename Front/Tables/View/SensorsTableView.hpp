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
    /**
     * @brief Delete the current selected entry or do nothing if the selected sensor is invalid.
     */
    void deleteSelectedSensor();

    /**
     * @brief Return a list of SensorData struct. SensorData are converted from the QVariant struct available in the model.
     * @return
     */
    QVector<SensorData> sensorDataList();

protected:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

signals:
    void newSensorSelected(const SensorData& sensor);

private:
    SensorData mSelectedSensor;
};

#endif // SENSORSTABLEVIEW_HPP
