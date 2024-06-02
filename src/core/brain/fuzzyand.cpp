//
// Created by difu on 6/2/24.
//

#include "fuzzyand.h"

#include "agentinstancebrain.h"
#include "brain/braineditoritem.h"
#include "src/core/agentinstance.h"

FuzzyAnd::FuzzyAnd(QObject *parent, BrainiacGlobals::BrainiacId id)
    : FuzzyBase{parent}
{
    m_type = FuzzyBase::AND;
    m_mode = MIN;
    setId(id);
    setMinValue(0.0);
    setMaxValue(1.0);
    this->setEditorItem(new BrainEditorItem(this));
}

void FuzzyAnd::fromJson(QJsonObject obj) {}

QJsonObject FuzzyAnd::toJson() const
{
    return {};
}

qreal FuzzyAnd::result(const AgentInstance *agentInstance) {
    if (agentInstance->instanceBrain()->hasResult(this->id())) {
        return agentInstance->instanceBrain()->fuzzyResults().value(this->id());
    }
    if( m_parents.count()>0) {
        qreal tmpResult=this->maxValue();
        foreach(Parent parent, m_parents) {
            const qreal parentResult=parent.parent->result(agentInstance);
            switch(m_mode) {
                case MIN:
                    if(parentResult<tmpResult) {
                        tmpResult=parentResult;
                    }
                    break;
                case PRODUCT:
                    tmpResult*=parentResult;
                break;
            }
        }
        const qreal finalResult = qBound(
            this->minValue(),
            tmpResult,
            this->maxValue());
        agentInstance->instanceBrain()->setResult(this->id(), finalResult);
        return finalResult;
    }
    return 0.0;
}
