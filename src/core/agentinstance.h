#ifndef AGENTINSTANCE_H
#define AGENTINSTANCE_H

#include <QObject>

#include <QHash>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QVector3D>

#include "brainiacglobals.h"
#include "channel.h"

#include "../../gui/agent/agentinstacegeometryquick3d.h"
#include "brain/brain.h"

class Agent;
class AgentInstanceBody;
class AgentInstanceBrain;
class Locator;

/**
 * @class AgentInstance
 *
 * @brief The AgentInstance class represents an instance of an Agent.
 *
 * An AgentInstance is created by the Locator and is used to hold information about the specific instance
 * of an Agent in the simulation. It contains the agent's input and output channels, as well as
 * information about its position in 3D space.
 *
 * @note This class inherits from QObject to enable the use of signals and slots.
 */
class AgentInstance : public QObject
{
    Q_OBJECT
public:
    explicit AgentInstance(Locator *locator, Agent *parent = nullptr);

    void addInputChannel(BrainiacGlobals::BrainiacId id, Channel::ChannelDefaults *defaults);
    void addOutputChannel(BrainiacGlobals::BrainiacId id, Channel::ChannelDefaults *defaults);

    /**
     * @brief Advance the AgentInstance.
     *
     * This method is called to advance the AgentInstance in the simulation. It updates the rotation and translation
     * of the AgentInstance based on the values of the output channels. The new rotation and translation values are calculated
     * using the current rotation, translation, and output channel values.
     *
     * @note This method assumes that the Quick3d node is ready. If the Quick3d node is null, these methods will not have any effect.
     */
    void advance();

    /**
     * @brief Commit the changes made during the advance step.
     *
     * This method is called at the end of the advance step to commit the changes made to the agent's rotation and translation.
     * The new rotation and translation values are set using the setRotation() and setTranslation() methods, respectively.
     * After committing the changes, the agent's instance brain is invalidated using the invalidateAll() method.
     *
     * @note This method assumes that the Quick3d node is ready. If the Quick3d node is null, these methods will not have any effect.
     */
    void advanceCommit();
    void reset();

    Agent *agent() const;

    [[nodiscard]] QHash<BrainiacGlobals::BrainiacId, Channel *> inputChannels() const;

    [[nodiscard]] QHash<BrainiacGlobals::BrainiacId, Channel *> outputChannels() const;

    /**
     * @brief Returns the geometryQuick3DNode of the AgentInstance.
     *
     * This method returns the geometryQuick3DNode of the AgentInstance, which is an instance of the AgentInstaceGeometryQuick3D class.
     * The geometryQuick3DNode represents the geometry of the AgentInstance in a Quick3D scene. It is used to hold information about the
     * position, rotation, and other properties of the AgentInstance in 3D space.
     *
     * @note This method does not create a new instance of AgentInstaceGeometryQuick3D. It simply returns the existing instance.
     * @return The geometryQuick3DNode of the AgentInstance.
     */
    [[nodiscard]] auto geometryQuick3DNode() const -> AgentInstaceGeometryQuick3D *;

    /**
     * @brief Returns the QML code for the instance of the Agent.
     *
     * This method reads the QML code from the file ":gui/AgentInstance.qml" and returns it as a QString.
     * The QML code represents the visual representation of the AgentInstance in the 3D view.
     *
     * @note If the file cannot be opened, qFatal() is called to terminate the application.
     * @return The QML code for the instance of the Agent.
     */
    [[nodiscard]] QString instanceQML() const;

    void setGeometryQuick3DNode(AgentInstaceGeometryQuick3D *newGeometryQuick3DNode);

    [[nodiscard]] Locator *locator() const;

    QVector3D translation() const;
    void setTranslation(const QVector3D &newTranslation);

    void setTranslationWithForcedEmit(const QVector3D &newTranslation);

    QVector3D rotation() const;
    void setRotation(const QVector3D &newRotation);

    void setRotationithForcedEmit(const QVector3D &newRotation);

private:
    Agent *m_agent;
    AgentInstanceBrain *m_instanceBrain;
    AgentInstanceBody *m_agentInstanceBody;

    static QMutex m_instanceIdMutex;
    static BrainiacGlobals::BrainiacId m_uniqueAgentInstanceId;

public:
    [[nodiscard]] AgentInstanceBrain *instanceBrain() const { return m_instanceBrain; }

private:
    BrainiacGlobals::BrainiacId m_id;

    static BrainiacGlobals::BrainiacId uniqueAgentInstanceId();

public:
    [[nodiscard]] BrainiacGlobals::BrainiacId id() const;

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
