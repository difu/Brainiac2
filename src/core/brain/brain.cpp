#include "brain.h"

#include <QtDebug>
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

Noise *Brain::addNoiseNode(BrainiacGlobals::BrainiacId id)
{
    if(id==0) {
        id=newId();
    }
    Noise *newNoise = new Noise(this);
    return newNoise;
}

BrainiacGlobals::BrainiacId Brain::newId() {
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
