#include "fuzzyoutput.h"

#include "agentinstancebrain.h"
#include "src/core/agentinstance.h"

FuzzyOutput::FuzzyOutput(QObject *parent)
    : FuzzyBase{parent}
{
    m_channelId = 0;
    m_type = FuzzyBase::OUTPUT;
}

void FuzzyOutput::fromJson(QJsonObject obj) {}

QJsonObject FuzzyOutput::toJson() const
{
    return QJsonObject();
}

BrainiacGlobals::BrainiacId FuzzyOutput::channelId() const
{
    return m_channelId;
}

void FuzzyOutput::setChannelId(BrainiacGlobals::BrainiacId newChannelId)
{
    if (m_channelId == newChannelId) {
        return;
    }
    m_channelId = newChannelId;
    emit channelIdChanged();
}

qreal FuzzyOutput::result(const AgentInstance *agentInstance)
{
    if(agentInstance->instanceBrain()->hasResult(this->id())) {
        return 0.0;
    }
    return 0;
}
