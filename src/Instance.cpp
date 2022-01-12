
#include "Instance.h"
#include "Cell.h"
#include <algorithm>
namespace Netlist {

Instance::Instance(Cell* owner, Cell* model, const std ::string& name)
    : owner_(owner)
    , masterCell_(model)
    , name_(name)
{
    owner_->add(this);

    for (auto Cterm : model->getTerms()) {
        new Term(this, Cterm);
    }
}
Instance::~Instance()
{
    while (not terms_.empty())
        delete *terms_.begin();
}

bool Instance::connect(const std ::string& name, Net* net)
{
    for (auto& term : terms_)
        if (term->getName() == name) {
            term->setNet(net);
            return true;
        }
    return false;
}
void Instance::add(Term* term) { terms_.emplace_back(term); }
void Instance::remove(Term* term)
{
    auto it = find(terms_.begin(), terms_.end(), term);
    terms_.erase(it);
}

Term* Instance::getTerm(const std ::string& name) const
{
    for (std::vector<Term*>::const_iterator iterm = terms_.begin(); iterm != terms_.end(); ++iterm) {
        if ((*iterm)->getName() == name)
            return *iterm;
    }
    return NULL;
}

void Instance::setPosition(int x, int y)
{
    position_ = Point(x, y);
    for (auto term : getTerms())
        term->setPosition(term->getPosition().translate(x, y));
}

void Instance::setPosition(const Point& pos)
{
    setPosition(pos.getX(), pos.getY());
}

void Instance::toXml(std::ostream& o)
{
    o << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << getMasterCell()->getName()
      << "\" x=\"" << getPosition().getX() << "\" y=\"" << getPosition().getY() << "\"/>" << std::endl;
}

Instance* Instance::fromXml(Cell* cell, xmlTextReaderPtr reader)
{

    int x, y;
    std::string instName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
    std::string mCellName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"mastercell"));
    bool vx = xmlGetIntAttribute(reader, "x", x);
    bool vy = xmlGetIntAttribute(reader, "y", y);

    bool cond = not(mCellName.empty() or instName.empty()) and vx and vy;

    if (cond) {
        for (auto mCell : cell->getAllCells())
            if (mCell->getName() == mCellName) {
                Instance* instance = new Instance(cell, mCell, instName);
                instance->setPosition(x, y);
                return instance;
            }
        return NULL;
    } else
        return NULL;
}

}