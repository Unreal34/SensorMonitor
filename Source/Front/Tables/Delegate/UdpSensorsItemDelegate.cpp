#include "UdpSensorsItemDelegate.hpp"
#include <QMessageBox>
#include <qhostaddress.h>
#include <qlineedit.h>
#include "Back/Structs/UdpSensorData.hpp"
#include "Back/Utility/Application.hpp"
#include "Front/Tables/Model/UdpSensorsTableModel.hpp"
#include <Back/Utility/SensorUtility.hpp>


UdpSensorsItemDelegate::UdpSensorsItemDelegate(QObject *parent) : QItemDelegate { parent }
{}

QWidget *UdpSensorsItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget* editor = nullptr;

    if(index.column() == UdpSensorsTableModel::Column::SensorName)
    {
        editor = new QLineEdit(parent);
        editor->setAutoFillBackground(true);
    }
    else if(index.column() == UdpSensorsTableModel::Column::UdpPort)
    {
        QLineEdit* lineEdit = new QLineEdit(parent);
        lineEdit->setValidator(new QIntValidator(std::numeric_limits<quint16>::min(), std::numeric_limits<quint16>::max(), editor));
        lineEdit->setAutoFillBackground(true);
        editor = lineEdit;
    }
    else if(index.column() == UdpSensorsTableModel::Column::SenderIpAddress)
    {
        QLineEdit* lineEdit = new QLineEdit(parent);
        QRegularExpression regex{R"(^(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}$)"};
        lineEdit->setValidator(new QRegularExpressionValidator(regex, editor));
        lineEdit->setAutoFillBackground(true);
        editor = lineEdit;
    }
    else
    {
        editor = QItemDelegate::createEditor(parent, option, index);
    }

    return editor;
}

void UdpSensorsItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == UdpSensorsTableModel::Column::SensorName)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString sensorName = index.model()->data(index, Qt::EditRole).toString();
        lineEdit->setText(sensorName);
    }
    else if(index.column() == UdpSensorsTableModel::Column::UdpPort)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        bool bSuccess;
        quint16 port = index.model()->data(index, Qt::EditRole).toUInt(&bSuccess);
        Q_ASSERT(bSuccess);

        lineEdit->setText(QString::number(port));
    }
    else if(index.column() == UdpSensorsTableModel::Column::UdpPort)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString address = index.model()->data(index, Qt::EditRole).toString();

        QHostAddress hostAddress;
        bool bSuccess = hostAddress.setAddress(address);
        Q_ASSERT(bSuccess);

        lineEdit->setText(hostAddress.toString());
    }
    else
    {
        QItemDelegate::setEditorData(editor,index);
    }
}

void UdpSensorsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // Retrieve model.
    UdpSensorsTableModel* udpSensorsTableModel = qobject_cast<UdpSensorsTableModel*>(model);
    Q_ASSERT(udpSensorsTableModel);

    // Retrieve all available sensor data.
    QVector<UdpSensorData> sensorData;
    bool bSuccess = SensorUtility::variantListToUdpDataList(udpSensorsTableModel->dataList(), sensorData);
    Q_ASSERT(bSuccess);

    // Retrieve the current sensor data linked to the current table entry (index).
    QVariant variant = model->data(index, BaseDataTableModel::ValueType);
    Q_ASSERT(variant.canConvert<UdpSensorData>());
    UdpSensorData currentSensorData = variant.value<UdpSensorData>();

    if(index.column() == UdpSensorsTableModel::Column::SensorName)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString newName = lineEdit->text();

        if(SensorUtility::checkUniqueName(newName, sensorData, currentSensorData.sensor_guid))
        {
            model->setData(index, newName, Qt::EditRole);
        }
        else
        {
            QMessageBox::critical(nullptr, APPLICATION_NAME, tr("Name is already used by one of the sensor."), QMessageBox::Ok);
            return;
        }
    }
    else if(index.column() == UdpSensorsTableModel::Column::UdpPort)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString newPort = lineEdit->text();

        bool bOk = false;
        quint16 port = newPort.toUShort(&bOk);
        Q_ASSERT(bOk);

        model->setData(index, port, Qt::EditRole);
    }
    else if(index.column() == UdpSensorsTableModel::Column::UdpPort)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString newPort = lineEdit->text();
    }
    else if(index.column() == UdpSensorsTableModel::Column::SenderIpAddress)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString newIpAddress = lineEdit->text();

        QHostAddress newHostAddress;
        bool bSuccess = newHostAddress.setAddress(newIpAddress);
        Q_ASSERT(bSuccess);

        if(SensorUtility::checkUniqueSenderIpAddress(newHostAddress, sensorData, currentSensorData.sensor_guid))
        {
            model->setData(index, newIpAddress, Qt::EditRole);
        }
        else
        {
            QMessageBox::critical(nullptr, APPLICATION_NAME, tr("IP address already used by a sensor."), QMessageBox::Ok);
            return;
        }
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void UdpSensorsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}

void UdpSensorsItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool UdpSensorsItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QItemDelegate::editorEvent(event, model, option, index);
}
