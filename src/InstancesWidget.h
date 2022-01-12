#pragma once

#include "InstancesModel.h"
#include <QHeaderView>
#include <QPushButton>
#include <QTableView>
#include <QWidget>

namespace Netlist {
class Cell;
class CellViewer;
class InstancesModel;

class InstancesWidget : public QWidget {
    Q_OBJECT;

public:
    InstancesWidget(QWidget* parent = NULL);
    void setCellViewer(CellViewer*);
    int getSelectedRow() const;
    inline void setCell(Cell*);

public slots:
    void load();

private:
    CellViewer* cellViewer_;
    InstancesModel* baseModel_;
    QTableView* view_;
    QPushButton* load_;
};
void InstancesWidget::setCell(Cell* cell)
{
    baseModel_->InstancesModel::setCell(cell);
}

} // namespace Netlist
