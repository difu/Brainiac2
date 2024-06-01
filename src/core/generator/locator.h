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
    enum LocatorState { DEFAULT, DELETED, LOCKED };
    explicit Locator(Agent *agent, QObject *parent);

    LocatorState locatorState() const;
    void setLocatorState(const LocatorState &newLocatorState);

    QVector3D location() const;
    void setLocation(const QVector3D &newLocation);

    QVector3D rotation() const;
    void setRotation(const QVector3D &newRotation);
    virtual ~Locator();

    Agent *agent() const;
    void setAgent(Agent *newAgent);

    bool isLocked() const;

    AgentInstance *agentInstance() const;
    void setAgentInstance(AgentInstance *newAgentInstance);

    quint32 seed() const;
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
