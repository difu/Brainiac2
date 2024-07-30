#include "generatorpoint.h"

GeneratorPoint::GeneratorPoint(QObject *parent)
    : GeneratorBase{parent}
{}

void GeneratorPoint::apply() {
}

void GeneratorPoint::updateLocators() {
    if (numTotalAgents() == locators().count()) {
    }
}

QVector3D GeneratorPoint::centerPoint() const {
    return m_centerPoint;
}

void GeneratorPoint::setCenterPoint(const QVector3D &newCenterPoint) {
    m_centerPoint = newCenterPoint;
}
