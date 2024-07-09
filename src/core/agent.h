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
class AgentReaderWriter;
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
     * Also, the instance is added to the scene. When added to the scene, the AgentInstaceGeometryQuick3D is set
     * and ready to use in the instance.
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

    [[nodiscard]] Brain *brain() const;

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

    [[nodiscard]] quint32 numInputChannels() const;

    [[nodiscard]] quint32 numOutputChannels() const;

    [[nodiscard]] QString inputChannelName(BrainiacGlobals::BrainiacId) const;

    [[nodiscard]] QString outputChannelName(BrainiacGlobals::BrainiacId) const;

    bool load();
    bool save();

    [[nodiscard]] QList<AgentInstance *> agentInstances() const;

    [[nodiscard]] QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> inputChannelDefaults() const;

    [[nodiscard]] QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> outputChannelDefaults() const;

    [[nodiscard]] QString fileName() const;

    void setFileName(const QString &newFileName);

    [[nodiscard]] QString name() const;

    /**
     * @brief Sets the name of the agent.
     *
     * This method sets the name of the agent to the specified new name.
     * If the new name is the same as the current name, the method returns true without making any changes.
     * If the new name is already assigned to another agent in the scene, the method returns false without making any changes.
     *
     * @param newName The new name for the agent.
     * @return True if the name was successfully changed, false otherwise.
     *
     * @note The name is used to identify and distinguish agents in the scene.
     * If the name is not unique within the scene, it can lead to conflicts and unexpected behavior.
     */
    [[nodiscard]] bool setName(const QString &newName);

    [[nodiscard]] QJsonObject toJson() const;

    [[nodiscard]] Scene *scene() const;

    void dumpChannels();

    ~Agent() override;

protected:
    [[nodiscard]] AgentReaderWriter *agentReaderWriter() const;

private:
    AgentInstance *m_defaultAgentInstance;
    QList<AgentInstance*> m_agentInstances;

    QHash<QString, BrainiacGlobals::BrainiacId> m_inputChannels;
    QHash<QString, BrainiacGlobals::BrainiacId> m_outputChannels;

    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> m_inputChannelDefaults;
    QHash<BrainiacGlobals::BrainiacId, Channel::ChannelDefaults *> m_outputChannelDefaults;

    QString m_fileName;

    AgentReaderWriter *m_agentReaderWriter;
    Body *m_body;
    Brain *m_brain;
    Scene *m_scene;

signals:
};

#endif // AGENT_H
