#include "agentinstance.h"
#include "agent.h"
#include "channel.h"

#include "generator/locator.h"

#include <QDebug>

AgentInstance::AgentInstance(Locator *locator, Agent *parent)
    : QObject{parent}
{
    m_agent = qobject_cast<Agent *>(parent);
    if (!m_agent) {
        qCritical() << "Parent of AgentInstance is not Agent!";
    }
    m_locator = locator;
    reset();
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

AgentInstaceGeometryQuick3D *AgentInstance::geometryQuick3DNode() const
{
    return m_geometryQuick3DNode;
}

void AgentInstance::setGeometryQuick3DNode(AgentInstaceGeometryQuick3D *newGeometryQuick3DNode)
{
    if (m_geometryQuick3DNode == newGeometryQuick3DNode)
        return;
    m_geometryQuick3DNode = newGeometryQuick3DNode;
    emit geometryQuick3DNodeChanged();
}

Locator *AgentInstance::locator() const
{
    return m_locator;
}

QVector3D AgentInstance::translation() const
{
    return m_translation;
}

void AgentInstance::setTranslation(const QVector3D &newTranslation)
{
    if (m_translation == newTranslation)
        return;
    m_translation = newTranslation;
    emit translationChanged();
}

void AgentInstance::reset()
{
    setTranslation(m_locator->location());
    m_rotation = m_locator->rotation();
    m_newTranslation = m_translation;
    m_newRotation = m_rotation;
}
