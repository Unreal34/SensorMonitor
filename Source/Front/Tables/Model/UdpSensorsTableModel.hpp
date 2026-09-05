#ifndef UDPSENSORSTABLEMODEL_HPP
#define UDPSENSORSTABLEMODEL_HPP

#include "BaseDataTableModel.hpp"

class UdpSensorsTableModel : public BaseDataTableModel
{
    Q_OBJECT
public :
    enum Column
    {
        SensorName,
        UdpPort,
        SenderIpAddress,
        Count,
    };

public:
    explicit UdpSensorsTableModel(QObject *parent = nullptr);

public:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // UDPSENSORSTABLEMODEL_HPP
