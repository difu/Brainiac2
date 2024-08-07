#include "generatorbase.h"
#include "../scene.h"

#include <QDebug>

#include "generatorpoint.h"
#include "locator.h"
#include "src/core/brainiaclogger.h"
#include "src/core/brain/brain.h"

GeneratorBase::GeneratorBase(Scene *parent, const BrainiacGlobals::ItemType type)
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
      , m_heightVariation(0)
      , m_editorPos(0, 0)
      , m_scene(parent)
      , m_type(type) {
    m_scene->addGenerator(this);
    connect(this, SIGNAL(destroyed(QObject *)), m_scene, SLOT(generatorDeleted(QObject *)));
}

void GeneratorBase::addAgent(Agent *newAgent, const qsizetype position) {
    if (m_agents.contains(newAgent)) {
        return;
    }
    m_agents.insert(position, newAgent);
    recalculateRatios();
}

void GeneratorBase::appendAgent(Agent *newAgent) {
    if (m_agents.contains(newAgent)) {
        return;
    }
    m_agents.append(newAgent);
    recalculateRatios();
}

void GeneratorBase::removeAgent(Agent *agent) {
    m_agents.removeAll(agent);
    m_agentRatios.remove(agent);
    recalculateRatios();
    // TODO: handle locators that still referencing this agent!
}

void GeneratorBase::recalculateRatios() {
    const auto totalAgents = m_agents.size();
    if (totalAgents > 0) {
        const qreal equalRatio = (1.0 - m_gap) / totalAgents;
        m_agentRatios.clear();
        foreach(auto *agent, agents()) {
            m_agentRatios.insert(agent, equalRatio);
        }
    }
}

BrainiacGlobals::ItemType GeneratorBase::type() const {
    return m_type;
}

QPointF GeneratorBase::editorPos() const {
    return m_editorPos;
}

void GeneratorBase::setEditorPos(QPointF newEditorPos) {
    m_editorPos = newEditorPos;
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
    // if(m_rows*m_columns < m_numTotalAgents) {
    //     this->setNumTotalAgents(m_columns*m_rows);
    // }
}

quint32 GeneratorBase::columns() const {
    return m_columns;
}

void GeneratorBase::setColumns(quint32 newColumns) {
    m_columns = newColumns;
    // if(m_rows*m_columns < m_numTotalAgents) {
    //     this->setNumTotalAgents(m_columns*m_rows);
    // }
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

QList<quint32> GeneratorBase::shuffeldList(const quint32 numberOfItems) {
    if (numberOfItems > BrainiacGlobals::NUMBER_OF_SHUFFLES) {
        qFatal() << "Out of shuffle range!";
    }
    QList<quint32> shuffelIds;
    for (int i = 0; i < numberOfItems; i++) {
        shuffelIds.append(BrainiacGlobals::shuffles[i]);
    }
    int n = shuffelIds.size();
    QVector<QPair<int, int> > valueIndexPairs(n);

    // Store value and original indices as pairs
    for (int i = 0; i < n; ++i) {
        valueIndexPairs[i] = qMakePair(shuffelIds[i], i);
    }

    // Sort pairs based on values
    std::sort(valueIndexPairs.begin(),
              valueIndexPairs.end(),
              [](const QPair<int, int> &a, const QPair<int, int> &b) { return a.first < b.first; });

    // Create a ranks list initialized with zeroes
    QList<quint32> ranks(n);
    for (int rank = 0; rank < n; ++rank) {
        ranks[valueIndexPairs[rank].second] = rank;
    }

    return ranks;
}

void GeneratorBase::removeLastNLocators(const int n) {
    if (n <= m_locators.size()) {
        m_locators.erase(m_locators.end() - n, m_locators.end());
    } else {
        qDebug() << "Error: n is greater than the size of the list.";
    }
}

GeneratorBase::~GeneratorBase() {
    qCDebug(bGenerator()) << "Destructor GeneratorBase";
}

QList<Locator *> GeneratorBase::locators() const {
    return m_locators;
}

bool GeneratorBase::compare(GeneratorBase *generator1, GeneratorBase *generator2, QStringList &differences) {
    bool equal = true;
    if (generator1->type() != generator2->type()) {
        equal = false;
        differences.append("Types differs!");
    }
    if (generator1->numTotalAgents() != generator2->numTotalAgents()) {
        equal = false;
        differences.append("Num of agents differs!");
    }
    if (generator1->rows() != generator2->rows()) {
        equal = false;
        differences.append("Num of rows differs!");
    }
    if (generator1->columns() != generator2->columns()) {
        equal = false;
        differences.append("Num of columns differs!");
    }
    if (!qFuzzyCompare(generator1->angle(), generator2->angle())) {
        equal = false;
        differences.append("Angle differs!");
    }
    if (!qFuzzyCompare(generator1->angleVariation(), generator2->angleVariation())) {
        equal = false;
        differences.append("Anglevariation differs!");
    }
    if (!qFuzzyCompare(generator1->height(), generator2->height())) {
        equal = false;
        differences.append("Height differs!");
    }
    if (!qFuzzyCompare(generator1->heightVariation(), generator2->heightVariation())) {
        equal = false;
        differences.append("Height variation differs!");
    }
    auto *gPoint1 = qobject_cast<GeneratorPoint *>(generator1);
    auto *gPoint2 = qobject_cast<GeneratorPoint *>(generator2);
    if (gPoint1 && gPoint2) {
        if (!qFuzzyCompare(gPoint1->centerPoint(), gPoint2->centerPoint())) {
            differences.append("Centerpoint differs!");
            equal = false;
        }
    }
    if (generator1->agents().count() == generator2->agents().count()) {
        for (int index = 0; index < generator1->agents().count(); index++) {
            auto *agentGen1 = generator1->agents().at(index);
            auto *agentGen2 = generator2->agents().at(index);
            auto ratio1 = generator1->agentRatios().value(agentGen1);
            auto ratio2 = generator2->agentRatios().value(agentGen2);
            if (!qFuzzyCompare(ratio1, ratio2)) {
                differences.append("AgentRatios differs!");
                equal = false;
            }
        }
    } else {
        differences.append("Number of agents differs!");
        equal = false;
    }

    return equal;
}
