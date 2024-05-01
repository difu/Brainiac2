#ifndef SCENE_H
#define SCENE_H

#include <QObject>

class QQmlApplicationEngine;
class AgentInstance;
class Simulation;

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

private:
    QQmlApplicationEngine *m_qQmlApplicationEngine;
    QObject *m_agentInstanceSpawner;
    Simulation *m_simulation;
    QList<AgentInstance *> m_agentInstances;
signals:
};

#endif // SCENE_H
