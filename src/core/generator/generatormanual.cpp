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

bool GeneratorManual::compare(GeneratorManual *generator1, GeneratorManual *generator2, QStringList &differences) {
    bool equal = true;
    if (generator1->type() != generator2->type()) {
        equal = false;
        differences.append("Types differs!");
    }
    if (generator1->numTotalAgents() != generator2->numTotalAgents()) {
        equal = false;
        differences.append("Num of agents differs!");
    }
    if (generator1->rows() != generator2->rows()) {
        equal = false;
        differences.append("Num of rows differs!");
    }
    if (generator1->columns() != generator2->columns()) {
        equal = false;
        differences.append("Num of columns differs!");
    }
    if (!qFuzzyCompare(generator1->angle(), generator2->angle())) {
        equal = false;
        differences.append("Angle differs!");
    }
    if (!qFuzzyCompare(generator1->angleVariation(), generator2->angleVariation())) {
        equal = false;
        differences.append("Anglevariation differs!");
    }
    if (!qFuzzyCompare(generator1->height(), generator2->height())) {
        equal = false;
        differences.append("Height differs!");
    }
    if (!qFuzzyCompare(generator1->heightVariation(), generator2->heightVariation())) {
        equal = false;
        differences.append("Height variation differs!");
    }
    auto *gPoint1 = qobject_cast<GeneratorPoint *>(generator1);
    auto *gPoint2 = qobject_cast<GeneratorPoint *>(generator2);
    if (gPoint1 && gPoint2) {
        if (!qFuzzyCompare(gPoint1->centerPoint(), gPoint2->centerPoint())) {
            differences.append("Centerpoint differs!");
            equal = false;
        }
    }
    if (generator1->agents().count() == generator2->agents().count()) {
        for (int index = 0; index < generator1->agents().count(); index++) {
            auto *agentGen1 = generator1->agents().at(index);
            auto *agentGen2 = generator2->agents().at(index);
            auto ratio1 = generator1->agentRatios().value(agentGen1);
            auto ratio2 = generator2->agentRatios().value(agentGen2);
            if (!qFuzzyCompare(ratio1, ratio2)) {
                differences.append("AgentRatios differs!");
                equal = false;
            }
        }
    } else {
        differences.append("Number of agents differs!");
        equal = false;
    }

    return equal;
}
