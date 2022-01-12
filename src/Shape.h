#pragma once

#include "Box.h"
#include "Term.h"

namespace Netlist {
class Symbol;

class Shape {
public:
    Shape(Symbol*);
    ~Shape();
    inline Symbol* getSymbol() const;
    virtual Box getBoundingBox() const = 0;
    virtual void toXml(std::ostream&) const = 0;
    static Shape* fromXml(Symbol*, xmlTextReaderPtr);

private:
    Symbol* owner_;
};

inline Symbol* Shape::getSymbol() const { return owner_; }

class BoxShape : public Shape {
private:
    Box box_;

public:
    BoxShape(Symbol*, const Box&);
    BoxShape(Symbol*, int x1, int y1, int x2, int y2);
    ~BoxShape();
    Box getBoundingBox() const;
    void toXml(std::ostream&) const;
    static Shape* fromXml(Symbol*, xmlTextReaderPtr reader);
};

class LineShape : public Shape {

public:
    LineShape(Symbol*, int x1, int y1, int x2, int y2);
    ~LineShape();
    Box getBoundingBox() const;
    void toXml(std::ostream&) const;
    static Shape* fromXml(Symbol*, xmlTextReaderPtr reader);

private:
    int x1_, y1_, x2_, y2_;
};

class TermShape : public Shape {
public:
    enum NameAlign { TopLeft = 1,
        TopRight,
        BottomLeft,
        BottomRight };

private:
    int x1_, y1_;
    Term* term_;
    NameAlign align_;

public:
    TermShape(Symbol*, std::string name, int x, int y);
    ~TermShape();
    Box getBoundingBox() const;
    inline Term* getTerm() const;
    inline void setAlign(NameAlign);
    inline Point getX() const;
    void toXml(std::ostream&) const;
    static Shape* fromXml(Symbol*, xmlTextReaderPtr reader);
};
inline Term* TermShape::getTerm() const { return term_; }
inline Point TermShape::getX() const { return Point(x1_, y1_); }
inline void TermShape::setAlign(NameAlign al) { align_ = al; }

class EllipseShape : public Shape {
private:
    Box box_;

public:
    EllipseShape(Box, Symbol*);
    ~EllipseShape();
    inline Box getBoundingBox() const;
    void toXml(std::ostream&) const;
    static Shape* fromXml(Symbol*, xmlTextReaderPtr);
};

inline Box EllipseShape::getBoundingBox() const { return box_; }

class ArcShape : public Shape {
private:
    Box box_;
    int start_;
    int span_;

public:
    ArcShape(Box, int, int, Symbol*);
    ~ArcShape();
    inline Box getBoundingBox() const;
    inline int getStart() const;
    inline int getSpan() const;
    void toXml(std::ostream&) const;
    static Shape* fromXml(Symbol*, xmlTextReaderPtr);
};

inline Box ArcShape::getBoundingBox() const { return box_; }
inline int ArcShape::getStart() const { return start_; }
inline int ArcShape::getSpan() const { return span_; }

} // namespace netlist