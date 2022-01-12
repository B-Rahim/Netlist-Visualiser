#pragma once

#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QWidget>
class QPainter;

#include "Box.h"
#include "Point.h"

namespace Netlist {

#define InstanceShapes 1
#define LineShapes 2
#define TermShapes 4

class Cell;
class NodeTerm;

class CellWidget : public QWidget {
    Q_OBJECT;

public:
    CellWidget(QWidget* parent = NULL);
    virtual ~CellWidget();
    void setCell(Cell*);
    inline Cell* getCell() const;
    virtual QSize minimumSizeHint() const;
    inline QRect boxToScreenRect(const Box&) const;
    inline QPoint pointToScreenPoint(const Point&) const;
    inline Box screenRectToBox(const QRect&) const;
    inline Point screenPointToPoint(const QPoint&) const;
    void goRight();
    void goLeft();
    void goUp();
    void goDown();

public:
    virtual void resizeEvent(QResizeEvent*);
    void query(unsigned int, QPainter&);

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void keyPressEvent(QKeyEvent*);

private:
    Cell* cell_;
    Box viewport_;
};
inline Cell* CellWidget::getCell() const { return cell_; }

inline QRect CellWidget::boxToScreenRect(const Box& box) const
{
    QPoint sp(pointToScreenPoint(Point(box.getX1(), box.getY2())));
    return QRect(sp.x(), sp.y(), box.getWidth(), box.getHeight());
}

inline Box CellWidget::screenRectToBox(const QRect& Qbox) const
{

    Point p1(screenPointToPoint(Qbox.bottomLeft()));
    Point p2(screenPointToPoint(Qbox.topRight()));
    return Box(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}

inline QPoint CellWidget::pointToScreenPoint(const Point& p) const
{
    return QPoint(p.getX() - viewport_.getX1(), viewport_.getY2() - p.getY());
}

inline Point CellWidget::screenPointToPoint(const QPoint& sp) const
{
    return Point(sp.x() + viewport_.getX1(), viewport_.getY2() - sp.y());
}

} // Netlist namespace.