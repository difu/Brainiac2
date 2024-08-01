#include "generatorpoint.h"

#include "locator.h"
#include "../brainiaclogger.h"
#include "src/core/agent.h"

GeneratorPoint::GeneratorPoint(QObject *parent)
    : GeneratorBase{parent}, m_allignInRowsColumns(true) {
}

void GeneratorPoint::instanciateAgentInstances() {
    foreach(Locator *loc, m_locators) {
        if (loc->agent()) {
            Agent *agent = loc->agent();
            if (!loc->isLocked() && loc->agentInstance() == nullptr) {
                AgentInstance *inst = agent->addAgentInstance(loc);
                loc->setAgentInstance(inst);
                //this->scene()->addAgentInstance(inst);
            }
        } else {
            qCWarning(bGenerator()) << "No Agent in Locator found!";
        }
    }
}

void GeneratorPoint::updateLocators() {
    qCDebug(bGenerator()) << "Updating locators";
    qCDebug(bGenerator()) << this->agents().count() << "agents registered, gap: " << this->gap() <<
 ", total number of agents:" << this->numTotalAgents();
    const quint32 numAgents = this->numTotalAgents();
    const quint32 numLocators = this->locators().count();
    if (numAgents > numLocators) {
        qCDebug(bGenerator()) << "Too few locators, creating more.";
        for (int i = 0; i < numAgents - numLocators; i++) {
            auto locator = new Locator(nullptr, this);
            m_locators.append(locator);
        }
    }
    if (numAgents < numLocators) {
        qCDebug(bGenerator()) << "Too many locators, deleting some.";
        this->removeLastNLocators(numLocators - numAgents);
    }

    if (numTotalAgents() == locators().count()) {
        qCDebug(bGenerator()) << "Number of locators matches the number of requested agent instances.";
        this->alignLocatorsInRowsCols();
        int actualInstance = 0;
        QList<quint32> locatorOrder = this->shuffeldList(m_locators.count());

        foreach(auto *agent, agents()) {
            const int numberOfInstances = this->agentRatios().value(agent) * m_locators.count();
            qCDebug(bGenerator()) << "Assigning locators for agent " << agent->name() << ", ratio " << this->
agentRatios().value(agent) << ", numOfInstances:" << numberOfInstances;
            for (int i = 0; i < numberOfInstances; i++) {
                auto *locator = m_locators.at(locatorOrder.at(actualInstance));
                locator->setAgent(agent);
                actualInstance++;
            }
        }
        qCDebug(bGenerator()) << "Processed" << actualInstance << "instances for" << m_locators.count() << "locators.";
        if (actualInstance <= m_locators.count()) {
            qCWarning(bGenerator()) << "TODO: Add additional filling instance";
        }

        return;
    }
    qWarning() << "This should never happen, we have still too few or many locators!";
}

void GeneratorPoint::alignLocatorsInRowsCols() {
    if (this->rows() * this->columns() < m_locators.count()) {
        qCCritical(bGenerator()) << "Too few locators for configured rows and cols.";
    }
    const int numLocatorsToProcess = m_locators.count();
    int currentLocatorIndex = 0;
    qCDebug(bGenerator()) << "Rows " << rows() << ", cols " << columns() << " #locators to process " <<
 numLocatorsToProcess;

    const float halfRow = rows() / 2;
    const float halfCol = columns() / 2;
    for (int row = 0; row < rows(); row++) {
        for (int col = 0; col < columns(); col++) {
            if (currentLocatorIndex >= numLocatorsToProcess) {
                break;
            }
            auto *locator = m_locators.at(currentLocatorIndex);
            if (locator->locatorState() != Locator::LOCKED) {
                const float xPos = (col - halfCol) * distance() - m_centerPoint.x() / 2;
                const float yPos = 0 - m_centerPoint.y() / 2;
                const float zPos = (row - halfRow) * distance() - m_centerPoint.z() / 2;
                auto pos = QVector3D(xPos,
                                     yPos,
                                     zPos);
                locator->setLocation(pos);
            }
            currentLocatorIndex++;
        }
    }
    qCDebug(bGenerator()) << "Processed" << currentLocatorIndex << "locators.";
}

QVector3D GeneratorPoint::centerPoint() const {
    return m_centerPoint;
}

void GeneratorPoint::setCenterPoint(const QVector3D &newCenterPoint) {
    m_centerPoint = newCenterPoint;
}
