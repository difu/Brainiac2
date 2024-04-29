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

private:
    QQmlApplicationEngine *m_qQmlApplicationEngine;
    QObject *m_agentInstanceSpawner;
    Simulation *m_simulation;
signals:
};

#endif // SCENE_H
