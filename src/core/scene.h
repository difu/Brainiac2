#ifndef SCENE_H
#define SCENE_H

#include <QObject>

class QQmlApplicationEngine;
class Agent;
class AgentInstance;
class Simulation;

/**
 * @class Scene
 * @brief The Scene class represents a scene in the application.
 *
 * The Scene class inherits from QObject and provides various methods to manage agent instances and perform simulations.
 */
class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addAgentInstance(AgentInstance *newAgentInstance);

    QQmlApplicationEngine *qQmlApplicationEngine() const;
    void setQQmlApplivationEngine(QQmlApplicationEngine *newQQmlApplivationEngine);

    QObject *agentInstanceSpawner() const;

    Simulation *simulation() const;

    QList<AgentInstance *> agentInstances() const;

    QList<AgentInstance *> selectedAgentInstances() const;
    /**
     * @brief setSelectedAgentInstances
     * sets the list of selected agentInstances.
     * 
     * @param newSelectedAgentInstances
     */
    void setSelectedAgentInstances(const QList<AgentInstance *> &newSelectedAgentInstances);
    void setSelectedAgentInstance(AgentInstance *newSelectedAgentInstance);

private:
    QQmlApplicationEngine *m_qQmlApplicationEngine;
    QObject *m_agentInstanceSpawner;
    Simulation *m_simulation;
    QList<AgentInstance *> m_agentInstances;
    QList<AgentInstance *> m_selectedAgentInstances;
    Q_PROPERTY(QList<AgentInstance *> selectedAgentInstances READ selectedAgentInstances WRITE
                   setSelectedAgentInstances NOTIFY selectedAgentInstancesChanged FINAL)

signals:
    void selectedAgentInstancesChanged();
};

#endif // SCENE_H
