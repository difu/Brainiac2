#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QGraphicsItem>

#include "src/core/brainiacglobals.h"

class EditorItemConnector;

/*!
 * \brief The EditorItem class
 * 
 * The parent is of one of 
 *   - FuzzyBase
 */
class EditorItem : public QGraphicsItem
{
public:
    explicit EditorItem(QObject *parent);
    virtual ~EditorItem();
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

    [[nodiscard]] int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QList<EditorItemConnector *> connectors() const;
    void addConnector(EditorItemConnector *connector);
    void removeConnector(EditorItemConnector *connector);
    void removeConnectors();

    QObject *object() const;

protected:
    QObject *m_object; //!< The parent object of this EditorItem, for example @ref Noise node
    QList<EditorItemConnector *> m_connectors;

private:
    BrainiacGlobals::ItemType m_type;
    QImage m_symbolPic;
};

#endif // EDITORITEM_H
