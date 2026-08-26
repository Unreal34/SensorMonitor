#include "SerialSensorsTableModel.hpp"
#include "Back/Structs/SerialSensorData.hpp"

#include <QIcon>

SerialSensorsTableModel::SerialSensorsTableModel(QObject *parent) : BaseDataTableModel{parent}
{}

Qt::ItemFlags SerialSensorsTableModel::flags(const QModelIndex &index) const
{
    switch(index.column())
    {
        case Column::SensorName:
        case Column::SerialPortName:
        {
            if(mIsEditable)
            {
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

            }
            else
            {
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
            }
        }
    }

    return Qt::ItemIsEnabled;
}

int SerialSensorsTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : Count;
}

QVariant SerialSensorsTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= mData.count())
    {
        return QVariant();
    }

   if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
            case Column::SensorName:
            {
                return mData[index.row()].value<SerialSensorData>().sensor_name;
            }

            case Column::SerialPortName:
            {
                return mData[index.row()].value<SerialSensorData>().sensor_portName;
            }
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        switch(index.column())
        {
            case Column::SensorName:
            {
                return Qt::AlignCenter;
            }

            case Column::SerialPortName:
            {
                return Qt::AlignCenter;
            }
        }
    }
    else if(role == ValueType)
    {
        return mData[index.row()];
    }

    return QVariant();
}

QVariant SerialSensorsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
            case Column::SensorName:
            {
                return QString(tr("Sensor name"));
            }

            case Column::SerialPortName:
            {
                return QString(tr("Serial port"));
            }
        }
    }
    else if(role == Qt::DecorationRole)
    {
        switch (section)
        {
            case Column::SensorName:
            {
                return QIcon("://Icons/Sensor.png");
            }

            case Column::SerialPortName:
            {
                return QIcon("://Icons/RS232.png");
            }
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignLeft;
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool SerialSensorsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= mData.count())
    {
        return false;
    }

    if(role != Qt::EditRole)
    {
        return false;
    }

    SerialSensorData data = mData[index.row()].value<SerialSensorData>();

    switch(index.column())
    {
        case Column::SensorName:
        {
            data.sensor_name = value.toString();
            break;
        }

        case Column::SerialPortName:
        {
            data.sensor_portName = value.toString();
            break;
        }

        default:
            return false;
    }

    mData[index.row()] = QVariant::fromValue(data);

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return true;
}
