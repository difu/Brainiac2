#include "bone.h"
#include "body.h"

Bone::Bone(QObject *parent, const BrainiacGlobals::ItemType type, const BrainiacGlobals::BrainiacId id,
           const BrainiacGlobals::BrainiacId parentId, const QString &name)
    : QObject{parent}, m_type(type), m_id(id), m_parent(parentId) {
    m_body = qobject_cast<Body *>(parent);
    if (!m_body) {
        qFatal() << "parent must be of type Body!";
    }
    if (id == 0) {
        qFatal() << "Id must not be 0, reserved as root!";
    }
    setBoneName(name);
    m_body->addBone(this);
}

BrainiacGlobals::ItemType Bone::type() const {
    return m_type;
}

BrainiacGlobals::BrainiacId Bone::id() const {
    return m_id;
}

BrainiacGlobals::BrainiacId Bone::parentBoneId() const {
    return m_parent;
}

QString Bone::setBoneName(const QString &newName) {
    if (newName.isEmpty()) {
        this->setBoneName(QString("newBone.").append(QString::number(this->id())));
    }
    foreach(Bone * bone, m_body->bones()) {
        if (bone == this) {
            continue;
        }
        if (bone->objectName() == newName) {
            this->setBoneName(QString(newName).append(QString::number(this->id())));
        }
    }

    this->setObjectName(newName);
    return newName;
}

QVector3D Bone::translation() const {
    return m_translation;
}

void Bone::setTranslation(const QVector3D &translation) {
    m_translation = translation;
}

QVector3D Bone::rotation() const {
    return m_rotation;
}

void Bone::setRotation(const QVector3D &rotation) {
    m_rotation = rotation;
}

Body *Bone::body() const {
    return m_body;
}
