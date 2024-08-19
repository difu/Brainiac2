#include "fuzzyinput.h"

FuzzyInput::FuzzyInput(QObject *parent)
    : FuzzyBase{FuzzyBase::INPUT, parent}
{
    m_channelId = 0;
}

void FuzzyInput::fromJson(QJsonObject obj) {}

QJsonObject FuzzyInput::toJson() const
{
    return {};
}

BrainiacGlobals::BrainiacId FuzzyInput::channelId() const
{
    return m_channelId;
}

void FuzzyInput::setChannelId(BrainiacGlobals::BrainiacId newChannelId)
{
    if (m_channelId == newChannelId) {
        return;
    }
    m_channelId = newChannelId;
    emit channelIdChanged();
}

qreal FuzzyInput::result(const AgentInstance *agentInstance)
{
    return 0;
}
