#ifndef SENSORSEDITORWIDGET_H
#define SENSORSEDITORWIDGET_H

#include "Back/Structs/SerialSensorData.hpp"
#include "Front/Tables/View/SerialSensorsTableView.hpp"
#include "BaseSensorsEditorWidget.hpp"
#include <QWidget>

class SerialSensorsEditorWidget : public BaseSensorsEditorWidget
{
    Q_OBJECT

public:
    explicit SerialSensorsEditorWidget(const QVector<SerialSensorData>& sensors, QWidget *parent = nullptr);

public:
    /**
     * @brief Returns the list of SerialSensorData updated by the user through the editor.
     * @return
     */
    QVector<SerialSensorData> updatedSensorData();

protected slots:
    virtual void addNewSensor() override;
    virtual void deleteCurrentSelectedSensor() override;

private:
    SerialSensorsTableView* mSensorsTableView = nullptr;
};

#endif // SENSORSEDITORWIDGET_H
