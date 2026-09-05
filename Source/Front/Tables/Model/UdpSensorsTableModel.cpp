#include "UdpSensorsTableModel.hpp"
#include "Back/Structs/UdpSensorData.hpp"
#include <qicon.h>

UdpSensorsTableModel::UdpSensorsTableModel(QObject *parent) : BaseDataTableModel{parent}
{}

Qt::ItemFlags UdpSensorsTableModel::flags(const QModelIndex &index) const
{
    switch(index.column())
    {
    case Column::SensorName:
    case Column::UdpPort:
    case Column::SenderIpAddress:
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

int UdpSensorsTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : Count;
}

QVariant UdpSensorsTableModel::data(const QModelIndex &index, int role) const
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
                return mData[index.row()].value<UdpSensorData>().sensor_name;
            }

            case Column::UdpPort:
            {
                return mData[index.row()].value<UdpSensorData>().sensor_udpPort;
            }

            case Column::SenderIpAddress:
            {
                return mData[index.row()].value<UdpSensorData>().sensor_sender_ipAddress.toString();
            }
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        switch(index.column())
        {
            case Column::SensorName:
            case Column::UdpPort:
            case Column::SenderIpAddress:
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

QVariant UdpSensorsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
            case Column::SensorName:
            {
                return QString(tr("Sensor name"));
            }

            case Column::UdpPort:
            {
                return QString(tr("Udp port"));
            }

            case Column::SenderIpAddress:
            {
                return QString(tr("IP address"));
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

            case Column::UdpPort:
            {
                return QIcon("://Icons/Ethernet.png");
            }

            case Column::SenderIpAddress:
            {
                return QIcon("://Icons/Network.png");
            }
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignLeft;
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool UdpSensorsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= mData.count())
    {
        return false;
    }

    if(role != Qt::EditRole)
    {
        return false;
    }

    UdpSensorData data = mData[index.row()].value<UdpSensorData>();

    switch(index.column())
    {
    case Column::SensorName:
    {
        data.sensor_name = value.toString();
        break;
    }

    case Column::UdpPort:
    {
        bool bOk = false;
        quint16 port = value.toUInt(&bOk);
        Q_ASSERT(bOk);
        data.sensor_udpPort = port;
        break;
    }

    case Column::SenderIpAddress:
    {
        QHostAddress address;
        bool bSuccess = address.setAddress(value.toString());
        Q_ASSERT(bSuccess);
        data.sensor_sender_ipAddress = address;
        break;
    }

    default:
        return false;
    }

    mData[index.row()] = QVariant::fromValue(data);

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return true;
}
