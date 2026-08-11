#include "SensorsEditorWidget.hpp"
#include "SensorData.hpp"
#include "ui_SensorsEditorWidget.h"
#include <quuid.h>

SensorsEditorWidget::SensorsEditorWidget(QWidget *parent) : QWidget(parent)
    , ui(new Ui::SensorsEditorWidget)
    , mSensorsTableView(new SensorsTableView(this))
{
    ui->setupUi(this);

    ui->mMainLayout->insertWidget(0, mSensorsTableView);

    connect(ui->mAdd, SIGNAL(clicked(bool)), this, SLOT(openNewSensorDialog()));
    connect(ui->mDelete, SIGNAL(clicked(bool)), this, SLOT(deleteCurrentSensor()));
}

SensorsEditorWidget::~SensorsEditorWidget()
{
    delete ui;
}

void SensorsEditorWidget::openNewSensorDialog()
{
    QVariantList data;

    SensorData s1;

    s1.sensor_name = QUuid::createUuid().toString(QUuid::WithoutBraces);
    s1.sensor_portName = "COM3";

    data.push_back(QVariant::fromValue(s1));

    mSensorsTableView->append(data, 0);
}

void SensorsEditorWidget::deleteCurrentSensor()
{

}
