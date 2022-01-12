#pragma once

#include "CellWidget.h"
#include <QAction>
#include <QMainWindow>
#include <QMenuBar>

namespace Netlist {

class Cell;
class CellWidget;
class SaveCellDialog;
class OpenCellDialog;
class CellsLib;
class InstancesWidget;

class CellViewer : public QMainWindow {
    Q_OBJECT;

public:
    CellViewer(QWidget* parent = NULL);
    virtual ~CellViewer();
    inline Cell* getCell() const;
    inline CellsLib* getCellsLib(); // TME9+.

public slots:
    void showCellsLib(); // TME9+.
    void showInstancesWidget(); // TME9+.
    void setCell(Cell*);
    void openCell();
    void saveCell();

signals:
    void cellLoaded();

private:
    Cell* cell_;
    CellWidget* cellWidget_;
    SaveCellDialog* saveCellDialog_;
    OpenCellDialog* openCellDialog_;
    CellsLib* cellsLib_; // TME9+.
    InstancesWidget* instancesWidget_;
};

Cell* CellViewer::getCell() const { return cell_; }
CellsLib* CellViewer::getCellsLib() { return cellsLib_; } // TME9+.

} // namespace Netlist
