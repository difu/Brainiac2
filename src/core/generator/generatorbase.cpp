#include "generatorbase.h"
#include "../scene.h"

#include <QDebug>

GeneratorBase::GeneratorBase(QObject *parent)
    : QObject{parent}
      , m_numTotalAgents(0)
      , m_gap(0)
      , m_rows(1)
      , m_columns(1)
      , m_distance(1.0)
      , m_noise(0)
      , m_angle(0.0)
      , m_angleVariation(0.0)
      , m_height(0.0)
      , m_heightVariation(0) {
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
        const qreal equalRatio = (1.0 - m_gap) / totalAgents;
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

quint32 GeneratorBase::rows() const {
    return m_rows;
}

void GeneratorBase::setRows(quint32 newRows) {
    m_rows = newRows;
    if(m_rows*m_columns < m_numTotalAgents) {
        this->setNumTotalAgents(m_columns*m_rows);
    }
}

quint32 GeneratorBase::columns() const {
    return m_columns;
}

void GeneratorBase::setColumns(quint32 newColumns) {
    m_columns = newColumns;
    if(m_rows*m_columns < m_numTotalAgents) {
        this->setNumTotalAgents(m_columns*m_rows);
    }
}

quint32 GeneratorBase::numTotalAgents() const {
    return m_numTotalAgents;
}

void GeneratorBase::setNumTotalAgents(quint32 newNumTotalAgents) {
    m_numTotalAgents = newNumTotalAgents;
}

QHash<Agent *, qreal> GeneratorBase::agentRatios() const {
    return m_agentRatios;
}

qreal GeneratorBase::distance() const {
    return m_distance;
}

void GeneratorBase::setDistance(qreal newDistance) {
    m_distance = newDistance;
}

qreal GeneratorBase::noise() const {
    return m_noise;
}

void GeneratorBase::setNoise(qreal newNoise) {
    m_noise = newNoise;
}

qreal GeneratorBase::angle() const {
    return m_angle;
}

void GeneratorBase::setAngle(qreal newAngle) {
    m_angle = newAngle;
}

qreal GeneratorBase::angleVariation() const {
    return m_angleVariation;
}

void GeneratorBase::setAngleVariation(qreal newAngleVariation) {
    m_angleVariation = newAngleVariation;
}

qreal GeneratorBase::height() const {
    return m_height;
}

void GeneratorBase::setHeight(qreal newHeight) {
    m_height = newHeight;
}

qreal GeneratorBase::heightVariation() const {
    return m_heightVariation;
}

void GeneratorBase::setHeightVariation(qreal newHeightVariation) {
    m_heightVariation = newHeightVariation;
}

qreal GeneratorBase::gap() const {
    return m_gap;
}

/**
 * @brief Setter method for the gap.
 * The value should be between 0.0 and 1.0.
 *
 * @param newGap The new gap value to set. Must be between 0.0 and 1.0.
 */
void GeneratorBase::setGap(qreal newGap) {
    m_gap = qBound(0.0, newGap, 1.0);
    recalculateRatios();
}

QList<Agent *> GeneratorBase::agents() const
{
    return m_agents;
}

GeneratorBase::~GeneratorBase() {}

QList<Locator *> GeneratorBase::locators() const {
    return m_locators;
}
