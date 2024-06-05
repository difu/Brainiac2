#include "agentinstancebrain.h"
#include "brain.h"
#include "src/core/agent.h"
#include "src/core/agentinstance.h"
#include "src/core/brain/fuzzyoutput.h"

AgentInstanceBrain::AgentInstanceBrain(QObject *parent)
    : QObject{parent}
{
    m_agentInstance = qobject_cast<AgentInstance *>(parent);
    m_brain = m_agentInstance->agent()->brain();
}

QHash<BrainiacGlobals::BrainiacId, qreal> AgentInstanceBrain::fuzzyResults() const
{
    return m_fuzzyResults;
}

void AgentInstanceBrain::invalidateAll()
{
    m_fuzzyResults.clear();
}

void AgentInstanceBrain::invalidateResult(BrainiacGlobals::BrainiacId fuzzyId)
{
    m_fuzzyResults.remove(fuzzyId);
}

bool AgentInstanceBrain::hasResult(BrainiacGlobals::BrainiacId id) const
{
    return m_fuzzyResults.contains(id);
}

void AgentInstanceBrain::setResult(BrainiacGlobals::BrainiacId id, qreal result)
{
    m_fuzzyResults.insert(id, result);
}

void AgentInstanceBrain::think() {
    foreach(FuzzyBase *fuzz, m_brain->fuzzies()) {
        qreal result = fuzz->result(m_agentInstance);
        qDebug() << "Fuzzy " << fuzz->name() << " result: " << result;
    }
}
