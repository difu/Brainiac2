#ifndef EDITORITEMCONNECTOR_H
#define EDITORITEMCONNECTOR_H

#include <QGraphicsLineItem>

class EditorItem;

class EditorItemConnector : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    EditorItemConnector(EditorItem *startItem, EditorItem *endItem);

    EditorItem *startItem() const { return m_startItem; }
    EditorItem *endItem() const { return m_endItem; }

    virtual ~EditorItemConnector();

    bool inverted() const;
    void setInverted(bool newInverted);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget = 0);

private:
    EditorItem *m_startItem;
    EditorItem *m_endItem;

    bool m_inverted;
};

#endif // EDITORITEMCONNECTOR_H
