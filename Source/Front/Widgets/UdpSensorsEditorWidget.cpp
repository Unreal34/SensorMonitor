#include "UdpSensorsEditorWidget.hpp"
#include "Front/Tables/View/UdpSensorsTableView.hpp"
#include "Source/Front/Widgets/ui_BaseSensorsEditorWidget.h"

UdpSensorsEditorWidget::UdpSensorsEditorWidget(const QVector<UdpSensorData>& sensors, QWidget *parent) : BaseSensorsEditorWidget { parent }
, mSensorsTableView(new UdpSensorsTableView(parent))
{
    ui->mMainLayout->insertWidget(0, mSensorsTableView);

    QVariantList datalist;

    Q_FOREACH(const UdpSensorData& data, sensors)
    {
        datalist.push_back(QVariant::fromValue(data));
    }

    mSensorsTableView->clearAndFill(datalist, 0);
}

QVector<UdpSensorData> UdpSensorsEditorWidget::updatedSensorData()
{
    return mSensorsTableView->udpDataList();
}

void UdpSensorsEditorWidget::addNewSensor()
{
    QVariantList data;

    UdpSensorData s1;
    s1.sensor_name = s1.sensor_guid.toString(QUuid::WithoutBraces);
    s1.sensor_udpPort = 5000;
    s1.sensor_sender_ipAddress = QHostAddress("192.168.1.63");
    data.push_back(QVariant::fromValue(s1));

    mSensorsTableView->append(data, 0);
}

void UdpSensorsEditorWidget::deleteCurrentSelectedSensor()
{
    mSensorsTableView->deleteSelectedSensor();
}
