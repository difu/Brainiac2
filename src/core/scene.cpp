#include "scene.h"

#include <QDebug>
#include <QQmlApplicationEngine>

#include "agentinstance.h"

Scene::Scene(QObject *parent)
    : QObject{parent}
{
    setObjectName("Scene");
    m_agentInstanceSpawner = nullptr;
    m_qQmlApplicationEngine = nullptr;
}

void Scene::addAgentInstance(AgentInstance *newAgentInstance)
{
    QPointer<AgentInstance> qp(newAgentInstance);
    QVariant qv = QVariant::fromValue(qp);
    //    qDebug() << "QVariant from scene " << qv;
    QMetaObject::invokeMethod(m_agentInstanceSpawner, "addAgentInstance", Q_ARG(QVariant, qv));
}

QQmlApplicationEngine *Scene::qQmlApplicationEngine() const
{
    return m_qQmlApplicationEngine;
}

void Scene::setQQmlApplivationEngine(QQmlApplicationEngine *newQQmlApplicationEngine)
{
    m_qQmlApplicationEngine = newQQmlApplicationEngine;
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
