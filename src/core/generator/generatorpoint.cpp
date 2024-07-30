#include "generatorpoint.h"

#include "locator.h"
#include "../brainiaclogger.h"

GeneratorPoint::GeneratorPoint(QObject *parent)
    : GeneratorBase{parent}, m_allignInRowsColumns(true) {
}

void GeneratorPoint::apply() {
}

void GeneratorPoint::updateLocators() {
    qCDebug(bGenerator()) << "Updating locators";
    qCDebug(bGenerator()) << this->agents().count() << "  agents registered, gap: " << this->gap() <<
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
    if (numTotalAgents() == locators().count()) {
        return;
    }
    qWarning() << "This should never happen!";
}

QVector3D GeneratorPoint::centerPoint() const {
    return m_centerPoint;
}

void GeneratorPoint::setCenterPoint(const QVector3D &newCenterPoint) {
    m_centerPoint = newCenterPoint;
}
