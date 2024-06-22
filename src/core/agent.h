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
class Body;
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

    /**
     * @brief Adds a new instance of an agent.
     *
     * This method creates a new instance of the agent using the specified locator and adds it to the agent's list of instances.
     * The locator object is used to initialize the instance's properties.
     * The method also adds the input and output channels to the new agent instance based on the predefined defaults of the agent.
     *
     * @param locator The locator object used to initialize the properties of the new agent instance.
     *
     * @return A pointer to the newly created AgentInstance object.
     * @note The Agent is the owner/parent of the created AgentInstance object.
     */
    AgentInstance *addAgentInstance(Locator *locator);

    [[nodiscard]] AgentInstance *defaultAgentInstance() const;

    void setDefaultAgentInstance(AgentInstance *newDefaultAgentInstance);

    [[nodiscard]] Body *body() const;

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

    [[nodiscard]] QList<AgentInstance *> agentInstances() const;

    [[nodiscard]] QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> inputChannelDefaults() const;

    [[nodiscard]] QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> outputChannelDefaults() const;

    [[nodiscard]] QString fileName() const;

    void setFileName(const QString &newFileName);

    [[nodiscard]] QString name() const;

    [[nodiscard]] bool setName(const QString &newName);

    [[nodiscard]] QJsonObject toJson() const;

    [[nodiscard]] Scene *scene() const;

    void dumpChannels();

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

    Body *m_body;
    Brain *m_brain;
    Scene *m_scene;

signals:
};

#endif // AGENT_H
