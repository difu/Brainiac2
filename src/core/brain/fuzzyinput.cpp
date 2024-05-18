#include "fuzzyinput.h"

FuzzyInput::FuzzyInput(QObject *parent)
    : FuzzyBase{parent}
{
    m_channelId = 0;
    m_type = FuzzyBase::INPUT;
}

void FuzzyInput::fromJson(QJsonObject obj) {}

QJsonObject FuzzyInput::toJson() const
{
    return QJsonObject();
}

BrainiacGlobals::BrainiacId FuzzyInput::channelId() const
{
    return m_channelId;
}

void FuzzyInput::setChannelId(BrainiacGlobals::BrainiacId newChannelId)
{
    if (m_channelId == newChannelId)
        return;
    m_channelId = newChannelId;
    emit channelIdChanged();
}

qreal FuzzyInput::result(const AgentInstance *agentInstance)
{
    return 0;
}
