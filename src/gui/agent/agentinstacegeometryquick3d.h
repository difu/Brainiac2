#ifndef AGENTINSTACEGEOMETRYQUICK3D_H
#define AGENTINSTACEGEOMETRYQUICK3D_H

#include <QQuick3DGeometry>

class AgentInstance;

class AgentInstaceGeometryQuick3D : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AgentInstaceGeometry)
public:
    AgentInstaceGeometryQuick3D();
    Q_PROPERTY(QVariant agentInstance READ agentInstance WRITE setAgentInstance NOTIFY
                   agentInstanceChanged FINAL)
    Q_PROPERTY(QVector3D translation READ translation NOTIFY translationChanged FINAL)
    QVector3D translation() const;
    QVariant agentInstance() const;
    Q_INVOKABLE void setAgentInstance(QVariant newAgentInstance);

signals:
    void agentInstanceChanged();
    void translationChanged();

private:
    void setAgentInstance(AgentInstance *newAgentInstance);
    void updateData();
    AgentInstance *m_agentInstance;
};

#endif // AGENTINSTACEGEOMETRYQUICK3D_H
