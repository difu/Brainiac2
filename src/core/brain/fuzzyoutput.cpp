#include "fuzzyoutput.h"

FuzzyOutput::FuzzyOutput(QObject *parent)
    : FuzzyBase{parent}
{
    m_channelId = 0;
    LogicType m_type = FuzzyBase::OUTPUT;
}

void FuzzyOutput::fromJson(QJsonObject obj) {}

QJsonObject FuzzyOutput::toJson() const {}

BrainiacGlobals::BrainiacId FuzzyOutput::channelId() const
{
    return m_channelId;
}

void FuzzyOutput::setChannelId(BrainiacGlobals::BrainiacId newChannelId)
{
    if (m_channelId == newChannelId)
        return;
    m_channelId = newChannelId;
    emit channelIdChanged();
}
