#include <QDialog>

#include "Cell.h"
#include "CellsModel.h"

namespace Netlist {

CellsModel ::CellsModel(QObject* parent)
    : QAbstractTableModel(parent)
    , cell_(NULL)
{
}
CellsModel ::~CellsModel()
{
}

int CellsModel ::rowCount(const QModelIndex& parent) const
{
    return Cell::getAllCells().size();
}
int CellsModel ::columnCount(const QModelIndex& parent) const
{
    return 1;
}
QVariant CellsModel ::data(const QModelIndex& index, int role) const
{
    if (not index.isValid())
        return QVariant();
    if (role == Qt ::DisplayRole) {
        int row = index.row();
        if (index.column() == 0) {
            return Cell::getAllCells()[row]->getName().c_str();
        }
    }
    return QVariant();
}

QVariant CellsModel ::headerData(int section, Qt ::Orientation orientation, int role) const
{
    if (orientation == Qt ::Vertical)
        return QVariant();
    if (role != Qt ::DisplayRole)
        return QVariant();
    switch (section) {
    case 0:
        return " Cell ";
    }
    return QVariant();
}

Cell* CellsModel ::getModel(int row)
{
    if (row >= (int)Cell::getAllCells().size())
        return NULL;
    return Cell::getAllCells()[row];
}
void CellsModel::updateData()
{
    int row = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), row, row);
    setData(createIndex(row, 0), Cell::getAllCells()[row - 1]->getName().c_str(), Qt::EditRole);
    endInsertRows();
}

} // namespace Netlist
