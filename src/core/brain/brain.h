#ifndef BRAIN_H
#define BRAIN_H

#include <QHash>
#include <QObject>
#include <QString>

#include "../brainiacglobals.h"

class Agent;
class FuzzyBase;

class Brain: public QObject
{
    Q_OBJECT
public:
    Brain(QObject *parent = nullptr);

private:
    Agent *m_agent;
    QHash<BrainiacGlobals::BrainiacId, FuzzyBase *> m_fuzzies;
};

#endif // BRAIN_H
