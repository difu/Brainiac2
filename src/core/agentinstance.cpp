#include "agentinstance.h"
#include "agent.h"
#include "channel.h"
#include "scene.h"

#include "generator/locator.h"

#include <QDebug>

AgentInstance::AgentInstance(Locator *locator, Agent *parent)
    : QObject{parent}
{
    m_agent = parent;
    locator->setAgentInstance(this);
    m_locator = locator;
    m_agent->scene()->addAgentInstance(this);

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

    // TODO
    // Should not be necessary, assume the Quick3d node is ready!
    if (m_geometryQuick3DNode) {
        m_geometryQuick3DNode->emitTranslationChanged();
    }
    emit translationChanged();
}

QVector3D AgentInstance::rotation() const
{
    return m_rotation;
}

void AgentInstance::setRotation(const QVector3D &newRotation)
{
    if (m_rotation == newRotation)
        return;

    m_rotation = newRotation;
    // TODO
    // Should not be necessary, assume the Quick3d node is ready!
    if (m_geometryQuick3DNode) {
        m_geometryQuick3DNode->emitRotationChanged();
    }
    emit rotationChanged();
}

void AgentInstance::reset()
{
    setTranslation(m_locator->location());
    setRotation(m_locator->rotation());
    // TODO
    // Should not be necessary, assume the Quick3d node is ready!
    if (m_geometryQuick3DNode) {
        m_geometryQuick3DNode->emitTranslationChanged();
        m_geometryQuick3DNode->emitRotationChanged();
    }
    m_newTranslation = m_translation;
    m_newRotation = m_rotation;
}
