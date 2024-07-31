#include "locator.h"

#include "../agent.h"
#include "../agentinstance.h"

Locator::Locator(Agent *agent, QObject *parent)
    : QObject{parent}
{
    m_agent = agent;
    m_agentInstance = nullptr;
    m_locatorState = Locator::DEFAULT;
    m_seed = 0;
}

Locator::LocatorState Locator::locatorState() const
{
    return m_locatorState;
}

void Locator::setLocatorState(const LocatorState &newLocatorState)
{
    m_locatorState = newLocatorState;
}

QVector3D Locator::location() const
{
    return m_location;
}

void Locator::setLocation(const QVector3D &newLocation)
{
    if(m_locatorState!=LocatorState::LOCKED) {
        m_location = newLocation;
    }
}

QVector3D Locator::rotation() const
{
    return m_rotation;
}

void Locator::setRotation(const QVector3D &newRotation)
{
    if(m_locatorState!=LocatorState::LOCKED) {
        m_rotation = newRotation;
    }
}

Locator::~Locator()
{
    if (m_agentInstance) {
        m_agentInstance->deleteLater();
    }
}

Agent *Locator::agent() const
{
    return m_agent;
}

void Locator::setAgent(Agent *newAgent)
{
    if (m_locatorState != LocatorState::LOCKED) {
        m_agent = newAgent;
    }
}

bool Locator::isLocked() const
{
    return m_locatorState == Locator::LOCKED;
}

AgentInstance *Locator::agentInstance() const
{
    return m_agentInstance;
}

void Locator::setAgentInstance(AgentInstance *newAgentInstance)
{
    if(m_locatorState!=LocatorState::LOCKED) {
        m_agentInstance = newAgentInstance;
    }
}

quint32 Locator::seed() const
{
    return m_seed;
}

void Locator::setSeed(quint32 newSeed)
{
    if(m_locatorState!=LocatorState::LOCKED) {
        m_seed = newSeed;
    }
}
