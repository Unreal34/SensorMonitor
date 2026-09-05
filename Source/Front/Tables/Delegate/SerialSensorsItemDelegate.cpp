#include "SerialSensorsItemDelegate.hpp"
#include "Back/Utility/Application.hpp"
#include "Back/Structs/SerialSensorData.hpp"
#include "Front/Tables/Model/SerialSensorsTableModel.hpp"
#include "Front/Tables/Widgets/SerialPortCombobox.hpp"
#include <QTextOption>
#include <QPainter>
#include <qserialportinfo.h>
#include "Back/Utility/SensorUtility.hpp"
#include <QMessageBox>
#include <QLineEdit>

SerialSensorsItemDelegate::SerialSensorsItemDelegate(QObject *parent) : QItemDelegate { parent }
{}

QWidget *SerialSensorsItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget* editor = nullptr;

    if(index.column() == SerialSensorsTableModel::Column::SensorName)
    {
        editor = new QLineEdit(parent);
        editor->setAutoFillBackground(true);
    }
    else if(index.column() == SerialSensorsTableModel::Column::SerialPortName)
    {
        editor = new SerialPortCombobox(parent);
        SerialPortCombobox* combobox = qobject_cast<SerialPortCombobox*>(editor);
        Q_ASSERT(combobox);
        editor->setAutoFillBackground(true);
    }
    else
    {
        editor = QItemDelegate::createEditor(parent, option, index);
    }

    return editor;
}

void SerialSensorsItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == SerialSensorsTableModel::Column::SensorName)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString sensorName = index.model()->data(index, Qt::EditRole).toString();
        lineEdit->setText(sensorName);
    }
    else if(index.column() == SerialSensorsTableModel::Column::SerialPortName)
    {
        SerialPortCombobox* combobox = qobject_cast<SerialPortCombobox*>(editor);
        Q_ASSERT(combobox);

        QString portName = index.model()->data(index, Qt::EditRole).toString();
        combobox->setCurrentText(portName);
    }
    else
    {
        QItemDelegate::setEditorData(editor,index);
    }
}

void SerialSensorsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // Retrieve model.
    SerialSensorsTableModel* sensorsTableModel = qobject_cast<SerialSensorsTableModel*>(model);
    Q_ASSERT(sensorsTableModel);

    // Retrieve all available sensor data.
    QVector<SerialSensorData> sensorData;
    bool bSuccess = SensorUtility::variantListToSensorDataList(sensorsTableModel->dataList(), sensorData);
    Q_ASSERT(bSuccess);

    // Retrieve the current sensor data linked to the current table entry (index).
    QVariant variant = model->data(index, BaseDataTableModel::ValueType);
    Q_ASSERT(variant.canConvert<SerialSensorData>());
    SerialSensorData currentSensorData = variant.value<SerialSensorData>();

    if(index.column() == SerialSensorsTableModel::Column::SensorName)
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
    else if(index.column() == SerialSensorsTableModel::Column::SerialPortName)
    {
        SerialPortCombobox* combobox = qobject_cast<SerialPortCombobox*>(editor);
        Q_ASSERT(combobox);

        QString newPort = combobox->currentText();

        if(SensorUtility::checkUniqueSerialPort(newPort, sensorData, currentSensorData.sensor_guid))
        {
            model->setData(index, newPort, Qt::EditRole);
        }
        else
        {
            QMessageBox::critical(nullptr, APPLICATION_NAME, tr("Serial port is already used by a sensor."), QMessageBox::Ok);
            return;
        }
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void SerialSensorsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}

void SerialSensorsItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool SerialSensorsItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QItemDelegate::editorEvent(event, model, option, index);
}
