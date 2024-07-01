#include "body.h"

#include <QMatrix4x4>

#include "../agent.h"

Body::Body(QObject *parent)
    : QObject{parent} {
    m_agent = qobject_cast<Agent *>(parent);
}

void Body::addBone(Bone *newBone) {
    m_bones.insert(newBone->id(), newBone);
    m_boneOrder.append(newBone->id());
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

QString Body::skinQML() const {
    QString tabs("\t");
    constexpr int indent = 1;
    tabs = tabs.repeated(indent);
    QString qml = tabs + QString("skin: Skin {\n") + QString("id: skin0\n") + tabs.repeated(2)
                  + QString("joints: [\n");

    foreach (auto boneId, m_boneOrder) {
        auto aBone = m_bones.value(boneId);
        qml += tabs.repeated(3) + QString(aBone->objectName()) + QString(",\n");
    }
    qml += tabs.repeated(2) + QString("]\n") + tabs + QString("inverseBindPoses: [\n");

    foreach (auto boneId, m_boneOrder) {
        auto aBone = m_bones.value(boneId);
        QMatrix4x4 mat = aBone->inverseBindMatrix();
        qml += tabs.repeated(3) + QString("Qt.matrix4x4(%1, %2, %3, %4,\n").arg(
            QString::number(mat.data()[0]), QString::number(mat.data()[4]),
            QString::number(mat.data()[8]), QString::number(mat.data()[12]));
        qml += tabs.repeated(4) + QString("%1, %2, %3, %4,\n").arg(
            QString::number(mat.data()[1]), QString::number(mat.data()[5]),
            QString::number(mat.data()[9]), QString::number(mat.data()[13]));
        qml += tabs.repeated(4) + QString("%1, %2, %3, %4,\n").arg(
            QString::number(mat.data()[2]), QString::number(mat.data()[6]),
            QString::number(mat.data()[10]), QString::number(mat.data()[14]));
        qml += tabs.repeated(4) + QString("%1, %2, %3, %4),\n").arg(
            QString::number(mat.data()[3]), QString::number(mat.data()[7]),
            QString::number(mat.data()[11]), QString::number(mat.data()[15]));
    }
    qml += tabs.repeated(2) + QString("]\n") + tabs + QString("}");


    return qml;
}

QList<BrainiacGlobals::BrainiacId> Body::boneOrder() const
{
    return m_boneOrder;
}

Agent *Body::agent() const
{
    return m_agent;
}

void Body::skeletonQmlTraverse(const quint32 level, const Bone *bone, QString &qml) const {
    QString tabs("\t");
    tabs = tabs.repeated(level);
    qml += QString("\n") + tabs + QString("Node { \n " + tabs.repeated(2) + "id: ")
           + bone->objectName() + QString("\n " + tabs.repeated(2) + "objectName: \"")
           + bone->objectName() + QString("\"\n " + tabs.repeated(2) + "position: Qt.vector3d(")
           + QString("%1, %2, %3)")
                 .arg(QString::number(bone->translation().x()),
                      QString::number(bone->translation().y()),
                      QString::number(bone->translation().z()))
           + QString("\n " + tabs.repeated(2) + "eulerRotation: Qt.vector3d(")
           + QString("%1, %2, %3)")
                 .arg(QString::number(bone->rotation().x()),
                      QString::number(bone->rotation().y()),
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
