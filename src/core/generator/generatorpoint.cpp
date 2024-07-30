#include "generatorpoint.h"

#include "locator.h"
#include "../brainiaclogger.h"
#include "src/core/agent.h"

GeneratorPoint::GeneratorPoint(QObject *parent)
    : GeneratorBase{parent}, m_allignInRowsColumns(true) {
}

void GeneratorPoint::apply() {
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
        int numInstancesProcessed = 0;
        QList<quint32> locatorOrder = this->shuffeldList(this->numTotalAgents());

        foreach(auto *agent, agents()) {
            qCDebug(bGenerator()) << "Assigning locators for agent " << agent->name() << ", ratio " << this->
agentRatios().value(agent);
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
    for (int row = 0; row < rows(); row++) {
        for (int col = 0; col < columns(); col++) {
            if (currentLocatorIndex >= numLocatorsToProcess) {
                break;
            }
            auto *locator = m_locators.at(currentLocatorIndex);
            if (locator->locatorState() != Locator::LOCKED) {
                auto pos = QVector3D(col * distance() - m_centerPoint.x() / 2, row * distance() - m_centerPoint.y() / 2,
                                     0 - m_centerPoint.z() / 2);
                locator->setLocation(pos);
            }
            currentLocatorIndex++;
        }
    }
}

QVector3D GeneratorPoint::centerPoint() const {
    return m_centerPoint;
}

void GeneratorPoint::setCenterPoint(const QVector3D &newCenterPoint) {
    m_centerPoint = newCenterPoint;
}
