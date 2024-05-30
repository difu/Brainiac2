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

class Scene;

/**
 * @class Agent
 * @brief This class represents an agent in the system.
 *
 * The Agent class is a QObject that encapsulates the functionality of an agent in the scene.
 * It provides methods for managing agent instances, input and output channels, and other agent-related operations.
 * It also provides access to the agent's brain, scene, and other properties.
 *
 * @note This class is part of the core functionality of the system and is used extensively throughout the codebase.
 */
class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(Scene *parent);

    QString m_name;

    AgentInstance *addAgentInstance(Locator *locator);

    AgentInstance *defaultAgentInstance() const;
    void setDefaultAgentInstance(AgentInstance *newDefaultAgentInstance);

    Brain *brain() const;

    BrainiacGlobals::BrainiacId addInputChannel(const QString channelName,
                                                qreal channelMinVal,
                                                qreal channelMaxVal,
                                                qreal channelDefaultVal,
                                                BrainiacGlobals::BrainiacId id = 0);

    BrainiacGlobals::BrainiacId addOutputChannel(const QString channelName,
                                                 qreal channelMinVal,
                                                 qreal channelMaxVal,
                                                 qreal channelDefaultVal,
                                                 BrainiacGlobals::BrainiacId id = 0);
    quint32 numInputChannels() const;
    quint32 numOutputChannels() const;

    QString inputChannelName(const BrainiacGlobals::BrainiacId) const;
    QString outputChannelName(const BrainiacGlobals::BrainiacId) const;

    bool load();
    bool save();

    QList<AgentInstance *> agentInstances() const;

    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> inputChannelDefaults() const;
    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> outputChannelDefaults() const;

    QString fileName() const;
    void setFileName(const QString &newFileName);

    QString name() const;
    void setName(const QString &newName);

    QJsonObject toJson() const;

    Scene *scene() const;

    virtual ~Agent();

protected:
private:
    AgentInstance *m_defaultAgentInstance;
    QList<AgentInstance*> m_agentInstances;

    QHash<QString, BrainiacGlobals::BrainiacId> m_inputChannels;
    QHash<QString, BrainiacGlobals::BrainiacId> m_outputChannels;

    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> m_inputChannelDefaults;
    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> m_outputChannelDefaults;

    QString m_fileName;

    Brain *m_brain;
    Scene *m_scene;

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

signals:
    void nameChanged();
};

#endif // AGENT_H
