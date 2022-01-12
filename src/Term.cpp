#include "Term.h"
#include "Cell.h"
#include "Instance.h"
#include "Net.h"

namespace Netlist {

Term::Term(Cell* cell, const std ::string& name, Direction direction)
    : owner_(cell)
    , name_(name)
    , direction_(direction)
    , type_(External)
    , net_(nullptr)
    , node_(this)
{
    ((Cell*)owner_)->add(this);
}

Term::Term(Instance* inst, const Term* modelTerm)
    : owner_(inst)
    , name_(modelTerm->name_)
    , direction_(modelTerm->direction_)
    , type_(Internal)
    , net_(nullptr)
    , node_(this)
{
    node_.setPosition(modelTerm->getPosition());
    ((Instance*)owner_)->add(this);
}
Term::~Term()
{

    net_->remove(&node_);
}

Cell* Term::getOwnerCell() const
{
    if (type_ == External)
        return (Cell*)owner_;
    else
        return ((Instance*)owner_)->getCell();
}

void Term::setNet(Net* net)
{
    if (net) {
        if (net->getCell() != getOwnerCell()) {
            std::cerr << "[ERROR] Term & Net do not belong to the same Cell."
                      << std::endl;
            return;
        }
        net_ = net;
        net_->add(&node_);
    } else
        net_->remove(&node_);
}
void Term::setNet(const std ::string& name)
{
    setNet(getOwnerCell()->getNet(name));
}
void Term::setPosition(const Point& point) { node_.setPosition(point); }
void Term::setPosition(int x, int y) { node_.setPosition(x, y); }
void Term::setDirection(Direction d) { direction_ = d; }

void Term::toXml(std::ostream& o)
{
    o << indent << "<term name=\"" << name_ << "\" direction=\"" << convertD(direction_) << "\" "
      << " x=\"" << getPosition().getX() << "\" y=\"" << getPosition().getY() << "\"/>" << std::endl;
}

std::string convertT(Term::Type t)
{
    if (t == Term::Internal)
        return "Internal";
    return "External";
}

std::string convertD(Term::Direction d)
{
    switch (d) {
    case Term::In:
        return "In";
    case Term::Out:
        return "Out";
    case Term::Inout:
        return "Inout";
    case Term::Tristate:
        return "Tristate";
    case Term::Transcv:
        return "Transcv";
    case Term::Unknown:
        return "Unknown";
    }
    return "error";
}

Term* Term::fromXml(Cell* cell, xmlTextReaderPtr reader)
{
    Direction dir;
    int x, y;
    std::string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
    std::string termDir = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"direction"));
    bool vx = xmlGetIntAttribute(reader, "x", x);
    bool vy = xmlGetIntAttribute(reader, "y", y);

    bool cond = not(termName.empty() or termDir.empty()) and vx and vy;

    if (cond) {

        if (termDir == "In")
            dir = In;
        else if (termDir == "Out")
            dir = Out;
        else if (termDir == "Inout")
            dir = Inout;
        else if (termDir == "Tristate")
            dir = Tristate;
        else if (termDir == "Transcv")
            dir = Transcv;
        else
            dir = Unknown;

        Term* term = new Term(cell, termName, dir);
        term->setPosition(x, y);
        return term;
    } else
        return nullptr;
}
}