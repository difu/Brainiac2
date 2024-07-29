#include "bone.h"

#include <QMatrix4x4>

#include "body.h"
#include "body/bodyeditoritem.h"
#include "src/core/agent.h"

#include "src/gui/editoritem.h"

Bone::Bone(QObject *parent, const BrainiacGlobals::ItemType type, const BrainiacGlobals::BrainiacId id,
           const BrainiacGlobals::BrainiacId parentId, const QString &name)
    : QObject{parent}, m_type(type), m_id(id), m_parentId(parentId) {
    m_body = qobject_cast<Body *>(parent);
    if (!m_body) {
        qFatal() << "parent must be of type Body!";
    }
    if (id == 0) {
        qFatal() << "Id must not be 0, reserved as root!";
    }
    setBoneName(name);
    Q_ASSERT(!this->objectName().isEmpty());
    m_body->addBone(this);

    m_channelID_TX = m_body->agent()->addOutputChannel(this->objectName().append(":tx"),
                                                       BrainiacGlobals::MINQREAL,
                                                       BrainiacGlobals::MAXQREAL,
                                                       this->translation().x());
    m_channelID_TY = m_body->agent()->addOutputChannel(this->objectName().append(":ty"),
                                                       BrainiacGlobals::MINQREAL,
                                                       BrainiacGlobals::MAXQREAL,
                                                       this->translation().y());
    m_channelID_TZ = m_body->agent()->addOutputChannel(this->objectName().append(":tz"),
                                                       BrainiacGlobals::MINQREAL,
                                                       BrainiacGlobals::MAXQREAL,
                                                       this->translation().z());

    m_channelID_RX = m_body->agent()->addOutputChannel(this->objectName().append(":rx"),
                                                       -360.,
                                                       360.,
                                                       this->rotation().x());
    m_channelID_RY = m_body->agent()->addOutputChannel(this->objectName().append(":ry"),
                                                       -360.,
                                                       360.,
                                                       this->rotation().y());
    m_channelID_RZ = m_body->agent()->addOutputChannel(this->objectName().append(":rz"),
                                                       -360.,
                                                       360.,
                                                       this->rotation().z());
}

BrainiacGlobals::ItemType Bone::type() const {
    return m_type;
}

BrainiacGlobals::BrainiacId Bone::id() const {
    return m_id;
}

BrainiacGlobals::BrainiacId Bone::parentBoneId() const {
    return m_parentId;
}

QString Bone::setBoneName(const QString &newName) {
    foreach(Bone *bone, m_body->bones()) {
        if (bone == this) {
            continue;
        }
        if (bone->objectName() == this->objectName()) {
            return this->setBoneName(this->objectName().append(QString::number(this->id())));
        }
    }
    if (newName.isEmpty()) {
        return this->setBoneName(QString("newBone.").append(QString::number(this->id())));
    } else {
        // this->setBoneNamePriv(newName);
    }
    Q_ASSERT(!newName.isEmpty());
    this->setBoneNamePriv(newName);


    //this->setObjectName(newName);
    Q_ASSERT(!this->objectName().isEmpty());
    return this->objectName();
}

void Bone::setBoneNamePriv(const QString &newName) {
    if (newName == this->objectName()) {
        qDebug() << "New bone name is the same as the old: " << newName;
        return;
    }
    // if objectName is empty, then this is a new bone and no channels have been created yet, we may skip the renaming.
    if (!objectName().isEmpty()) {
        m_body->agent()->renameOutputChannel(objectName().append(":tx"), QString(newName).append(":tx"));
        m_body->agent()->renameOutputChannel(objectName().append(":tz"), QString(newName).append(":tz"));
        m_body->agent()->renameOutputChannel(objectName().append(":ty"), QString(newName).append(":ty"));

        m_body->agent()->renameOutputChannel(objectName().append(":rx"), QString(newName).append(":rx"));
        m_body->agent()->renameOutputChannel(objectName().append(":rz"), QString(newName).append(":rz"));
        m_body->agent()->renameOutputChannel(objectName().append(":ry"), QString(newName).append(":ry"));
    }

    this->setObjectName(newName);
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

QMatrix4x4 Bone::inverseBindMatrix() const {
    QMatrix4x4 local{};
    local.translate(m_translation);
    bool invertible = false;
    QMatrix4x4 localInverted = local.inverted(&invertible);
    if (!invertible) {
        qFatal() << "Cannot invert matrix for bone " << this->id();
    }
    if (m_parentId == 0) {
        return local.inverted();
    }
    const QMatrix4x4 inverseBind = m_body->bones().value(m_parentId)->inverseBindMatrix() * local.inverted();
    return inverseBind;
}

void Bone::setParentId(BrainiacGlobals::BrainiacId newParentId)
{
    m_parentId = newParentId;
}

EditorItem *Bone::editorItem() const {
    return m_editorItem;
}

Body *Bone::body() const {
    return m_body;
}

QPointF Bone::editorPos() const {
    return m_editorItem->pos();
}

void Bone::setEditorPos(const qreal x, const qreal y) {
    m_editorItem->setPos(x, y);
}
