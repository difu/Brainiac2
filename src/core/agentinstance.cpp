#include "agentinstance.h"
#include "agent.h"
#include "channel.h"
#include "scene.h"

#include "generator/locator.h"
#include "src/core/brain/agentinstancebrain.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

AgentInstance::AgentInstance(Locator *locator, Agent *parent)
    : QObject{parent}
{
    m_agent = parent;
    m_geometryQuick3DNode = nullptr;
    m_instanceBrain = new AgentInstanceBrain(this);

    /**
        Take care that @ref Locator and @ref AgentInstance know each other.
        Only after that it is safe to add an agentInstance to the Quick 3D view.
    */
    locator->setAgentInstance(this);
    m_locator = locator;
    // m_agent->scene()->addAgentInstance(this);

    reset();
}

void AgentInstance::addInputChannel(BrainiacGlobals::BrainiacId id,
                                    Channel::ChannelDefaults *defaults)
{
    if(m_inputChannels.contains(id)) {
        return;
    }
    auto *newChannel = new Channel(this, defaults);
    m_inputChannels.insert(id, newChannel);
    qDebug() << "ChannelName " << newChannel->getInfo()
             << "Defaults: " << newChannel->defaults()->value;
}

void AgentInstance::addOutputChannel(BrainiacGlobals::BrainiacId id,
                                     Channel::ChannelDefaults *defaults)
{
    if(m_outputChannels.contains(id)){
        return;
    }
    auto *newChannel = new Channel(this, defaults);
    m_outputChannels.insert(id, newChannel);
//    qDebug() << "ChannelName " << newChannel->getInfo()
    //             << "Defaults: " << newChannel->defaults()->value;
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

QString AgentInstance::instanceQML() const {
    QFile file(":gui/AgentInstance.qml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Could not open gentInstance.qml";
    }
    QTextStream in(&file);
    QString qml = in.readAll();;
    return qml;
}

void AgentInstance::setGeometryQuick3DNode(AgentInstaceGeometryQuick3D *newGeometryQuick3DNode)
{
    if (m_geometryQuick3DNode == newGeometryQuick3DNode) {
        return;
    }
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
    if (qFuzzyCompare(newTranslation, m_translation)) {
        return;
    }
    m_translation = newTranslation;

    // TODO
    // Should not be necessary, assume the Quick3d node is ready!
    if (m_geometryQuick3DNode) {
        m_geometryQuick3DNode->emitTranslationChanged();
    }
    emit translationChanged();
}

void AgentInstance::setTranslationWithForcedEmit(const QVector3D &newTranslation) {
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
    if (qFuzzyCompare(newRotation, m_rotation)) {
        return;
    }

    m_rotation = newRotation;
    // TODO
    // Should not be necessary, assume the Quick3d node is ready!
    if (m_geometryQuick3DNode) {
        m_geometryQuick3DNode->emitRotationChanged();
    }
    emit rotationChanged();
}

void AgentInstance::setRotationithForcedEmit(const QVector3D &newRotation) {
    m_rotation = newRotation;
    // TODO
    // Should not be necessary, assume the Quick3d node is ready!
    if (m_geometryQuick3DNode) {
        m_geometryQuick3DNode->emitRotationChanged();
    }
    emit rotationChanged();
}

void AgentInstance::advance() {
    m_instanceBrain->think();
    for (auto [id, channel]: m_outputChannels.asKeyValueRange()) {
        if (id == BrainiacGlobals::CO_COLOR) {
            qDebug() << "Color: " << channel->value();
        }
    }
    m_newRotation.setX(m_rotation.x() + m_outputChannels.value(BrainiacGlobals::CO_RX)->value());
    m_newRotation.setY(m_rotation.y() + m_outputChannels.value(BrainiacGlobals::CO_RY)->value());
    m_newRotation.setZ(m_rotation.z() + m_outputChannels.value(BrainiacGlobals::CO_RZ)->value());

    m_newTranslation.setX(m_translation.x()
                          + m_outputChannels.value(BrainiacGlobals::CO_TZ)->value()
                          * BrainiacGlobals::sinGrad(m_newRotation.y())
                          + m_outputChannels.value(BrainiacGlobals::CO_TX)->value()
                          * BrainiacGlobals::cosGrad(m_newRotation.y()));
    m_newTranslation.setY(m_translation.y()); //!< @todo Implement this!
    m_newTranslation.setZ(m_translation.z()
                          + m_outputChannels.value(BrainiacGlobals::CO_TZ)->value()
                          * BrainiacGlobals::cosGrad(m_newRotation.y())
                          + m_outputChannels.value(BrainiacGlobals::CO_TX)->value()
                          * BrainiacGlobals::sinGrad(m_newRotation.y()));
    m_geometryQuick3DNode->setGeometryIsDirty(); // TODO: Think of using this in a more optimized way
}

void AgentInstance::advanceCommit()
{
    // TODO: Think of only emitting once the update of rot and trans
    setRotation(m_newRotation);
    setTranslation(m_newTranslation);
    m_instanceBrain->invalidateAll();
}

void AgentInstance::reset()
{
    setTranslationWithForcedEmit(m_locator->location());
    setRotationithForcedEmit(m_locator->rotation());

    m_newTranslation = m_translation;
    m_newRotation = m_rotation;
}
