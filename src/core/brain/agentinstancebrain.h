#ifndef AGENTINSTANCEBRAIN_H
#define AGENTINSTANCEBRAIN_H

#include <QObject>

#include "../brainiacglobals.h"

class Brain;
class AgentInstance;

/**
 * @class AgentInstanceBrain
 *
 * @brief The AgentInstanceBrain class represents the brain of a specific agent instance.
 *
 * It is responsible for storing and managing the fuzzy results for various fuzzy logic operations
 * performed by the associated AgentInstance.
 *
 * @note This class inherits from QObject.
 */
class AgentInstanceBrain : public QObject
{
    Q_OBJECT
public:
    explicit AgentInstanceBrain(QObject *parent);

    QHash<BrainiacGlobals::BrainiacId, qreal> fuzzyResults() const;
    void invalidateAll();
    void invalidateResult(BrainiacGlobals::BrainiacId fuzzyId);
    bool hasResult(BrainiacGlobals::BrainiacId id) const;
    void setResult(BrainiacGlobals::BrainiacId id, qreal result);

    void think() const;

private:
    QHash<BrainiacGlobals::BrainiacId, qreal> m_fuzzyResults;
    AgentInstance *m_agentInstance;
    Brain *m_brain;
signals:
};

#endif // AGENTINSTANCEBRAIN_H
