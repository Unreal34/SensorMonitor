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
    explicit SensorsEditorWidget(QWidget *parent = nullptr);
    ~SensorsEditorWidget();

private slots:
    void openNewSensorDialog();
    void deleteCurrentSensor();

private:
    Ui::SensorsEditorWidget *ui;
    SensorsTableView* mSensorsTableView = nullptr;
};

#endif // SENSORSEDITORWIDGET_HPP
