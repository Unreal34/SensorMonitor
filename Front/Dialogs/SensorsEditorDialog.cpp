#include "SensorsEditorDialog.hpp"
#include "SensorData.hpp"

SensorsEditorDialog::SensorsEditorDialog(bool *ok, QWidget* parent) : BaseDialog(ok, tr("Validate"), tr("Cancel"), parent)
    , mSensorsEditorWidget(new SensorsEditorWidget(this))
{
    setWindowTitle(tr("Sensors and ports"));
    resize(600, 400);
    containLayout()->addWidget(mSensorsEditorWidget);
}
