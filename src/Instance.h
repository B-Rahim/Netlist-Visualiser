#pragma once

#include "Term.h"

namespace Netlist {
class Cell;
class Net;
class Instance {
public:
    Instance(Cell* owner, Cell* model, const std ::string&);
    ~Instance();
    inline const std ::string& getName() const;
    inline Cell* getMasterCell() const;
    inline Cell* getCell() const;
    inline const std ::vector<Term*>& getTerms() const;
    Term* getTerm(const std ::string&) const;
    inline Point getPosition() const;
    bool connect(const std ::string& name, Net*);
    void add(Term*);
    void remove(Term*);
    void setPosition(const Point&);
    void setPosition(int x, int y);
    void toXml(std::ostream& o);
    static Instance* fromXml(Cell*, xmlTextReaderPtr reader);

private:
    Cell* owner_;
    Cell* masterCell_;
    std ::string name_;
    std ::vector<Term*> terms_;
    Point position_;
};

inline const std ::string& Instance::getName() const { return name_; }
inline Cell* Instance::getMasterCell() const { return masterCell_; }
inline Cell* Instance::getCell() const { return owner_; }
inline const std ::vector<Term*>& Instance::getTerms() const { return terms_; }
inline Point Instance::getPosition() const { return position_; }
} // namespace Netlist
