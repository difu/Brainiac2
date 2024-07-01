#include "agentinstancebody.h"

#include "src/core/agent.h"
#include "src/core/agentinstance.h"
#include "src/core/body/body.h"

AgentInstanceBody::AgentInstanceBody(QObject *parent)
    : QObject{parent}
{
    m_agentInstance = qobject_cast<AgentInstance *>(parent);
    Q_ASSERT(m_agentInstance);
    m_body = m_agentInstance->agent()->body();
}
