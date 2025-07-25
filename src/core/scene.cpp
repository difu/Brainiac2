#include "scene.h"

#include <QDebug>
#include <QQmlApplicationEngine>

#include "agent.h"
#include "agentinstance.h"
#include "qqmlcontext.h"
#include "src/core/simulation.h"

Scene::Scene(QObject *parent)
    : QObject{parent} {
    m_sceneReaderWriter = new SceneReaderWriter(this);
    setObjectName("Scene");
    m_simulation=new Simulation(this);
    m_agentInstanceSpawner = nullptr;
    m_qQmlApplicationEngine = nullptr;
}

Scene::~Scene() {
    qDebug() << "Scene destructor";
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

void Scene::addGenerator(GeneratorBase *generator) {
    m_generators.append(generator);
}

QList<GeneratorBase *> Scene::generators() const {
    return m_generators;
}

QString Scene::fileName() const {
    return m_fileName;
}

void Scene::setFileName(const QString &newFileName) {
    m_fileName = newFileName;
}

bool Scene::save() {
    bool ok = m_sceneReaderWriter->saveAsBSF();
    qWarning() << "Error while saving the scene!";
    return ok;
}

bool Scene::load() {
    return m_sceneReaderWriter->loadFromBSF();
}

bool Scene::compare(const Scene *scene1, const Scene *scene2, QStringList &differences) {
    bool equal = true;
    const auto numOfGenerators1 = scene1->generators().count();
    const auto numOfGenerators2 = scene2->generators().count();
    if (numOfGenerators1 != numOfGenerators2) {
        equal = false;
        differences.append("Different number of generators!");
    } else {
        for (int index = 0; index < numOfGenerators1; index++) {
            auto genScene1 = scene1->generators().at(index);
            auto genScene2 = scene2->generators().at(index);
            equal = GeneratorBase::compare(genScene1, genScene2, differences);
        }
    }
    return equal;
}

void Scene::generatorDeleted(QObject *delGenerator) {
    auto removed = m_generators.removeAll(delGenerator);
    if (!(removed == 1)) {
        qFatal() << "Unable to remove generator from Scene!";
    }
}

QList<Agent *> Scene::agents() const {
    return m_agents;
}

Agent *Scene::agentByName(const QString &agentName) const {
    foreach(auto *agent, m_agents) {
        if (agent->name() == agentName) {
            return agent;
        }
    }
    return nullptr;
}
