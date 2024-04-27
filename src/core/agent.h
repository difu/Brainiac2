#ifndef AGENT_H
#define AGENT_H

#include <QObject>

#include <QHash>
#include <QJsonObject>
#include <QList>
#include <QString>

#include "brainiacglobals.h"
#include "channel.h"

class AgentInstance;
class Brain;
class Locator;

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(QObject *parent = nullptr);

    QString m_name;

    AgentInstance *addAgentInstance(Locator *locator);

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

    bool load();
    bool save();

    QList<AgentInstance *> agentInstances() const;

    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> inputChannelDefaults() const;
    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> outputChannelDefaults() const;

    QString fileName() const;
    void setFileName(const QString &newFileName);

    QString name() const;
    void setName(const QString &newName);

    QJsonObject toJson() const;

protected:
private:
    AgentInstance *m_defaultAgentInstance;
    QList<AgentInstance*> m_agentInstances;

    QHash<QString, BrainiacGlobals::BrainiacId> m_inputChannels;
    QHash<QString, BrainiacGlobals::BrainiacId> m_outputChannels;

    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> m_inputChannelDefaults;
    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults> m_outputChannelDefaults;

    QString m_fileName;

    Brain *m_brain;

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

signals:
    void nameChanged();
};

#endif // AGENT_H
