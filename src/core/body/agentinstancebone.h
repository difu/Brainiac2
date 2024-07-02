#ifndef AGENTINSTANCEBONE_H
#define AGENTINSTANCEBONE_H

#include <QObject>
#include <QQuaternion>
#include <QVector3D>

class Bone;

class AgentInstanceBone : public QObject
{
    Q_OBJECT
public:
    AgentInstanceBone(QObject *parent, Bone *bone);

    [[nodiscard]] Bone *bone() const;
    [[nodiscard]] QVector3D rotation() const;
    void setRotation(const QVector3D &newRotation);
    void resetRotation();
    [[nodiscard]] QVector3D translation() const;
    void setTranslation(const QVector3D &newTranslation);
    void resetTranslation();

    [[nodiscard]] QQuaternion quaternion() const;

private:
    Bone *m_bone;
    QVector3D m_rotation;
    QVector3D m_translation;
    QQuaternion m_quaternion;
signals:
    void rotationChanged();
    void translationChanged();
};

#endif // AGENTINSTANCEBONE_H
