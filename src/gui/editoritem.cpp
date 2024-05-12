#include "editoritem.h"

#include <QDebug>

#include <QLinearGradient>
#include <QPainter>
#include <QPalette>
#include "src/core/brain/noise.h"
#include <qdrawutil.h>

EditorItem::EditorItem(QObject *parent)
    : m_object{parent}
{
    m_type = BrainiacGlobals::UNKNOWN;
    if (qobject_cast<Noise *>(parent)) {
        m_type = BrainiacGlobals::NOISE;
        m_symbolPic.load(":/gui/pics/editor_logo_noise.png");
    }
    if (m_type == BrainiacGlobals::UNKNOWN) {
        qFatal() << "Unknown Itemtype!";
    }
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QRectF EditorItem::boundingRect() const
{
    return QRectF(relxPos - adjust * 15,
                  relyPos - adjust * 15,
                  WIDTH + adjust * 15,
                  HEIGHT + adjust * 15);
}

BrainiacGlobals::ItemType EditorItem::objectType() const
{
    return m_type;
}

void EditorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QLinearGradient myGrad(relxPos, relyPos, relxPos + WIDTH, relyPos + HEIGHT);
    myGrad.setColorAt(0.5, Qt::gray);
    myGrad.setColorAt(1, Qt::white);
    QBrush myBrush(myGrad);
    qDrawShadePanel(painter,
                    relxPos,
                    relyPos,
                    WIDTH,
                    HEIGHT,
                    QPalette(QColor(30, 30, 30, 127)),
                    true,
                    4,
                    &myBrush);
    painter->setPen(Qt::black);
    if (m_type == BrainiacGlobals::NOISE) {
        painter->drawText(relxPos - adjust + 40,
                          relyPos - adjust + 10,
                          75,
                          50,
                          Qt::TextWordWrap | Qt::AlignHCenter,
                          "Name of Noise");
    }
    painter->drawImage(QPoint(relxPos + 4, relyPos + 6), m_symbolPic);

    if (this->isSelected()) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(
            QRectF(relxPos - adjust, relyPos - adjust, WIDTH + adjust, HEIGHT + adjust));
    }
}

QPainterPath EditorItem::shape() const
{
    QPainterPath path;
    path.addRect(relxPos, relyPos, WIDTH, HEIGHT);
    return path;
}
