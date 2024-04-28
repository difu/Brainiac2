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
    Q_PROPERTY(QVector3D agentTranslation READ agentTranslation NOTIFY agentTranslationChanged FINAL)
    QVector3D agentTranslation() const;
    QVariant agentInstance() const;
    Q_INVOKABLE void setAgentInstance(QVariant newAgentInstance);
    void emitTranslationChanged();

signals:
    void agentInstanceChanged();
    void agentTranslationChanged(QVector3D);

private:
    void setAgentInstance(AgentInstance *newAgentInstance);
    void updateData();
    AgentInstance *m_agentInstance;
};

#endif // AGENTINSTACEGEOMETRYQUICK3D_H
