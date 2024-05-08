#include "agentinstancebrain.h"
#include "brain.h"
#include "src/core/agent.h"
#include "src/core/agentinstance.h"
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
