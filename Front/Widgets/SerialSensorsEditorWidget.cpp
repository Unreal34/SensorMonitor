#include "SerialSensorsEditorWidget.hpp"
#include "SerialSensorData.hpp"
#include "Utility.hpp"
#include "ui_SerialSensorsEditorWidget.h"
#include <quuid.h>

SerialSensorsEditorWidget::SerialSensorsEditorWidget(const QVector<SerialSensorData>& sensors, QWidget *parent) : QWidget(parent)
, ui(new Ui::SerialSensorsEditorWidget)
, mSensorsTableView(new SerialSensorsTableView(this))
{
    ui->setupUi(this);
    ui->mMainLayout->insertWidget(0, mSensorsTableView);

    connect(ui->mAdd, SIGNAL(clicked(bool)), this, SLOT(addNewSensor()));
    connect(ui->mDelete, SIGNAL(clicked(bool)), this, SLOT(deleteCurrentSelectedSensor()));

    QVariantList datalist;

    Q_FOREACH(const SerialSensorData& data, sensors)
    {
        datalist.push_back(QVariant::fromValue(data));
    }

    mSensorsTableView->clearAndFill(datalist, 0);
}

SerialSensorsEditorWidget::~SerialSensorsEditorWidget()
{
    delete ui;
}

QVector<SerialSensorData> SerialSensorsEditorWidget::updatedSensorData()
{
    return mSensorsTableView->sensorDataList();
}

void SerialSensorsEditorWidget::addNewSensor()
{
    QVariantList data;

    SerialSensorData s1;
    s1.sensor_name = s1.sensor_guid.toString(QUuid::WithoutBraces);
    s1.sensor_portName = INVALID_SERIAL_PORT;

    data.push_back(QVariant::fromValue(s1));

    mSensorsTableView->append(data, 0);
}

void SerialSensorsEditorWidget::deleteCurrentSelectedSensor()
{
    mSensorsTableView->deleteSelectedSensor();
}
