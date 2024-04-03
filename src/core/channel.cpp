#include "channel.h"
#include "agent.h"
#include "agentinstance.h"

#include <QDebug>

Channel::Channel(AgentInstance *parent, ChannelDefaults *defaults)
    : QObject{parent}
{
    m_defaults = defaults;
    m_agentInstance = parent;
}

Channel::ChannelDefaults *Channel::defaults() const
{
    return m_defaults;
}

QString Channel::getInfo()
{
    QString name;
    QString type;
    BrainiacGlobals::BrainiacId id = m_agentInstance->inputChannels().key(this, 0);
    if (id != 0) {
        type = "Input";
        name = m_agentInstance->agent()->inputChannelName(id);
    } else {
        id = m_agentInstance->outputChannels().key(this, 0);
        if (id != 0) {
            type = "Output";
            name = m_agentInstance->agent()->outputChannelName(id);
        } else {
            qCritical() << __PRETTY_FUNCTION__ << "This should never happen!";
        }
    }

    return name + "(" + type + ")";
}
