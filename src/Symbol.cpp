#include "Symbol.h"
#include "Cell.h"
#include "Shape.h"
namespace Netlist {

Symbol::Symbol(Cell* cell)
    : owner_(cell)
{
}
Symbol::~Symbol()
{
    //   while ( not shapes_     .empty() ) delete *shapes_     .begin();
}

Cell* Symbol::getCell() const { return owner_; }
Point Symbol::getTermPosition(Term* term) const
{
    return getTermShape(term)->getX();
}

TermShape* Symbol::getTermShape(Term* term) const
{
    for (auto shape : getShapes()) {
        TermShape* termShape = dynamic_cast<TermShape*>(shape);
        if (termShape and (term == termShape->getTerm()))
            return termShape;
    }
    std::cout << "TermShape not found" << std::endl;
    return nullptr;
}

void Symbol::add(Shape* shape)
{
    // if (find(shapes_.begin(), shapes_.end(),shape)) {
    //     std::cerr << "[ERROR] Attemp to add duplicated Shape <" << owner_->getName() << ">." << std::endl;
    //     exit( 1 );
    //   }
    shapes_.push_back(shape);
}

void Symbol::remove(Shape* shape)
{
    for (std::vector<Shape*>::iterator ishp = shapes_.begin(); ishp != shapes_.end(); ++ishp) {
        if (*ishp == shape)
            shapes_.erase(ishp);
    }
}
void Symbol::toXml(std::ostream& stream) const
{
    stream << indent++ << "<symbol>\n";

    for (std::vector<Shape*>::const_iterator ishp = shapes_.begin(); ishp != shapes_.end(); ++ishp) {
        (*ishp)->toXml(stream);
    }
    stream << --indent << "</symbol>\n";
}

Symbol* Symbol::fromXml(Cell* cell, xmlTextReaderPtr reader)
{
    const xmlChar* symbolTag = xmlTextReaderConstString(reader, (const xmlChar*)"symbol");
    const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
    Symbol* symbol = nullptr;
    if (symbolTag == nodeName) {
        symbol = cell->getSymbol();

        while (true) {
            int status = xmlTextReaderRead(reader);
            if (status != 1) {
                if (status != 0) {
                    std::cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << std::endl;
                }
                break;
            }

            switch (xmlTextReaderNodeType(reader)) {
            case XML_READER_TYPE_COMMENT:
            case XML_READER_TYPE_WHITESPACE:
            case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);

            if (nodeName == symbolTag) {
                if (xmlTextReaderNodeType(reader) != XML_READER_TYPE_END_ELEMENT)
                    std::cerr << "[ERROR] no closing </symbol> tag (line: "
                              << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
                break;
            }

            if (Shape::fromXml(symbol, reader))
                continue;

            std::cerr << "[ERROR] Cell::fromXml(): Unknown or misplaced tag <" << nodeName
                      << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return symbol;
        }
    } else
        std::cerr << "[ERROR] Cell::fromXml(): Unknown or misplaced tag <" << nodeName
                  << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

    return symbol;
}
Box Symbol::getBoundingBox() const
{
    Box bb;
    for (const auto ishape : shapes_)
        bb.merge(ishape->getBoundingBox());

    return bb;
}

}