#ifndef BASEDATATABLEVIEW_HPP
#define BASEDATATABLEVIEW_HPP

#include <QTableView>
#include "BaseDataTableModel.hpp"

class BaseDataTableView : public QTableView
{
public:
    explicit BaseDataTableView(QWidget* parent = nullptr);

public:
    BaseDataTableModel* dataTableModel() const;

public slots:
    void clear();

    /**
     * @brief Used to clear and fill the table from a QVariantList.
     * @param data
     * @param selectedIndex
     */
    void clearAndFill(const QVariantList& data, int selectedIndex = 0);

    /**
     * @brief append
     * @param data
     * @param selectedIndex
     */
    void append(const QVariantList& data, int selectedIndex = 0);

    /**
     * @brief Return all entries available in the table model as QVariantList.
     * @return
     */
    const QVariantList& dataList() const { return dataTableModel()->dataList(); }

protected:
    bool mClearing = false;
};

#endif // BASEDATATABLEVIEW_HPP
