#include "agentinstacegeometryquick3d.h"

#include <qcolor.h>
#include <QVector3D>
#include "src/core/agentinstance.h"

AgentInstaceGeometryQuick3D::AgentInstaceGeometryQuick3D()
{
    updateData();
}

void AgentInstaceGeometryQuick3D::updateData()
{
    clear();
    int stride = 7 * sizeof(float);

    QColor color = QColor(255, 0, 0, 0);

    QByteArray vertexData(2 * 3 * stride, Qt::Initialization::Uninitialized);
    float *p = reinterpret_cast<float *>(vertexData.data());
    *p++ = -10.0f;
    *p++ = .0f;
    *p++ = -10.0f;

    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;

    *p++ = 10.0f;
    *p++ = 0.0f;
    *p++ = -10.0f;

    *p++ = 0.0f;
    *p++ = 255.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;

    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 20.0f;

    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;

    // Second triangle

    *p++ = -10.0f;
    *p++ = 10.0f;
    *p++ = -10.0f;

    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;

    *p++ = 10.0f;
    *p++ = 10.0f;
    *p++ = -10.0f;

    *p++ = 255.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;

    *p++ = 0.0f;
    *p++ = 10.0f;
    *p++ = 20.0f;

    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;
    *p++ = 0.0f;


    setVertexData(vertexData);
    setStride(stride);
    setBounds(QVector3D(-10.0f, 0.0f, -10.0f), QVector3D(+10.0f, 0.0f, +10.0f));

    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::ColorSemantic,
                 3 * sizeof(float),
                 QQuick3DGeometry::Attribute::F32Type);
}

QVariant AgentInstaceGeometryQuick3D::agentInstance() const
{
    QPointer<AgentInstance> qp(m_agentInstance);
    QVariant qv = QVariant::fromValue(qp);
    return qv;
}

void AgentInstaceGeometryQuick3D::setAgentInstance(AgentInstance *newAgentInstance)
{
    if (m_agentInstance == newAgentInstance)
        return;
    m_agentInstance = newAgentInstance;
    m_agentInstance->setGeometryQuick3DNode(this);
    emit agentInstanceChanged();
}

void AgentInstaceGeometryQuick3D::setAgentInstance(QVariant newAgentInstance)
{
    //qDebug() << newAgentInstance;
    QPointer newAgentInstanceQP = newAgentInstance.value<QPointer<AgentInstance> >();
    AgentInstance *inst = newAgentInstanceQP.data();
    if (inst) {
        this->setAgentInstance(inst);
    } else {
        qWarning() << "newAgentInstance is nullpointer!";
    }
}

QVector3D AgentInstaceGeometryQuick3D::agentTranslation() const
{
    return m_agentInstance->translation();
}

QVector3D AgentInstaceGeometryQuick3D::agentRotation() const
{
    return m_agentInstance->rotation();
}

void AgentInstaceGeometryQuick3D::emitRotationChanged()
{
    emit agentRotationChanged(m_agentInstance->rotation());
}

void AgentInstaceGeometryQuick3D::emitTranslationChanged()
{
    emit agentTranslationChanged(m_agentInstance->translation());
}
