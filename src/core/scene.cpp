#include "scene.h"

#include <QDebug>
#include <QQmlApplicationEngine>

#include "agentinstance.h"
#include "qqmlcontext.h"
#include "src/core/simulation.h"

Scene::Scene(QObject *parent)
    : QObject{parent}
{
    setObjectName("Scene");
    m_simulation=new Simulation(this);
    m_agentInstanceSpawner = nullptr;
    m_qQmlApplicationEngine = nullptr;
}

void Scene::addAgent(Agent *agent) {
    m_agents.append(agent);
}

void Scene::addAgentInstance(AgentInstance *newAgentInstance)
{
    QPointer<AgentInstance> qp(newAgentInstance);
    QVariant qv_agentInstance = QVariant::fromValue(qp);
    QVariant qv_agentInstanceQML = QVariant::fromValue(newAgentInstance->instanceQML());
    //    qDebug() << "QVariant from scene " << qv;
    QMetaObject::invokeMethod(m_agentInstanceSpawner, "addAgentInstance", Q_ARG(QVariant, qv_agentInstance),
                              Q_ARG(QVariant, qv_agentInstanceQML));
    m_agentInstances.append(newAgentInstance);
}

QQmlApplicationEngine *Scene::qQmlApplicationEngine() const
{
    return m_qQmlApplicationEngine;
}

void Scene::setQQmlApplivationEngine(QQmlApplicationEngine *newQQmlApplicationEngine)
{
    m_qQmlApplicationEngine = newQQmlApplicationEngine;

    QQmlContext *ctx = m_qQmlApplicationEngine->rootContext();
    ctx->setContextProperty("simulation", m_simulation);

    QObject *viewer = m_qQmlApplicationEngine->rootObjects().constFirst();
    QObject *agentSpawner = nullptr;
    foreach (QObject *obj, viewer->findChildren<QObject *>("agentInstanceSpawner")) {
        agentSpawner = obj;
    }
    if (!agentSpawner) {
        qCritical() << "Could not find agentInstanceSpawner!";
    }
    m_agentInstanceSpawner = agentSpawner;
}

QObject *Scene::agentInstanceSpawner() const
{
    return m_agentInstanceSpawner;
}

Simulation *Scene::simulation() const
{
    return m_simulation;
}

QList<AgentInstance *> Scene::agentInstances() const
{
    return m_agentInstances;
}

QList<AgentInstance *> Scene::selectedAgentInstances() const
{
    return m_selectedAgentInstances;
}

void Scene::setSelectedAgentInstances(const QList<AgentInstance *> &newSelectedAgentInstances)
{
    if (m_selectedAgentInstances == newSelectedAgentInstances)
        return;
    m_selectedAgentInstances = newSelectedAgentInstances;
    emit selectedAgentInstancesChanged();
}

void Scene::setSelectedAgentInstance(AgentInstance *newSelectedAgentInstance)
{
    m_selectedAgentInstances.clear();
    m_selectedAgentInstances.append(newSelectedAgentInstance);
    emit selectedAgentInstancesChanged();
}

QList<Agent *> Scene::agents() const {
    return m_agents;
}
