#include "SensorsEditorWidget.hpp"
#include "SensorData.hpp"
#include "ui_SensorsEditorWidget.h"
#include <quuid.h>

SensorsEditorWidget::SensorsEditorWidget(const QVector<SensorData>& sensors, QWidget *parent) : QWidget(parent)
, ui(new Ui::SensorsEditorWidget)
, mSensorsTableView(new SensorsTableView(this))
{
    ui->setupUi(this);
    ui->mMainLayout->insertWidget(0, mSensorsTableView);

    connect(ui->mAdd, SIGNAL(clicked(bool)), this, SLOT(addNewSensor()));
    connect(ui->mDelete, SIGNAL(clicked(bool)), this, SLOT(deleteCurrentSelectedSensor()));

    QVariantList datalist;

    Q_FOREACH(const SensorData& data, sensors)
    {
        datalist.push_back(QVariant::fromValue(data));
    }

    mSensorsTableView->clearAndFill(datalist, 0);
}

SensorsEditorWidget::~SensorsEditorWidget()
{
    delete ui;
}

QVector<SensorData> SensorsEditorWidget::updatedSensorData()
{
    return mSensorsTableView->sensorDataList();
}

void SensorsEditorWidget::addNewSensor()
{
    QVariantList data;

    SensorData s1;
    s1.sensor_name = s1.sensor_guid.toString(QUuid::WithoutBraces);
    s1.sensor_portName = "NONE";

    data.push_back(QVariant::fromValue(s1));

    mSensorsTableView->append(data, 0);
}

void SensorsEditorWidget::deleteCurrentSelectedSensor()
{
    mSensorsTableView->deleteSelectedSensor();
}
