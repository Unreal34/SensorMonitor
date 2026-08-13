#include "SensorsItemDelegate.hpp"
#include "Application.hpp"
#include "SensorData.hpp"
#include "SensorsTableModel.hpp"
#include "SerialPortCombobox.hpp"
#include <QTextOption>
#include <QPainter>
#include <qserialportinfo.h>
#include <SensorUtility.hpp>
#include <QMessageBox>
#include <QLineEdit>

SensorsItemDelegate::SensorsItemDelegate(QObject *parent) : QItemDelegate{parent}
{}

QWidget *SensorsItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget* editor = nullptr;

    if(index.column() == SensorsTableModel::Column::SensorName)
    {
        QWidget* editor = new QLineEdit(parent);
        editor->setAutoFillBackground(true);
        return editor;
    }
    else if(index.column() == SensorsTableModel::Column::SerialPortName)
    {
        QWidget* editor = new SerialPortCombobox(parent);

        SerialPortCombobox* combobox = qobject_cast<SerialPortCombobox*>(editor);
        Q_ASSERT(combobox);

        editor->setAutoFillBackground(true);
        return editor;
    }
    else
    {
        editor = QItemDelegate::createEditor(parent, option, index);
    }

    return editor;
}

void SensorsItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == SensorsTableModel::Column::SensorName)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT(lineEdit);

        QString sensorName = index.model()->data(index, Qt::EditRole).toString();
        lineEdit->setText(sensorName);
    }
    else if(index.column() == SensorsTableModel::Column::SerialPortName)
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

void SensorsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // Retrieve model.
    SensorsTableModel* sensorsTableModel = qobject_cast<SensorsTableModel*>(model);
    Q_ASSERT(sensorsTableModel);

    // Retrieve all available sensor data.
    QVector<SensorData> sensorData;
    bool bSuccess = SensorUtility::variantListToSensorDataList(sensorsTableModel->dataList(), sensorData);
    Q_ASSERT(bSuccess);

    // Retrieve the current sensor data linked to the current table entry (index).
    QVariant variant = model->data(index, BaseDataTableModel::ValueType);
    Q_ASSERT(variant.canConvert<SensorData>());
    SensorData currentSensorData = variant.value<SensorData>();

    if(index.column() == SensorsTableModel::Column::SensorName)
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
    else if(index.column() == SensorsTableModel::Column::SerialPortName)
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

void SensorsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}

void SensorsItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool SensorsItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QItemDelegate::editorEvent(event, model, option, index);
}
