#ifndef BRAIN_H
#define BRAIN_H

#include <QHash>
#include <QObject>
#include <QString>

#include "../brainiacglobals.h"
#include "brain/braineditor.h"
#include "src/core/brain/noise.h"

class Agent;
class FuzzyBase;

class Brain: public QObject
{
    Q_OBJECT
public:
    Brain(QObject *parent = nullptr);

    QHash<BrainiacGlobals::BrainiacId, FuzzyBase *> fuzzies() const;

    Agent *agent() const;

    Noise *addNoiseNode(BrainiacGlobals::BrainiacId id=0);
    BrainiacGlobals::BrainiacId newId();

    BrainEditor *brainEditor() const;

private:
    Agent *m_agent;
    QHash<BrainiacGlobals::BrainiacId, FuzzyBase *> m_fuzzies;
    BrainEditor *m_brainEditor;
};

#endif // BRAIN_H
