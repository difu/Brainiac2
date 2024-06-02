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
