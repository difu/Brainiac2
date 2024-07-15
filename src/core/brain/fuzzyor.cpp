//
// Created by difu on 6/2/24.
//

#include "fuzzyor.h"

#include "agentinstancebrain.h"
#include "brain/braineditoritem.h"
#include "src/core/agentinstance.h"

FuzzyOr::FuzzyOr(QObject *parent, BrainiacGlobals::BrainiacId id)
    : FuzzyBase{parent}
{
    m_type = FuzzyBase::OR;
    m_mode = MAX;
    setId(id);
    setMinValue(0.0);
    setMaxValue(1.0);
    this->setEditorItem(new BrainEditorItem(this));
}

void FuzzyOr::fromJson(QJsonObject obj) {}

QJsonObject FuzzyOr::toJson() const
{
    return {};
}

qreal FuzzyOr::result(const AgentInstance *agentInstance) {
    if (agentInstance->instanceBrain()->hasResult(this->id())) {
        return agentInstance->instanceBrain()->fuzzyResults().value(this->id());
    }
    if( m_parents.count()>0) {
        qreal tmpResult=this->minValue();
        foreach(Parent parent, m_parents) {
            const qreal parentResult=parent.parent->result(agentInstance);
            switch(m_mode) {
                case MAX:
                    if(parentResult>tmpResult) {
                        tmpResult=parentResult;
                    }
                    break;
                case SUM:
                    tmpResult+=parentResult;
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

FuzzyOr::Mode FuzzyOr::mode() const {
    return m_mode;
}

void FuzzyOr::setMode(const Mode newMode) {
    m_mode = newMode;
}
