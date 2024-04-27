#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

#include <QObject>

#include <QHash>
#include <QList>
#include <QVector3D>

#include "brainiacglobals.h"
#include "channel.h"

#include "../../gui/agent/agentinstacegeometryquick3d.h"

class Agent;

class AgentInstance : public QObject
{
    Q_OBJECT
public:
    explicit AgentInstance(QObject *parent = nullptr);

    void addInputChannel(BrainiacGlobals::BrainiacId id, Channel::ChannelDefaults *defaults);
    void addOutputChannel(BrainiacGlobals::BrainiacId id, Channel::ChannelDefaults *defaults);

    void reset();

    Agent *agent() const;

    QHash<BrainiacGlobals::BrainiacId, Channel *> inputChannels() const;

    QHash<BrainiacGlobals::BrainiacId, Channel *> outputChannels() const;

    QVector3D initialTranslation() const;
    void setInitialTranslation(const QVector3D &newInitialTranslation);

    AgentInstaceGeometryQuick3D *geometryQuick3DNode() const;
    void setGeometryQuick3DNode(AgentInstaceGeometryQuick3D *newGeometryQuick3DNode);

private:
    QVector3D m_initialTranslation;
    QVector3D m_initialRotation;
    Agent *m_agent;
    AgentInstaceGeometryQuick3D *m_geometryQuick3DNode;

    QHash<BrainiacGlobals::BrainiacId, Channel *>
        m_inputChannels; //!< List of all input channels of this agent
    QHash<BrainiacGlobals::BrainiacId, Channel *>
        m_outputChannels; //!< List of all output channels of this agent

    Q_PROPERTY(AgentInstaceGeometryQuick3D *geometryQuick3DNode READ geometryQuick3DNode WRITE
                   setGeometryQuick3DNode NOTIFY geometryQuick3DNodeChanged FINAL)

signals:
    void geometryQuick3DNodeChanged();
};

#endif // AGENTINSTANCE_H
