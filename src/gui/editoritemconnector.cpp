#include "editoritemconnector.h"
#include "editoritem.h"

EditorItemConnector::EditorItemConnector(EditorItem *startItem, EditorItem *endItem)
{
    m_StartItem = startItem;
    m_EndItem = endItem;
    m_inverted = false;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_StartItem->addConnector(this);
    m_EndItem->addConnector(this);
    setZValue(-1000.0);
}

void EditorItemConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;
}

bool EditorItemConnector::inverted() const
{
    return m_inverted;
}

void EditorItemConnector::setInverted(bool newInverted)
{
    m_inverted = newInverted;
}

EditorItemConnector::~EditorItemConnector()
{
    m_StartItem->removeConnector(this);
    m_EndItem->removeConnector(this);
}
