#ifndef LOCATOR_H
#define LOCATOR_H

#include <QObject>
#include <QVector3D>

class Agent;
class AgentInstance;

/**
 * @class Locator
 *
 * @brief The Locator class represents a locator object.
 *
 * The class provides methods to manage the state, location, rotation,
 * agent instance, and seed of a locator. The locator can have three
 * possible states: default, deleted, and locked. It also holds the
 * location and rotation of the locator in 3D space. The locator can be
 * associated with an agent and agent instance. Additionally, it holds a
 * seed value for randomization purposes.
 *
 * @ingroup GroupName
 *
 * @sa Agent, AgentInstance
 */
class Locator : public QObject
{
    Q_OBJECT

public:
    /**
     * @enum LocatorState
     *
     * @brief The LocatorState enum represents the possible states of a locator.
     *
     * The enum defines the follwing possible states for a locator: DEFAULT, DELETED, LOCKED, GAP.
     * The DEFAULT state represents the default state of a locator. The DELETED state represents
     * the state of a locator that has been deleted. The LOCKED state represents the state of a
     * locator that is locked and cannot be modified. GAP is a locator that is not instanciated because
     * of a gap ratio. If a locator is in GAP or DELETED state, no instance will be created.
     *
     * @ingroup GroupName
     *
     * @sa Locator
     */
    enum LocatorState { DEFAULT, DELETED, LOCKED, GAP };

    explicit Locator(Agent *agent, QObject *parent);

    [[nodiscard]] LocatorState locatorState() const;

    void setLocatorState(const LocatorState &newLocatorState);

    [[nodiscard]] QVector3D location() const;

    void setLocation(const QVector3D &newLocation);

    [[nodiscard]] QVector3D rotation() const;

    void setRotation(const QVector3D &newRotation);
    virtual ~Locator();

    [[nodiscard]] Agent *agent() const;

    void setAgent(Agent *newAgent);

    [[nodiscard]] bool isLocked() const;

    [[nodiscard]] AgentInstance *agentInstance() const;
    void setAgentInstance(AgentInstance *newAgentInstance);

    [[nodiscard]] quint32 seed() const;
    void setSeed(quint32 newSeed);

private:
    AgentInstance *m_agentInstance;
    Agent *m_agent;
    LocatorState m_locatorState;
    QVector3D m_location;
    QVector3D m_rotation;
    quint32 m_seed;
signals:
};

#endif // LOCATOR_H
