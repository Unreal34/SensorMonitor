#ifndef SERIALSENSORSEDITORWIDGET_H
#define SERIALSENSORSEDITORWIDGET_H

#include "Back/Structs/SerialSensorData.hpp"
#include "Front/Tables/View/SerialSensorsTableView.hpp"
#include <QWidget>

namespace Ui {
class SerialSensorsEditorWidget;
}

class SerialSensorsEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialSensorsEditorWidget(const QVector<SerialSensorData>& sensors, QWidget *parent = nullptr);
    ~SerialSensorsEditorWidget();

public:
    /**
     * @brief Returns the list of SerialSensorData updated by the user through the editor.
     * @return
     */
    QVector<SerialSensorData> updatedSensorData();

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
    Ui::SerialSensorsEditorWidget *ui;
    SerialSensorsTableView* mSensorsTableView = nullptr;
};

#endif // SERIALSENSORSEDITORWIDGET_H
