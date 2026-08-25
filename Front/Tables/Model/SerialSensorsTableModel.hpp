#ifndef SERIALSENSORSTABLEMODEL_H
#define SERIALSENSORSTABLEMODEL_H

#include <QObject>
#include "BaseDataTableModel.hpp"

class SerialSensorsTableModel : public BaseDataTableModel
{
    Q_OBJECT
public :
    enum Column
    {
        SensorName,
        SerialPortName,
        Count,
    };

public:
    explicit SerialSensorsTableModel(QObject *parent = nullptr);

public:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // SERIALSENSORSTABLEMODEL_H
