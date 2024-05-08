#include "braineditoritem.h"
#include "src/core/agent.h"
#include "src/core/brain/brain.h"
#include "src/core/brain/fuzzybase.h"

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
    FuzzyBase *fuzzy = qobject_cast<FuzzyBase *>(m_object);
    if (!fuzzy) {
        qFatal() << "m_object not of type <FuzzyBase>!";
    } else {
        qreal tmpResult = 0.0;
        Brain *brain = fuzzy->brain();
        Agent *agent = brain->agent();
        AgentInstance *agentInstance = agent->defaultAgentInstance();
        if (fuzzy->minValue() < 0) {
            tmpResult = -30 * ((fuzzy->result(agentInstance)) / (fuzzy->minValue()));
        } else {
            tmpResult = +30 * ((fuzzy->result(agentInstance)) / (fuzzy->maxValue()));
        }

        if (tmpResult > 0) {
            painter->fillRect(QRectF(relxPos - adjust + 5, relyPos - adjust + 39, tmpResult, 7),
                              Qt::red);
        } else {
            tmpResult = -tmpResult;
            painter->fillRect(QRectF(relxPos - adjust + 5, relyPos - adjust + 39, tmpResult, 7),
                              Qt::blue);
        }
        qDebug() << "Result " << tmpResult << "AgentResult " << fuzzy->result(agentInstance);
    }

    // if (lUnit->getType() == FuzzyBase::AND) {
    //     FuzzyAnd *myAnd = (FuzzyAnd *) lUnit;
    //     if (myAnd->isSoundRule()) {
    //         painter->drawImage(relxPos + 22, relyPos + 5, m_featurePic);
    //     }
    // }
}
