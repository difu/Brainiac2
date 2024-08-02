#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "brainiacglobals.h"
#include "scenereaderwriter.h"
#include "src/core/generator/generatorbase.h"

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

    virtual ~Scene();

    auto addAgent(Agent *agent) -> void;

    /**
     * @brief agents
     * Returns the list of agents in the scene.
     *
     * This function returns a QList of Agent pointers representing the agents in the scene.
     *
     * @return The list of agents in the scene.
     */
    [[nodiscard]] QList<Agent *> agents() const;

    void addAgentInstance(AgentInstance *newAgentInstance);

    [[nodiscard]] QQmlApplicationEngine *qQmlApplicationEngine() const;

    void setQQmlApplivationEngine(QQmlApplicationEngine *newQQmlApplivationEngine);

    [[nodiscard]] QObject *agentInstanceSpawner() const;

    [[nodiscard]] Simulation *simulation() const;

    [[nodiscard]] QList<AgentInstance *> agentInstances() const;

    [[nodiscard]] QList<AgentInstance *> selectedAgentInstances() const;

    /**
     * @brief setSelectedAgentInstances
     * sets the list of selected agentInstances.
     * 
     * @param newSelectedAgentInstances
     */
    void setSelectedAgentInstances(const QList<AgentInstance *> &newSelectedAgentInstances);
    void setSelectedAgentInstance(AgentInstance *newSelectedAgentInstance);

    void addGenerator(GeneratorBase *generator);

    [[nodiscard]] QList<GeneratorBase *> generators() const;

    [[nodiscard]] QString fileName() const;

    void setFileName(const QString &newFileName);

public slots:
    void generatorDeleted(GeneratorBase *delGenerator);

private:
    QQmlApplicationEngine *m_qQmlApplicationEngine;
    QObject *m_agentInstanceSpawner;
    Simulation *m_simulation;
    QList<Agent *> m_agents;
    QList<AgentInstance *> m_agentInstances;
    QList<AgentInstance *> m_selectedAgentInstances;

    QList<GeneratorBase *> m_generators;
    Q_PROPERTY(QList<AgentInstance *> selectedAgentInstances READ selectedAgentInstances WRITE
                   setSelectedAgentInstances NOTIFY selectedAgentInstancesChanged FINAL)
    SceneReaderWriter *m_sceneReaderWriter;
    QString m_fileName;

signals:
    void selectedAgentInstancesChanged();
};

#endif // SCENE_H
