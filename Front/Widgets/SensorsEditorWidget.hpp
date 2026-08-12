#ifndef SENSORSEDITORWIDGET_HPP
#define SENSORSEDITORWIDGET_HPP

#include "SensorsTableView.hpp"
#include <QWidget>

namespace Ui {
class SensorsEditorWidget;
}

class SensorsEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SensorsEditorWidget(const QVector<SensorData>& sensors, QWidget *parent = nullptr);
    ~SensorsEditorWidget();

public:
    /**
     * @brief Return a list of SensorData udpated by the user.
     * @return
     */
    QVector<SensorData> updatedSensorData();

private slots:
    void addNewSensor();
    void deleteCurrentSelectedSensor();

private:
    Ui::SensorsEditorWidget *ui;
    SensorsTableView* mSensorsTableView = nullptr;
};

#endif // SENSORSEDITORWIDGET_HPP
