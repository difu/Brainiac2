#include "generatorbase.h"
#include "../scene.h"

#include <QDebug>

GeneratorBase::GeneratorBase(QObject *parent)
    : QObject{parent}, m_numTotalAgents(0) {
}

void GeneratorBase::addAgent(Agent *newAgent, const qsizetype position) {
    if (m_agents.contains(newAgent)) {
        return;
    }
    m_agents.insert(position, newAgent);
    recalculateRatios();
}

void GeneratorBase::removeAgent(Agent *agent) {
    m_agents.removeAll(agent);
    m_agentRatios.remove(agent);
    recalculateRatios();
}


void GeneratorBase::recalculateRatios() {
    const auto totalAgents = m_agents.size();
    if (totalAgents > 0) {
        const qreal equalRatio = 1.0 / totalAgents;
        for (auto it = m_agentRatios.begin(); it != m_agentRatios.end(); ++it) {
            it.value() = equalRatio;
        }
    }
}

Scene *GeneratorBase::scene()
{
    Scene *scene = qobject_cast<Scene *>(this->parent());
    if (scene) {
        return scene;
    }
    qCritical() << "parent is not a Scene!";
    return nullptr;
}

GeneratorBase::~GeneratorBase() {}

QList<Locator *> GeneratorBase::locators() const {
    return m_locators;
}
