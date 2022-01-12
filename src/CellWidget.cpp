// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include "CellWidget.h"
#include "Cell.h"
#include "Instance.h"
#include "Line.h"
#include "Net.h"
#include "Node.h"
#include "Shape.h"
#include "Symbol.h"
#include "Term.h"
#include <QApplication>
#include <QBrush>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QResizeEvent>

namespace Netlist {

using namespace std;

ostream& operator<<(ostream& o, const QRect& rect)
{
    o << "<QRect x:" << rect.x()
      << " y:" << rect.y()
      << " w:" << rect.width()
      << " h:" << rect.height() << ">";
    return o;
}

ostream& operator<<(ostream& o, const QPoint& p)
{
    o << "<QRect x:" << p.x() << " y:" << p.y() << ">";
    return o;
}

CellWidget::CellWidget(QWidget* parent)
    : QWidget(parent)
    , cell_(NULL)
    , viewport_(Box(0, 0, 500, 500))
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

CellWidget::~CellWidget()
{
}

void CellWidget::setCell(Cell* cell)
{
    cell_ = cell;
    repaint();
}

QSize CellWidget::minimumSizeHint() const
{
    return QSize(500, 500);
}

void CellWidget::resizeEvent(QResizeEvent* event)
{
    const QSize& size = event->size();
    // Assume the resize is always done by drawing the bottom right corner .
    viewport_.setX2(viewport_.getX1() + size.width());
    viewport_.setY1(viewport_.getY2() - size.height());

    repaint();
}

void CellWidget::paintEvent(QPaintEvent* event)
{
    QFont bigFont = QFont("URW Bookman L", 36);

    QString cellName = "NULL";
    if (cell_)
        cellName = cell_->getName().c_str();

    QPainter painter(this);
    painter.setFont(bigFont);
    painter.setBackground(QBrush(Qt::black));
    painter.eraseRect(QRect(QPoint(0, 0), size()));

    int frameWidth = 460;
    int frameHeight = 100;
    QRect nameRect((size().width() - frameWidth) / 2, (size().height() - frameHeight) / 2, frameWidth, frameHeight);
    painter.setFont(QFont("URW Bookman L", 6));

    painter.drawRect(nameRect);
    painter.drawText(nameRect, Qt::AlignCenter, cellName); //
    painter.setPen(QPen(Qt::cyan, 2));
    painter.setBrush(QBrush(Qt::cyan));
    query(LineShapes, painter);
    painter.setPen(QPen(Qt::green, 3));
    painter.setBrush(QBrush(Qt::black));
    query(InstanceShapes, painter);
    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(QBrush(Qt::red));
    query(TermShapes | InstanceShapes, painter);
}

void CellWidget ::keyPressEvent(QKeyEvent* event)
{

    event->ignore();
    if (event->modifiers() & (Qt ::ControlModifier | Qt ::ShiftModifier))
        return;
    switch (event->key()) {
    case Qt ::Key_Up:
        goUp();
        break;
    case Qt ::Key_Down:
        goDown();
        break;
    case Qt ::Key_Left:
        goLeft();
        break;
    case Qt ::Key_Right:
        goRight();
        break;
    default:
        return;
    }
    event->accept();
}

void CellWidget ::goRight()
{
    viewport_.translate(Point(20, 0));
    repaint();
}
void CellWidget ::goUp()
{
    viewport_.translate(Point(0, 20));
    repaint();
}
void CellWidget ::goLeft()
{
    viewport_.translate(Point(-20, 0));
    repaint();
}
void CellWidget ::goDown()
{
    viewport_.translate(Point(0, -20));
    repaint();
}

void CellWidget ::query(unsigned int flags, QPainter& painter)
{

    if ((not cell_) or (not flags))
        return;
    const vector<Instance*>& instances = cell_->getInstances();
    int seg = 10;

    for (size_t i = 0; i < instances.size(); ++i) {
        Point instPos = instances[i]->getPosition();
        const Symbol* symbol = instances[i]->getMasterCell()->getSymbol();

        if (not symbol)
            continue;
        if (flags & InstanceShapes) {
            const vector<Shape*>& shapes = symbol->getShapes();

            for (size_t j = 0; j < shapes.size(); ++j) {
                if (flags == InstanceShapes) {

                    BoxShape* boxShape = dynamic_cast<BoxShape*>(shapes[j]);

                    if (boxShape) {
                        Box box = boxShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box.translate(instPos));
                        painter.drawRect(rect);
                    }

                    LineShape* lineShape = dynamic_cast<LineShape*>(shapes[j]);
                    if (lineShape) {

                        Box box = lineShape->getBoundingBox();
                        box.translate(instPos);
                        QPoint source = pointToScreenPoint(Point(box.getX1(), box.getY1()));
                        QPoint target = pointToScreenPoint(Point(box.getX2(), box.getY2()));

                        painter.drawLine(source, target);
                    }

                    EllipseShape* ellipseShape = dynamic_cast<EllipseShape*>(shapes[j]);
                    if (ellipseShape) {
                        Box box = ellipseShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box.translate(instPos));
                        painter.drawEllipse(rect);
                    }
                    ArcShape* arcShape = dynamic_cast<ArcShape*>(shapes[j]);
                    if (arcShape) {

                        Box box = arcShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box.translate(instPos));
                        painter.drawArc(rect, arcShape->getStart() * 16, arcShape->getSpan() * 16);
                    }
                }

                if (flags & TermShapes) {
                    TermShape* termShape = dynamic_cast<TermShape*>(shapes[j]);
                    if (termShape) {
                        Point point = termShape->getX();
                        point.translate(instPos);
                        QPoint p[5] = { pointToScreenPoint(point) };

                        if (termShape->getTerm()->getType() == Term::External) {
                            //   int width = 8;
                            //   int height = 8;
                            //   QPoint bottomRight(p[0].x(), p[0].y() + height / 2);
                            //   QPoint topLeft(bottomRight.x() - width, bottomRight.y() - height);

                            //   if (termShape->getTerm()->getDirection() == Term::Out)
                            //   {
                            //     topLeft = bottomRight;
                            //     topLeft.ry() -= height;
                            //     bottomRight.rx() += width;
                            //     seg = 10;
                            //   }
                            //   else
                            //     seg = -10;

                            int width = 8;
                            int height = 8;
                            QPoint topLeft(p[0].x() - width / 2, p[0].y() - height / 2);
                            QPoint bottomRight(topLeft.x() + width, topLeft.y() + height);

                            painter.drawRect(QRect(topLeft, bottomRight));
                            p[0].ry() -= abs(seg);
                            p[0].rx() += seg;
                            painter.drawText(p[0], termShape->getTerm()->getName().c_str());
                        }
                    }
                }
            }
            if (flags == InstanceShapes) {
                QPoint sp = pointToScreenPoint(instPos);
                sp.ry() += 25;
                painter.drawText(sp, instances[i]->getName().c_str());
            }
        }
        if (flags & TermShapes) {
            const vector<Term*>& terms = cell_->getTerms();

            for (auto term : terms) {
                int offSet = 0;
                QPoint p[5] = { pointToScreenPoint(term->getPosition()) };
                if (term->getDirection() == Term::Out)
                    seg = 10;

                else {
                    seg = -10;
                    offSet = seg;
                }

                p[1].rx() = p[0].x() + seg;
                p[1].ry() = p[0].y() + seg;

                p[2].rx() = p[1].x() + seg;
                p[2].ry() = p[1].y();

                p[3].rx() = p[2].x();
                p[3].ry() = p[2].y() - 2 * (seg);

                p[4].rx() = p[3].x() - seg;
                p[4].ry() = p[3].y();

                painter.setPen(QPen(Qt::red, 2));
                painter.drawConvexPolygon(p, 5);
                p[0].rx() += 3 * seg + offSet;
                p[0].ry() += 3 * abs(seg);
                painter.drawText(p[0], term->getName().c_str());
            }
        }
        if (flags & LineShapes) {
            QPoint source, target;
            for (auto net : cell_->getNets()) {
                for (auto line : net->getLines()) {
                    source = pointToScreenPoint(line->getSourcePosition());
                    target = pointToScreenPoint(line->getTargetPosition());
                    painter.drawLine(source, target);
                }
                for (auto node : net->getNodes()) {
                    NodePoint* nodeP = dynamic_cast<NodePoint*>(node);
                    if (!nodeP)
                        continue;
                    if (nodeP->getLines().size() > 2) {
                        source = pointToScreenPoint(nodeP->getPosition());
                        painter.drawEllipse(source, 5, 5);
                    }
                }
            }
        }
    }
}

} // namespace Netlist