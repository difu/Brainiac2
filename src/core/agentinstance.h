#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

#include <QObject>

#include <QHash>
#include <QList>
#include <QVector3D>

#include "brainiacglobals.h"
#include "channel.h"

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

private:
    QVector3D m_initialTranslation;
    QVector3D m_initialRotation;
    Agent *m_agent;

    QHash<BrainiacGlobals::BrainiacId, Channel *>
        m_inputChannels; //!< List of all input channels of this agent
    QHash<BrainiacGlobals::BrainiacId, Channel *>
        m_outputChannels; //!< List of all output channels of this agent

signals:
};

#endif // AGENTINSTANCE_H
