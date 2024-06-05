#include "agentinstacegeometryquick3d.h"

#include <qcolor.h>
#include <QVector3D>

#include "src/core/agent.h"
#include "src/core/agentinstance.h"
#include "src/core/brainiacglobals.h"


AgentInstaceGeometryQuick3D::AgentInstaceGeometryQuick3D()
{
    m_agentInstance = nullptr;
    m_geometryIsDirty = true;
    clear();
    updateData();
}

void AgentInstaceGeometryQuick3D::updateData()
{
    if (!m_agentInstance) {
        return;
    }

    if (geometryIsDirty()) {
        static float bla = 01.0;
        bla = bla + 1.0;
        clear();
        qreal agentColor = 0.45481;
        //m_agentInstance->agent()->dumpChannels();
        for (auto [id, channel]: m_agentInstance->outputChannels().asKeyValueRange()) {
            if (id == BrainiacGlobals::CO_COLOR) {
                agentColor = channel->value();
                qDebug() << "Color of agent: " << channel->value();
            }
        }

        const QColor agentQColor = BrainiacGlobals::BrainiacColor(agentColor).toRgb();

        const float red = (float) agentQColor.redF();
        const float green = (float) agentQColor.greenF();
        const float blue = (float) agentQColor.blueF();
        const float alpha = (float) agentQColor.alphaF();


        qDebug() << "Color of agent r: " << red;
        qDebug() << "Color of agent g: " << green;
        qDebug() << "Color of agent b: " << blue;
        qDebug() << "Color of agent a: " << alpha;
        qDebug() << "QColor of agent a: " << agentQColor;


        int stride = 7 * sizeof(float);

        QByteArray vertexData(3 * stride, Qt::Initialization::Uninitialized);
        float *p = reinterpret_cast<float *>(vertexData.data());
        *p++ = -10.0f;
        *p++ = .0f;
        *p++ = -10.0f;

        *p++ = red;
        *p++ = green;
        *p++ = blue;
        *p++ = alpha;

        *p++ = 10.0f;
        *p++ = 0.0f;
        *p++ = -10.0f;

        *p++ = red;
        *p++ = green;
        *p++ = blue;
        *p++ = alpha;

        *p++ = 0.0f;
        *p++ = 0.0f;
        *p++ = 20.0f;

        *p++ = red;
        *p++ = green;
        *p++ = blue;
        *p++ = alpha;
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

        m_geometryIsDirty = false;
    }
    qDebug() << "State of geometry: " << m_geometryIsDirty;
}

bool AgentInstaceGeometryQuick3D::geometryIsDirty() const {
    return m_geometryIsDirty;
}

void AgentInstaceGeometryQuick3D::setGeometryIsDirty() {
    m_geometryIsDirty = true;
    // TODO: Fix this bottleneck. This is a workaround to update the geometry.
    updateData();
    update();
}

QVariant AgentInstaceGeometryQuick3D::agentInstance() const
{
    QPointer<AgentInstance> qp(m_agentInstance);
    QVariant qv = QVariant::fromValue(qp);
    return qv;
}

void AgentInstaceGeometryQuick3D::setAgentInstance(AgentInstance *newAgentInstance) {
    if (m_agentInstance == newAgentInstance) {
        return;
    }
    m_agentInstance = newAgentInstance;
    m_agentInstance->setGeometryQuick3DNode(this);
    emit agentInstanceChanged();
}

void AgentInstaceGeometryQuick3D::setAgentInstance(QVariant newAgentInstance)
{
    //qDebug() << newAgentInstance;
    const auto newAgentInstanceQP = newAgentInstance.value<QPointer<AgentInstance> >();
    AgentInstance *inst = newAgentInstanceQP.data();
    if (inst) {
        this->setAgentInstance(inst);
        updateData();
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

void AgentInstaceGeometryQuick3D::emitRotationChanged() {
    updateData();
    update();
    emit agentRotationChanged(m_agentInstance->rotation());
}

void AgentInstaceGeometryQuick3D::emitTranslationChanged() {
    updateData();
    update();
    emit agentTranslationChanged(m_agentInstance->translation());
}
