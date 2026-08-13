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
     * @brief Returns the list of SensorData updated by the user through the editor.
     * @return
     */
    QVector<SensorData> updatedSensorData();

private slots:

    /**
     * @brief Adds a new sensor entry to the table with a default name and no serial port selected.
     */
    void addNewSensor();

    /**
     * @brief Deletes the currently selected sensor entry if it is valid.
     */
    void deleteCurrentSelectedSensor();

private:
    Ui::SensorsEditorWidget *ui;
    SensorsTableView* mSensorsTableView = nullptr;
};

#endif // SENSORSEDITORWIDGET_HPP
