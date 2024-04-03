#ifndef AGENT_H
#define AGENT_H

#include <QObject>

#include <QHash>
#include <QList>
#include <QString>

#include "brainiacglobals.h"
#include "channel.h"

class AgentInstance;
class Brain;

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(QObject *parent = nullptr);


    AgentInstance *defaultAgentInstance() const;
    void setDefaultAgentInstance(AgentInstance *newDefaultAgentInstance);

    Brain *brain() const;

    BrainiacGlobals::BrainiacId addInputChannel(const QString channelName,
                                                Channel::ChannelDefaults defaults,
                                                BrainiacGlobals::BrainiacId id = 0);

    BrainiacGlobals::BrainiacId addOutputChannel(const QString channelName,
                                                 Channel::ChannelDefaults defaults,
                                                 BrainiacGlobals::BrainiacId id = 0);
    quint32 numInputChannels() const;
    quint32 numOutputChannels() const;

    QString inputChannelName(const BrainiacGlobals::BrainiacId) const;
    QString outputChannelName(const BrainiacGlobals::BrainiacId) const;

    QList<AgentInstance *> agentInstances() const;

    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> inputChannelDefaults() const;
    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> outputChannelDefaults() const;

protected:
private:
    AgentInstance *m_defaultAgentInstance;
    QList<AgentInstance*> m_agentInstances;

    QHash<QString, BrainiacGlobals::BrainiacId> m_inputChannels;
    QHash<QString, BrainiacGlobals::BrainiacId> m_outputChannels;

    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> m_inputChannelDefaults;
    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> m_outputChannelDefaults;

    Brain *m_brain;

signals:
};

#endif // AGENT_H
