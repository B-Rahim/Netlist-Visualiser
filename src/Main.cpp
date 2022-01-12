// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/7>" -*-

#include "Cell.h"
#include "CellViewer.h"
#include "Instance.h"
#include "InstancesWidget.h"
#include "Net.h"
#include "Term.h"
#include <QApplication>
#include <QString>
#include <QtGui>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using namespace std;
using namespace Netlist;

int main(int argc, char* argv[])
{
    cout << "Chargement des modeles..." << endl;
    Cell::load("vdd");
    Cell::load("gnd");
    Cell::load("TransistorN");
    Cell::load("TransistorP");
    Cell::load("and2");
    Cell::load("or2");
    Cell::load("xor2");

    QApplication* qa = new QApplication(argc, argv);

    CellViewer* viewer = new CellViewer();
    viewer->show();

    int rvalue = qa->exec();
    delete qa;

    return rvalue;
}