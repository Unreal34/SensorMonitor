#ifndef SENSORSEDITORDIALOG_H
#define SENSORSEDITORDIALOG_H

#include <QObject>
#include "BaseDialog.hpp"
#include "SensorsEditorWidget.hpp"

class SensorsEditorDialog : public BaseDialog
{
public:
    explicit SensorsEditorDialog(bool *ok, QWidget* parent = nullptr);

private:
    SensorsEditorWidget* mSensorsEditorWidget = nullptr;
};

#endif // SENSORSEDITORDIALOG_H
