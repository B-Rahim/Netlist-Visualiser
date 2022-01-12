#include "Shape.h"
#include "Cell.h"
#include "Symbol.h"

namespace Netlist {

Shape ::Shape(Symbol* owner)
    : owner_(owner)
{
    owner->add(this);
}

Shape ::~Shape()
{
    owner_->remove(this);
}

Shape* Shape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
{
    // Factory-like method.
    const xmlChar* boxTag
        = xmlTextReaderConstString(reader, (const xmlChar*)"box");
    const xmlChar* ellipseTag
        = xmlTextReaderConstString(reader, (const xmlChar*)"ellipse");
    const xmlChar* arcTag
        = xmlTextReaderConstString(reader, (const xmlChar*)"arc");
    const xmlChar* lineTag
        = xmlTextReaderConstString(reader, (const xmlChar*)"line");
    const xmlChar* termTag
        = xmlTextReaderConstString(reader, (const xmlChar*)"term");
    const xmlChar* nodeName
        = xmlTextReaderConstLocalName(reader);

    Shape* shape = NULL;
    if (boxTag == nodeName)
        shape = BoxShape::fromXml(owner, reader);
    if (ellipseTag == nodeName)
        shape = EllipseShape::fromXml(owner, reader);
    if (arcTag == nodeName)
        shape = ArcShape::fromXml(owner, reader);
    if (lineTag == nodeName)
        shape = LineShape::fromXml(owner, reader);
    if (termTag == nodeName)
        shape = TermShape::fromXml(owner, reader);

    if (shape == NULL)
        std::cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
                  << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

    return shape;
}

TermShape ::TermShape(Symbol* owner, std::string name, int x1, int y1)
    : Shape(owner)
    , x1_(x1)
    , y1_(y1)
    , term_(NULL)
{

    term_ = owner->getCell()->getTerm(name);

    if (term_ == NULL)
        std::cout << "There is no term named:" << name << std::endl;
}

TermShape ::~TermShape() { }

Box TermShape ::getBoundingBox() const
{
    return Box(x1_, y1_, x1_, y1_);
}

TermShape::NameAlign str2Align(std::string);
std::string align2str(TermShape::NameAlign);

void TermShape::toXml(std::ostream& o) const
{
    o << indent << "<term name=\"" << term_->getName() << "\" x1=\""
      << x1_ << "\" y1=\"" << y1_ << "\" align=\"" << align2str(align_) << "\"/>" << std::endl;
}

Shape* TermShape ::fromXml(Symbol* symbol, xmlTextReaderPtr reader)
{

    const xmlChar* termTag = xmlTextReaderConstString(reader, (const xmlChar*)"term");
    const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
    if (termTag == nodeName) {
        int x, y;
        std::string tshName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
        std::string alignName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"align"));
        bool vx = xmlGetIntAttribute(reader, "x1", x);
        bool vy = xmlGetIntAttribute(reader, "y1", y);

        bool cond = alignName.empty() or tshName.empty() or not(vx and vy);

        if (cond) {
            std::cerr << "[ERROR] atribute missing in <term> tag (line: "
                      << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return NULL;
        }
        TermShape* termShape = new TermShape(symbol, tshName, x, y);
        termShape->setAlign(str2Align(alignName));

        return termShape;
    }
    return NULL;
}

BoxShape ::BoxShape(Symbol* owner, const Box& box)
    : Shape(owner)
    , box_(box)
{
}

BoxShape ::BoxShape(Symbol* owner, int x1, int y1, int x2, int y2)
    : Shape(owner)
    , box_(x1, y1, x2, y2)
{
}

BoxShape ::~BoxShape() { }
Box BoxShape ::getBoundingBox() const
{
    return box_;
}

void BoxShape::toXml(std::ostream& o) const
{
    o << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
      << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>" << std::endl;
}

Shape* BoxShape ::fromXml(Symbol* symbol, xmlTextReaderPtr reader)
{

    const xmlChar* termTag = xmlTextReaderConstString(reader, (const xmlChar*)"box");
    const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
    if (termTag == nodeName) {
        int x1, y1, x2, y2;
        bool vx1 = xmlGetIntAttribute(reader, "x1", x1);
        bool vy1 = xmlGetIntAttribute(reader, "y1", y1);
        bool vx2 = xmlGetIntAttribute(reader, "x2", x2);
        bool vy2 = xmlGetIntAttribute(reader, "y2", y2);

        if (not(vx1 and vy1 and vx2 and vy2)) {
            std::cerr << "[ERROR] atribute missing in <box> tag (line: "
                      << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return NULL;
        }
        BoxShape* boxShape = new BoxShape(symbol, x1, y1, x2, y2);

        return boxShape;
    }
    return NULL;
}

LineShape ::LineShape(Symbol* owner, int x1, int y1, int x2, int y2)
    : Shape(owner)
    , x1_(x1)
    , y1_(y1)
    , x2_(x2)
    , y2_(y2)
{
}

LineShape ::~LineShape() { }

Box LineShape ::getBoundingBox() const
{
    return Box(x1_, y1_, x2_, y2_);
}

void LineShape::toXml(std::ostream& o) const
{
    o << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_
      << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>" << std::endl;
}

Shape* LineShape ::fromXml(Symbol* symbol, xmlTextReaderPtr reader)
{
    const xmlChar* lineTag = xmlTextReaderConstString(reader, (const xmlChar*)"line");
    const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
    if (lineTag == nodeName) {
        int x1, y1, x2, y2;
        bool vx1 = xmlGetIntAttribute(reader, "x1", x1);
        bool vy1 = xmlGetIntAttribute(reader, "y1", y1);
        bool vx2 = xmlGetIntAttribute(reader, "x2", x2);
        bool vy2 = xmlGetIntAttribute(reader, "y2", y2);

        if (not(vx1 and vy1 and vx2 and vy2)) {
            std::cerr << "[ERROR] atribute missing in <line> tag (line: "
                      << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return NULL;
        }
        LineShape* lineShape = new LineShape(symbol, x1, y1, x2, y2);
        return lineShape;
    }

    return NULL;
}

ArcShape::ArcShape(Box box, int start, int span, Symbol* owner)
    : Shape(owner)
    , box_(box)
    , start_(start)
    , span_(span)
{
}

ArcShape::~ArcShape()
{
}

void ArcShape::toXml(std::ostream& stream) const
{
    stream << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
           << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" "
           << "start=\"" << start_ << "\" span=\"" << span_ << "\"/>" << std::endl;
}
Shape* ArcShape ::fromXml(Symbol* symbol, xmlTextReaderPtr reader)
{
    const xmlChar* arcTag = xmlTextReaderConstString(reader, (const xmlChar*)"arc");
    const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
    if (arcTag == nodeName) {
        int x1, y1, x2, y2, start, span;
        bool vx1 = xmlGetIntAttribute(reader, "x1", x1);
        bool vy1 = xmlGetIntAttribute(reader, "y1", y1);
        bool vx2 = xmlGetIntAttribute(reader, "x2", x2);
        bool vy2 = xmlGetIntAttribute(reader, "y2", y2);
        bool vStart = xmlGetIntAttribute(reader, "start", start);
        bool vSpan = xmlGetIntAttribute(reader, "span", span);

        if (not(vx1 and vy1 and vx2 and vy2 and vSpan and vStart)) {
            std::cerr << "[ERROR] atribute missing in <arc> tag (line: "
                      << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return NULL;
        }
        Box box(x1, y1, x2, y2);
        ArcShape* arcShape = new ArcShape(box, start, span, symbol);
        return arcShape;
    }
    return NULL;
}

EllipseShape::EllipseShape(Box box, Symbol* owner)
    : Shape(owner)
    , box_(box)
{
}

EllipseShape::~EllipseShape()
{
}

void EllipseShape::toXml(std::ostream& stream) const
{
    stream << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
           << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>" << std::endl;
}
Shape* EllipseShape ::fromXml(Symbol* symbol, xmlTextReaderPtr reader)
{
    const xmlChar* ellipseTag = xmlTextReaderConstString(reader, (const xmlChar*)"ellipse");
    const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
    if (ellipseTag == nodeName) {
        int x1, y1, x2, y2;
        bool vx1 = xmlGetIntAttribute(reader, "x1", x1);
        bool vy1 = xmlGetIntAttribute(reader, "y1", y1);
        bool vx2 = xmlGetIntAttribute(reader, "x2", x2);
        bool vy2 = xmlGetIntAttribute(reader, "y2", y2);

        if (not(vx1 and vy1 and vx2 and vy2)) {
            std::cerr << "[ERROR] atribute missing in <ellipse> tag (line: "
                      << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return NULL;
        }
        Box box(x1, y1, x2, y2);
        EllipseShape* ellipseShape = new EllipseShape(box, symbol);
        return ellipseShape;
    }
    return NULL;
}

std::string align2str(TermShape::NameAlign a)
{
    std::string s;
    if (a == TermShape::TopLeft)
        s = "top_left";
    else if (a == TermShape::TopRight)
        s = "top_right";
    else if (a == TermShape::BottomLeft)
        s = "bottom_left";
    else if (a == TermShape::BottomRight)
        s = "bottom_right";
    return s;
}
TermShape::NameAlign str2Align(std::string s)
{
    TermShape::NameAlign a;
    if (s == "top_left")
        a = TermShape::TopLeft;
    else if (s == "top_right")
        a = TermShape::TopRight;
    else if (s == "bottom_left")
        a = TermShape::BottomLeft;
    else if (s == "bottom_right")
        a = TermShape::BottomRight;
    return a;
}

}