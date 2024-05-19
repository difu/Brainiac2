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
class AgentInstanceBrain;
class Locator;

class AgentInstance : public QObject
{
    Q_OBJECT
public:
    explicit AgentInstance(Locator *locator, Agent *parent = nullptr);

    void addInputChannel(BrainiacGlobals::BrainiacId id, Channel::ChannelDefaults *defaults);
    void addOutputChannel(BrainiacGlobals::BrainiacId id, Channel::ChannelDefaults *defaults);

    void advance();
    void advanceCommit();
    void reset();

    Agent *agent() const;

    QHash<BrainiacGlobals::BrainiacId, Channel *> inputChannels() const;
    QHash<BrainiacGlobals::BrainiacId, Channel *> outputChannels() const;

    AgentInstaceGeometryQuick3D *geometryQuick3DNode() const;
    void setGeometryQuick3DNode(AgentInstaceGeometryQuick3D *newGeometryQuick3DNode);

    Locator *locator() const;

    QVector3D translation() const;
    void setTranslation(const QVector3D &newTranslation);

    QVector3D rotation() const;
    void setRotation(const QVector3D &newRotation);

private:
    Agent *m_agent;
    AgentInstanceBrain *m_instanceBrain;

public:
    [[nodiscard]] AgentInstanceBrain * instanceBrain() const {
        return m_instanceBrain;
    }

private:
    AgentInstaceGeometryQuick3D *m_geometryQuick3DNode;
    Locator *m_locator;
    QVector3D m_translation;
    QVector3D m_newTranslation;
    QVector3D m_rotation;
    QVector3D m_newRotation;

    QHash<BrainiacGlobals::BrainiacId, Channel *>
        m_inputChannels; //!< List of all input channels of this agent
    QHash<BrainiacGlobals::BrainiacId, Channel *>
        m_outputChannels; //!< List of all output channels of this agent

    Q_PROPERTY(AgentInstaceGeometryQuick3D *geometryQuick3DNode READ geometryQuick3DNode WRITE
                   setGeometryQuick3DNode NOTIFY geometryQuick3DNodeChanged FINAL)

    Q_PROPERTY(
        QVector3D translation READ translation WRITE setTranslation NOTIFY translationChanged FINAL)

signals:
    void geometryQuick3DNodeChanged();
    void translationChanged();
    void rotationChanged();
};

#endif // AGENTINSTANCE_H
