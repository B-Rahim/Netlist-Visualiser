

#include "CellViewer.h"
#include "Cell.h"
#include "CellsLib.h"
#include "CellsModel.h"
#include "InstancesWidget.h"
#include "OpenCellDialog.h"
#include "SaveCellDialog.h"

namespace Netlist {

CellViewer ::CellViewer(QWidget* parent)
    : QMainWindow(parent)
    , cellWidget_(NULL)
    , saveCellDialog_(NULL)
    , openCellDialog_(NULL)
    , cellsLib_(NULL)
    , instancesWidget_(NULL)
{
    cellWidget_ = new CellWidget();
    saveCellDialog_ = new SaveCellDialog(this);
    openCellDialog_ = new OpenCellDialog(this);
    cellsLib_ = new CellsLib();
    instancesWidget_ = new InstancesWidget();

    setCentralWidget(cellWidget_);

    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* action = new QAction(" &Open Cell", this);
    action->setStatusTip(" Open cell from disk ");
    action->setShortcut(QKeySequence("CTRL+O"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(openCell()));

    action = new QAction(" &Save As", this);
    action->setStatusTip(" Save to disk ( rename ) the Cell");
    action->setShortcut(QKeySequence("CTRL+S"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(saveCell()));

    action = new QAction(" &Cells Library", this);
    action->setStatusTip(" Display table of loaded cells");
    action->setShortcut(QKeySequence("CTRL+L"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));

    action = new QAction(" &Instances", this);
    action->setStatusTip(" Display table of loaded cells");
    action->setShortcut(QKeySequence("CTRL+I"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()));

    action = new QAction("&Quit", this);
    action->setStatusTip("Exit the Netlist Viewer");
    action->setShortcut(QKeySequence("CTRL+Q"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(close()));
    connect(this, &CellViewer::cellLoaded, cellsLib_->getBaseModel(), &CellsModel::updateData);
}
CellViewer::~CellViewer()
{
    delete cellWidget_;
    delete saveCellDialog_;
    delete openCellDialog_;
}

void CellViewer::setCell(Cell* cell)
{
    cell_ = cell;
    cellWidget_->setCell(cell);
    instancesWidget_->setCellViewer(this);
}

void CellViewer ::saveCell()
{
    Cell* cell = getCell();
    if (cell == NULL)
        return;
    QString cellName = cell->getName().c_str();
    if (saveCellDialog_->run(cellName)) {
        cell->setName(cellName.toStdString());
        cell->save(cellName.toStdString());
    }
}

void CellViewer::showCellsLib() // TME9+
{
    cellsLib_->setCellViewer(this);
    cellsLib_->show();
    cellsLib_->raise();
}
void CellViewer::showInstancesWidget() // TME9+.
{
    //instancesWidget_->setCellViewer(this);
    instancesWidget_->show();
    instancesWidget_->raise();
}

void CellViewer::openCell()
{

    QString cellName;
    if (openCellDialog_->run(cellName)) {
        Cell* cell = Cell::find(cellName.toStdString());
        if (cell) {
            std::cout << "cell set" << std::endl;
            setCell(cell);
            return;
        }

        cell = Cell::load(cellName.toStdString());
        if (cell == NULL)
            return;

        std::cout << "cell set" << std::endl;

        emit cellLoaded(); //informing CellsModel
        setCell(cell);
    }
}
} // namespace Netlist
