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

Noise *Brain::addNoiseNode()
{
    Noise *newNoise = new Noise(this);
    return newNoise;
}

BrainEditor *Brain::brainEditor() const
{
    return m_brainEditor;
}
