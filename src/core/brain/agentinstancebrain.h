#ifndef AGENTINSTANCEBRAIN_H
#define AGENTINSTANCEBRAIN_H

#include <QObject>

#include "../brainiacglobals.h"

class Brain;
class AgentInstance;

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

private:
    QHash<BrainiacGlobals::BrainiacId, qreal> m_fuzzyResults;
    AgentInstance *m_agentInstance;
    Brain *m_brain;
signals:
};

#endif // AGENTINSTANCEBRAIN_H
