#include "generatorbase.h"
#include "../scene.h"

#include <QDebug>

GeneratorBase::GeneratorBase(QObject *parent)
    : QObject{parent}
{}

Scene *GeneratorBase::scene()
{
    Scene *scene = qobject_cast<Scene *>(this->parent());
    if (scene) {
        return scene;
    }
    qCritical() << "parent is not a Scene!";
    return nullptr;
}

GeneratorBase::~GeneratorBase() {}

QList<Locator *> GeneratorBase::locators() const {
    return m_locators;
}
