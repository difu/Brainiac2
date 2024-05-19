#include "editoritemconnector.h"

#include <QPainter>

#include "editoritem.h"

EditorItemConnector::EditorItemConnector(EditorItem *startItem, EditorItem *endItem)
{
    m_startItem = startItem;
    m_endItem = endItem;
    m_inverted = false;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_startItem->addConnector(this);
    m_endItem->addConnector(this);
    setZValue(-1000.0);
}

void EditorItemConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_startItem->collidesWithItem(m_endItem)) {
        return;
    }
    const QLineF line(mapFromItem(m_startItem, EditorItem::relxPos / 2, EditorItem::relyPos / 2),
                      mapFromItem(m_endItem, EditorItem::relxPos / 2, EditorItem::relyPos / 2));
    const qreal angle = line.angle() / 360 * 2 * BrainiacGlobals::PI;

    QPolygonF arrow;
    QPainterPath arrowPath;
    painter->drawLine(line);

    arrow << mapFromItem(m_startItem, 5 * sin(angle) + EditorItem::relxPos / 2,
                         5 * cos(angle) + EditorItem::relyPos / 2) << mapFromItem(
                m_startItem, -5 * sin(angle) + EditorItem::relxPos / 2,
                -5 * cos(angle) + EditorItem::relyPos / 2) <<
            mapFromItem(m_endItem, EditorItem::relxPos / 2, EditorItem::relyPos / 2) << mapFromItem(
                m_startItem, 5 * sin(angle) + EditorItem::relxPos / 2, 5 * cos(angle) + EditorItem::relyPos / 2);
    arrowPath.addPolygon(arrow);
    QColor arrowBorderColor;
    if (this->isSelected()) {
        arrowBorderColor = Qt::red;
    } else {
        arrowBorderColor = QColor(79, 106, 25);
    }
    painter->setPen(QPen(arrowBorderColor, 1, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    if (m_inverted) {
        painter->setBrush(QColor(20, 20, 39));
    } else {
        painter->setBrush(QColor(122, 163, 39));
    }
    painter->drawPath(arrowPath);
    setLine(line);
}

bool EditorItemConnector::inverted() const
{
    return m_inverted;
}

void EditorItemConnector::setInverted(bool newInverted)
{
    m_inverted = newInverted;
}

EditorItemConnector::~EditorItemConnector() {
    m_startItem->removeConnector(this);
    m_endItem->removeConnector(this);
}
