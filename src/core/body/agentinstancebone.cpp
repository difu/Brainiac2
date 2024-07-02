#include "agentinstancebone.h"
#include "src/core/body/bone.h"

AgentInstanceBone::AgentInstanceBone(QObject *parent, Bone *bone)
    : QObject{parent}
    , m_bone{bone}
{}

Bone *AgentInstanceBone::bone() const
{
    return m_bone;
}

QVector3D AgentInstanceBone::rotation() const
{
    return m_rotation;
}

void AgentInstanceBone::setRotation(const QVector3D &newRotation)
{
    if (m_rotation == newRotation)
        return;
    m_rotation = newRotation;
    emit rotationChanged();
}

void AgentInstanceBone::resetRotation()
{
    setRotation({}); // TODO: Adapt to use your actual default value
}

QVector3D AgentInstanceBone::translation() const
{
    return m_translation;
}

void AgentInstanceBone::setTranslation(const QVector3D &newTranslation)
{
    if (m_translation == newTranslation)
        return;
    m_translation = newTranslation;
    emit translationChanged();
}

void AgentInstanceBone::resetTranslation()
{
    setTranslation({}); // TODO: Adapt to use your actual default value
}

QQuaternion AgentInstanceBone::quaternion() const
{
    return m_quaternion;
}
