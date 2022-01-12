#pragma once

#include "Term.h"

namespace Netlist {

class Cell;
class Node;

class Net {

public:
    Net(Cell*, const std ::string& name, Term ::Type dir);
    Net(Instance*, const std ::string& name, Term ::Type dir);
    ~Net();
    Cell* getCell() const;
    const std ::string& getName() const;
    unsigned int getId() const;
    Term ::Type getType() const;
    Node* getNode(size_t id) const;
    const std ::vector<Node*>& getNodes() const;
    size_t getFreeNodeId() const;
    void add(Node*);
    bool remove(Node*);
    void add(Line*);
    bool remove(Line*);
    inline const std::vector<Line*>& getLines() const;
    void toXml(std::ostream&);
    static Net* fromXml(Cell*, xmlTextReaderPtr reader);

private:
    Net(const Net&);

private:
    Cell* owner_;
    std ::string name_;
    unsigned int id_;
    Term ::Type type_;
    std ::vector<Node*> nodes_;
    std ::vector<Line*> lines_;
};

inline const std::vector<Line*>& Net::getLines() const { return lines_; }

} // namespace Netlist