#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QGraphicsItem>

#include "src/core/brainiacglobals.h"

class EditorItem : public QGraphicsItem
{
public:
    EditorItem(QObject *parent);
    enum { Type = UserType + 15 };
    QRectF boundingRect() const;
    BrainiacGlobals::ItemType objectType() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    static const qint32 relxPos = -150;
    static const qint32 relyPos = -50;
    static const qint32 HEIGHT = 50;
    static const qint32 WIDTH = 120;
    static const qint32 _raster = 10;
    constexpr static const qreal adjust = 0.5;

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

protected:
private:
    QObject *m_object;
    BrainiacGlobals::ItemType m_type;
};

#endif // EDITORITEM_H
