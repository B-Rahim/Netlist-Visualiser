#pragma once

#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

namespace Netlist {
class CellViewer;
class CellsModel;

class CellsLib : public QWidget {
    Q_OBJECT;

public:
    CellsLib(QWidget* parent = NULL);
    void setCellViewer(CellViewer*);
    int getSelectedRow() const;
    inline CellsModel* getBaseModel();

public slots:
    void load();

private:
    CellViewer* cellViewer_;
    CellsModel* baseModel_;
    QTableView* view_;
    QPushButton* load_;
};

CellsModel* CellsLib::getBaseModel() { return baseModel_; }

} // namespace Netlist
