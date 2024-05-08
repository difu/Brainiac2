#include "braineditoritem.h"

#include <QPainter>

BrainEditorItem::BrainEditorItem(QObject *parent)
    : EditorItem{parent}
{}

void BrainEditorItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    EditorItem::paint(painter, option, widget);

    painter->fillRect(QRectF(relxPos - adjust + 5, relyPos - adjust + 39, 30, 7), Qt::black);

    // int tmpResult = 0;
    // if (lUnit->getMinValue() < 0) {
    //     tmpResult = -30 * ((lUnit->getResult()) / (lUnit->getMinValue()));
    // } else {
    //     tmpResult = 30 * ((lUnit->getResult()) / (lUnit->getMaxValue()));
    // }

    // if (tmpResult > 0) {
    //     painter->fillRect(QRectF(relxPos - adjust + 5, relyPos - adjust + 39, tmpResult, 7),
    //                       Qt::red);
    // } else {
    //     tmpResult = -tmpResult;
    //     painter->fillRect(QRectF(relxPos - adjust + 5, relyPos - adjust + 39, tmpResult, 7),
    //                       Qt::blue);
    // }
    // if (lUnit->getType() == FuzzyBase::AND) {
    //     FuzzyAnd *myAnd = (FuzzyAnd *) lUnit;
    //     if (myAnd->isSoundRule()) {
    //         painter->drawImage(relxPos + 22, relyPos + 5, m_featurePic);
    //     }
    // }
}
