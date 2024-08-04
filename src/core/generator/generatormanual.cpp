#include "generatormanual.h"

#include "generatorpoint.h"
#include "locator.h"

#include "../agent.h"
#include "../agentinstance.h"
#include "../scene.h"
#include "src/core/brain/brain.h"

GeneratorManual::GeneratorManual(Scene *parent)
    : GeneratorBase{parent, BrainiacGlobals::GENERATORMANUAL} {
}

Locator *GeneratorManual::addLocator(Agent *agent)
{
    Locator *loc = new Locator(agent, this);
    m_locators.append(loc);
    return loc;
}

void GeneratorManual::instanciateAgentInstances()
{
    foreach (Locator *loc, m_locators) {
        if (loc->agent()) {
            Agent *agent = loc->agent();
            if (!loc->isLocked() && loc->agentInstance() == nullptr) {
                AgentInstance *inst = agent->addAgentInstance(loc);
                loc->setAgentInstance(inst);
                //this->scene()->addAgentInstance(inst);
            }
        } else {
            qCritical() << "No Agent in Locator found!";
        }
    }
}

void GeneratorManual::updateLocators() {
}

GeneratorManual::~GeneratorManual() {}
