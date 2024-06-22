#include "body.h"

#include "../agent.h"

Body::Body(QObject *parent)
    : QObject{parent} {
    m_agent = qobject_cast<Agent *>(parent);
}

void Body::addBone(Bone *newBone) {
    m_bones.insert(newBone->id(), newBone);
}

BoneBox *Body::addBoneBox(const BrainiacGlobals::BrainiacId id, const BrainiacGlobals::BrainiacId parentId,
                          const QString &name) {
    auto newBoneBox = new BoneBox(this, id, parentId, name);
    return newBoneBox;
}

BrainiacGlobals::BrainiacId Body::newId() const {
    BrainiacGlobals::BrainiacId newId = 1;
    foreach(BrainiacGlobals::BrainiacId id, m_bones.keys()) {
        if (newId <= id) {
            newId = id + 1;
        }
    }
    return newId;
}

QString Body::skeletonQML() const {
    // The root bone is the bone that has parentId 0.
    Bone *rootBone = nullptr;
    foreach(Bone* bone, m_bones) {
        if (bone->parentBoneId() == 0) {
            rootBone = bone;
            break;
        }
    }
    if (!rootBone) {
        return {};
    }
    QString qml;
    skeletonQmlTraverse(1, rootBone, qml);

    return qml;
}

void Body::skeletonQmlTraverse(const quint32 level, const Bone *bone, QString &qml) const {
    // const int indent = 1;
    QString tabs("\t");
    tabs = tabs.repeated(level);
    qml += QString("\n") + tabs + QString("Node { \n " + tabs.repeated(2) + "id: ") + bone->objectName() + QString(
                "\n " + tabs.repeated(2) + "position: Qt.vector3d(")
            + QString("%1, %2, %3)").arg(
                QString::number(bone->translation().x()), QString::number(bone->translation().y()),
                QString::number(bone->translation().z())) + QString(
                "\n " + tabs.repeated(2) + "eulerRotation: Qt.vector3d(")
            + QString("%1, %2, %3)").arg(
                QString::number(bone->rotation().x()), QString::number(bone->rotation().y()),
                QString::number(bone->rotation().z()));
    foreach(Bone *aBone, m_bones) {
        if (aBone->parentBoneId() == bone->id()) {
            skeletonQmlTraverse(level + 1, aBone, qml);
        }
    }

    qml += QString("\n" + tabs + "}");
}

QHash<BrainiacGlobals::BrainiacId, Bone *> Body::bones() const {
    return m_bones;
}
