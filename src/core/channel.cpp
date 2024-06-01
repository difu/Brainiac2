#include "channel.h"
#include "agent.h"
#include "agentinstance.h"

#include <QDebug>

Channel::Channel(AgentInstance *parent, ChannelDefaults *defaults)
    : QObject{parent}
{
    m_inherited = false;
    m_defaults = defaults;
    m_agentInstance = parent;
    m_value = m_oldValue = m_defaults->value;
}

void Channel::advance() {
    m_oldValue=m_value;
}

Channel::ChannelDefaults *Channel::defaults() const
{
    return m_defaults;
}

qreal Channel::value() const {
    return m_value;
}

qreal Channel::oldValue() const {
    return m_oldValue;
}

void Channel::setValue(qreal value, bool isSpeed)
{
    qreal origValue=m_value;
    if(isSpeed) {
        m_value=m_oldValue+value;
    } else {
        m_value=value;
    }
    m_value=qBound(m_defaults->min,m_value,m_defaults->max);
    if(m_value!=origValue) {
        //emit valueChanged(m_value);
    }
}

QString Channel::getInfo()
{
    QString name;
    QString type;
    if (BrainiacGlobals::BrainiacId id = m_agentInstance->inputChannels().key(this, 0); id != 0) {
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
