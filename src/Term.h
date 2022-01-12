#pragma once

#include "Node.h"

namespace Netlist {
class Cell;
class Instance;

class Term {
public:
    enum Type {
        Internal = 1,
        External = 2
    };
    enum Direction {
        In = 1,
        Out = 2,
        Inout = 3,
        Tristate = 4,
        Transcv = 5,
        Unknown = 6
    };

public:
    Term(Cell*, const std ::string& name, Direction);
    Term(Instance*, const Term* modelTerm);
    ~Term();
    inline bool isInternal() const;
    inline bool isExternal() const;
    inline const std ::string& getName() const;
    inline Node* getNode();
    inline Net* getNet() const;
    inline Cell* getCell() const;
    Cell* getOwnerCell() const;
    inline Instance* getInstance() const;
    inline Direction getDirection() const;
    inline Point getPosition() const;
    inline Type getType() const;
    void setNet(Net*);
    void setNet(const std ::string&);
    void setPosition(const Point&);
    void setPosition(int x, int y);
    void setDirection(Direction);
    void toXml(std::ostream&);
    static Term* fromXml(Cell*, xmlTextReaderPtr reader);

private:
    void* owner_;
    std ::string name_;
    Direction direction_;
    Type type_;
    Net* net_;
    NodeTerm node_;
};

const std::string& Term::getName() const { return name_; }
Term::Direction Term::getDirection() const { return direction_; }
Point Term::getPosition() const { return node_.getPosition(); }
Term::Type Term::getType() const { return type_; }
Node* Term::getNode() { return &node_; }
Net* Term::getNet() const { return net_; }
bool Term::isInternal() const { return type_ == Internal; }
bool Term::isExternal() const
{
    return type_ == External;
    ;
}

Cell* Term ::getCell() const
{
    return (type_ == External) ? static_cast<Cell*>(owner_)
                               : NULL;
}
Instance* Term ::getInstance() const
{
    return (type_ == Internal) ? static_cast<Instance*>(owner_)
                               : NULL;
}

std::string convertT(Term::Type t);
std::string convertD(Term::Direction d);

} // namespace Netlist