#include "brain.h"

#include <QtDebug>

#include "fuzzyand.h"
#include "fuzzyor.h"
#include "../agent.h"

Brain::Brain(QObject *parent)
    : QObject{parent}
{
    m_agent = qobject_cast<Agent *> (parent);
    m_brainEditor = new BrainEditor(this);
}

QHash<BrainiacGlobals::BrainiacId, FuzzyBase *> Brain::fuzzies() const
{
    return m_fuzzies;
}

Agent *Brain::agent() const
{
    return m_agent;
}

FuzzyAnd *Brain::addAndNode(BrainiacGlobals::BrainiacId id) {
    if (id == 0) {
        id = newId();
    }
    const auto newAnd = new FuzzyAnd(this, id);
    if (m_fuzzies.contains(id)) {
        qFatal() << "Fuzzy id " << id << " already exists!";
    }
    m_fuzzies.insert(id, newAnd);
    return newAnd;
}

FuzzyOr *Brain::addOrNode(BrainiacGlobals::BrainiacId id) {
    if (id == 0) {
        id = newId();
    }
    const auto newOr = new FuzzyOr(this, id);
    if (m_fuzzies.contains(id)) {
        qFatal() << "Fuzzy id " << id << " already exists!";
    }
    m_fuzzies.insert(id, newOr);
    return newOr;
}

Noise *Brain::addNoiseNode(BrainiacGlobals::BrainiacId id)
{
    if(id==0) {
        id=newId();
    }
    const auto newNoise = new Noise(this, id);
    if(m_fuzzies.contains(id)) {
        qFatal() << "Fuzzy id " << id << " already exists!";
    }
    m_fuzzies.insert(id, newNoise);
    return newNoise;
}

FuzzyOutput *Brain::addOutputNode(BrainiacGlobals::BrainiacId id)
{
    if(id==0) {
        id=newId();
    }
    const auto newOutput = new FuzzyOutput(this, id);
    if(m_fuzzies.contains(id)) {
        qFatal() << "Fuzzy id " << id << " already exists!";
    }
    m_fuzzies.insert(id, newOutput);
    return newOutput;
}

BrainiacGlobals::BrainiacId Brain::newId() const {
    BrainiacGlobals::BrainiacId newId=1;
    foreach(BrainiacGlobals::BrainiacId id, m_fuzzies.keys() ) {
        if(newId <= id) {
            newId = id+1;
        }
    }
    return newId;
}


BrainEditor *Brain::brainEditor() const
{
    return m_brainEditor;
}

void Brain::compare(Brain *brain1, Brain *brain2, QStringList &differences) {
    if (brain1->fuzzies().count() != brain2->fuzzies().count()) {
        differences.append(QString("Different number of fuzzies"));
    } else {
        foreach(auto *fuzz, brain1->fuzzies()) {
            bool otherFuzzFound = false;
            foreach(auto *otherFuzz, brain2->fuzzies()) {
                if (otherFuzz->type() == fuzz->type() && otherFuzz->name() == fuzz->name()) {
                    otherFuzzFound = true;
                    if (!(fuzz->editorPos() == otherFuzz->editorPos())) {
                        differences.append(QString("EditorPos differs: ").append(std::to_string(fuzz->id())));
                    }
                }
            }
            if (!otherFuzzFound) {
                differences.append(QString("No corresponding fuzz found: ").append(std::to_string(fuzz->id())));
            }
        }
    }
}
