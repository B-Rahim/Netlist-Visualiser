#include "CellsLib.h"
#include "CellViewer.h"
#include "CellsModel.h"
namespace Netlist {

CellsLib ::CellsLib(QWidget* parent)
    : QWidget(parent)
    , cellViewer_(NULL)
    , baseModel_(new CellsModel(this))
    , view_(new QTableView(this))
    , load_(new QPushButton(this))
{
    setAttribute(Qt ::WA_QuitOnClose, false);
    setAttribute(Qt ::WA_DeleteOnClose, false);
    setContextMenuPolicy(Qt ::ActionsContextMenu);
    view_->setShowGrid(false);
    view_->setAlternatingRowColors(true);
    view_->setSelectionBehavior(QAbstractItemView ::SelectRows);
    view_->setSelectionMode(QAbstractItemView ::SingleSelection);
    view_->setSortingEnabled(true);
    view_->setModel(baseModel_); // On associe le modele .
    // .
    view_->setMinimumSize(600, 600);

    QHeaderView* horizontalHeader = view_->horizontalHeader();
    horizontalHeader->setDefaultAlignment(Qt ::AlignHCenter);
    horizontalHeader->setMinimumSectionSize(600);
    horizontalHeader->setStretchLastSection(true);
    QHeaderView* verticalHeader = view_->verticalHeader();
    verticalHeader->setVisible(false);
    load_->setText(" Load ");

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(load_);
    hLayout->addStretch();

    QVBoxLayout* vLayout = new QVBoxLayout();

    vLayout->addWidget(view_);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    connect(load_, SIGNAL(clicked()), this, SLOT(load()));
}

int CellsLib ::getSelectedRow() const
{
    QModelIndexList selecteds = view_->selectionModel()
                                    ->selection()
                                    .indexes();
    if (selecteds.empty())
        return -1;
    return selecteds.first().row();
}
void CellsLib ::load()
{
    int selectedRow = getSelectedRow();
    if (selectedRow < 0)
        return;
    cellViewer_->setCell(baseModel_->getModel(selectedRow));
    cellViewer_->show();
}

void CellsLib::setCellViewer(CellViewer* cellv)
{
    cellViewer_ = cellv;
}

} // namespace Netlist
