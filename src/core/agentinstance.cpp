#include "agentinstance.h"
#include "agent.h"
#include "channel.h"

#include <QDebug>

AgentInstance::AgentInstance(QObject *parent)
    : QObject{parent}
{
    m_agent = qobject_cast<Agent *>(parent);
}

void AgentInstance::addInputChannel(BrainiacGlobals::BrainiacId id,
                                    Channel::ChannelDefaults *defaults)
{
    Channel *newChannel = new Channel(this, defaults);
    m_inputChannels.insert(id, newChannel);
    qDebug() << "ChannelName " << newChannel->getInfo()
             << "Defaults: " << newChannel->defaults()->value;
}

void AgentInstance::addOutputChannel(BrainiacGlobals::BrainiacId id,
                                     Channel::ChannelDefaults *defaults)
{
    Channel *newChannel = new Channel(this, defaults);
    m_outputChannels.insert(id, newChannel);
    qDebug() << "ChannelName " << newChannel->getInfo()
             << "Defaults: " << newChannel->defaults()->value;
}

void AgentInstance::reset() {}
// QTC_TEMP
Agent *AgentInstance::agent() const
{
    return m_agent;
}

QHash<BrainiacGlobals::BrainiacId, Channel *> AgentInstance::inputChannels() const
{
    return m_inputChannels;
}

QHash<BrainiacGlobals::BrainiacId, Channel *> AgentInstance::outputChannels() const
{
    return m_outputChannels;
}

QVector3D AgentInstance::initialTranslation() const
{
    return m_initialTranslation;
}

void AgentInstance::setInitialTranslation(const QVector3D &newInitialTranslation)
{
    m_initialTranslation = newInitialTranslation;
}
