#include "agentinstacegeometryquick3d.h"

#include <qcolor.h>
#include <QVector3D>

#include "qqmlapplicationengine.h"
#include "qquaternion.h"
#include "src/core/agent.h"
#include "src/core/agentinstance.h"
#include "src/core/body/body.h"
#include "src/core/body/bonebox.h"
#include "src/core/brainiacglobals.h"
#include "src/core/scene.h"

struct Vertex {
    QVector3D position;
    qint32 joints[4];
    float weights[4];

    float pad;
};

static const int s_vertexSize = sizeof(Vertex);

AgentInstaceGeometryQuick3D::AgentInstaceGeometryQuick3D()
{
    m_agentInstance = nullptr;
    m_markerGeometryIsDirty = true;

    m_indexes.clear();
    m_vertexPositions.clear();
    m_weights.clear();
    m_joints.clear();

    clear();
    updateData();
}

void AgentInstaceGeometryQuick3D::updateData()
{
    if (!m_agentInstance) {
        return;
    }
    // This geometry shall only be used, if the agent's body does not have any bones. If that, only display a
    // triangle with the agents color.
    // TODO: Check if it is sufficient to check if m_vertexPositions is empty?
    if (m_agentInstance->agent()->body()->bones().count() > 0) {
        return;
    }

    if (markerGeometryIsDirty()) {
        clear();
        qreal agentColor = 0.45481;
        //m_agentInstance->agent()->dumpChannels();
        for (auto [id, channel]: m_agentInstance->outputChannels().asKeyValueRange()) {
            if (id == BrainiacGlobals::CO_COLOR) {
                agentColor = channel->value();
                //               qDebug() << "Color of agent: " << channel->value();
            }
        }

        const QColor agentQColor = BrainiacGlobals::BrainiacColor(agentColor).toRgb();

        const float red = (float) agentQColor.redF();
        const float green = (float) agentQColor.greenF();
        const float blue = (float) agentQColor.blueF();
        const float alpha = (float) agentQColor.alphaF();

        // qDebug() << "Color of agent r: " << red;
        // qDebug() << "Color of agent g: " << green;
        // qDebug() << "Color of agent b: " << blue;
        // qDebug() << "Color of agent a: " << alpha;
        // qDebug() << "QColor of agent a: " << agentQColor;

        static constexpr int stride = 7 * sizeof(float);

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

        m_markerGeometryIsDirty = false;
    }
    qDebug() << "State of geometry: " << m_markerGeometryIsDirty;
}

bool AgentInstaceGeometryQuick3D::markerGeometryIsDirty() const {
    return m_markerGeometryIsDirty;
}

void AgentInstaceGeometryQuick3D::setMarkerGeometryIsDirty() {
    m_markerGeometryIsDirty = true;
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

    if (!m_agentInstance->agent()->body()->bones().empty()) {
        addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0,
                     QQuick3DGeometry::Attribute::ComponentType::F32Type);
        addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0,
                     QQuick3DGeometry::Attribute::ComponentType::U32Type);
        addAttribute(QQuick3DGeometry::Attribute::JointSemantic, offsetof(Vertex, joints[0]),
                QQuick3DGeometry::Attribute::ComponentType::I32Type
        )
        ;
        addAttribute(QQuick3DGeometry::Attribute::WeightSemantic, offsetof(Vertex, weights[0]),
                QQuick3DGeometry::Attribute::ComponentType::F32Type
        )
        ;

        // foreach (QObject *obj, viewer->findChildren<QObject *>("root")) {
        //     qDebug() << "Found QML: " << obj->objectName();
        //     QQuaternion q = QQuaternion::fromEulerAngles(10, 20, 40);
        //     obj->setProperty("rotation", QVariant(q));
        // }
        QQmlApplicationEngine *engine = m_agentInstance->agent()->scene()->qQmlApplicationEngine();
        QObject *viewer = engine->rootObjects().constFirst();
        foreach(Bone *bone, m_agentInstance->agent()->body()->bones()) {
            if (qobject_cast<BoneBox *>(bone)) {
                quint32 boneIndex = m_agentInstance->agent()->body()->boneOrder().indexOf(bone->id());
                addCube(bone->translation(), QVector3D(10, 10, 10), boneIndex);
                qDebug() << "Added Bone " << bone->objectName() << " Id " << bone->id()
                        << " with boneIndex " << boneIndex;
            }

            auto *joint = viewer->findChild<QObject *>(bone->objectName());
            if (!joint) {
                qFatal() << "Joint with name " << bone->objectName() << "not found!";
            }
            m_boneJointLookup.insert(bone, joint);
        }
    }

    emit agentInstanceChanged();
}

void AgentInstaceGeometryQuick3D::setAgentInstance(const QVariant &newAgentInstance) {
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

QList<QVector3D> AgentInstaceGeometryQuick3D::vertexPositions() const {
    return m_vertexPositions;
}

QList<qint32> AgentInstaceGeometryQuick3D::joints() const {
    return m_joints;
}

QList<float> AgentInstaceGeometryQuick3D::weights() const {
    return m_weights;
}

QList<quint32> AgentInstaceGeometryQuick3D::indexes() const {
    return m_indexes;
}

void AgentInstaceGeometryQuick3D::setVertexPositions(const QList<QVector3D> &vertexPositions) {
    if (vertexPositions == m_vertexPositions)
        return;
    m_vertexPositions = vertexPositions;
    // emit positionsChanged();
    m_vertexDirty = true;
}

void AgentInstaceGeometryQuick3D::setJoints(const QList<qint32> &joints) {
    if (joints == m_joints)
        return;
    m_joints = joints;
    // emit jointsChanged();
    m_vertexDirty = true;
}

void AgentInstaceGeometryQuick3D::setWeights(const QList<float> &weights) {
    if (weights == m_weights)
        return;
    m_weights = weights;
    // emit weightsChanged();
    m_vertexDirty = true;
}

void AgentInstaceGeometryQuick3D::setIndexes(const QList<quint32> &indexes) {
    if (indexes == m_indexes)
        return;
    m_indexes = indexes;
    // emit indexesChanged();
    m_indexDirty = true;
}

QSSGRenderGraphObject *AgentInstaceGeometryQuick3D::updateSpatialNode(QSSGRenderGraphObject *node) {
    if (m_vertexPositions.count() > 0) {
        qDebug() << "Seems 2 have Bones!";
    }
    if (m_vertexDirty) {
        m_vertexDirty = false;
        qDebug() << "Num of Verts" << m_vertexPositions.count();
        qDebug() << "Num of Index" << m_indexes.count();
        qDebug() << "Num of weights" << m_weights.count();
        qDebug() << "Num of joints" << m_joints.count();
        constexpr float maxFloat = std::numeric_limits<float>::max();
        auto boundsMin = QVector3D(maxFloat, maxFloat, maxFloat);
        auto boundsMax = QVector3D(-maxFloat, -maxFloat, -maxFloat);

        const int numVertexes = m_vertexPositions.size();
        m_vertexBuffer.resize(numVertexes * s_vertexSize);
        Vertex *vert = reinterpret_cast<Vertex *>(m_vertexBuffer.data());

        for (int i = 0; i < numVertexes; ++i) {
            Vertex &v = vert[i];
            v.position = m_vertexPositions[i];
            if (m_joints.size() >= 4 * (i + 1))
                memcpy(v.joints, m_joints.constData() + 4 * i, 4 * sizeof(qint32));
            else
                v.joints[0] = v.joints[1] = v.joints[2] = v.joints[3] = 0;
            if (m_weights.size() >= 4 * (i + 1))
                memcpy(v.weights, m_weights.constData() + 4 * i, 4 * sizeof(float));
            else
                v.weights[0] = v.weights[1] = v.weights[2] = v.weights[3] = 0.0f;

            boundsMin.setX(std::min(boundsMin.x(), v.position.x()));
            boundsMin.setY(std::min(boundsMin.y(), v.position.y()));
            boundsMin.setZ(std::min(boundsMin.z(), v.position.z()));

            boundsMax.setX(std::max(boundsMax.x(), v.position.x()));
            boundsMax.setY(std::max(boundsMax.y(), v.position.y()));
            boundsMax.setZ(std::max(boundsMax.z(), v.position.z()));
        }

        setStride(s_vertexSize);
        setVertexData(m_vertexBuffer);
        setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
        setBounds(boundsMin, boundsMax);
    }

    if (m_indexDirty) {
        qDebug() << "IndexDirty";
        m_indexDirty = false;
        m_indexBuffer = QByteArray(reinterpret_cast<char *>(m_indexes.data()), m_indexes.size() * sizeof(quint32));
        setIndexData(m_indexBuffer);
    }
    //}

    node = QQuick3DGeometry::updateSpatialNode(node);
    return node;
}

void AgentInstaceGeometryQuick3D::addCube(QVector3D position, QVector3D dimensions, quint32 joint) {
    qDebug() << "Adding weights for joint:" << joint;
    QList<float> temp_weights;
    QList<qint32> temp_joints;
    for (int i = 0; i < 8; i++) {
        temp_weights.append(1);
        temp_weights.append(0);
        temp_weights.append(0);
        temp_weights.append(0);

        temp_joints.append(joint);
        temp_joints.append(0);
        temp_joints.append(0);
        temp_joints.append(0);
    }
    addCube(position, dimensions, temp_joints, temp_weights);
}

void AgentInstaceGeometryQuick3D::addCube(QVector3D position,
                                          QVector3D dimensions,
                                          QList<qint32> joints,
                                          QList<float> weights) {
    QList<QVector3D> verts;
    quint32 newVertexIndexOffset = m_vertexPositions.size();
    verts.append((QVector3D(-0.5, -0.5, -0.5) * dimensions) + position);
    verts.append((QVector3D(0.5, -0.5, -0.5) * dimensions) + position);
    verts.append((QVector3D(-0.5, -0.5, 0.5) * dimensions) + position);
    verts.append((QVector3D(0.5, -0.5, 0.5) * dimensions) + position);
    verts.append((QVector3D(-0.5, 0.5, -0.5) * dimensions) + position);
    verts.append((QVector3D(0.5, 0.5, -0.5) * dimensions) + position);
    verts.append((QVector3D(-0.5, 0.5, 0.5) * dimensions) + position);
    verts.append((QVector3D(0.5, 0.5, 0.5) * dimensions) + position);

    QList<quint32> indexes;
    // Bottom triangles
    indexes.append(1 + newVertexIndexOffset);
    indexes.append(2 + newVertexIndexOffset);
    indexes.append(0 + newVertexIndexOffset);
    indexes.append(2 + newVertexIndexOffset);
    indexes.append(3 + newVertexIndexOffset);
    indexes.append(1 + newVertexIndexOffset);
    // // Top triangles
    indexes.append(4 + newVertexIndexOffset);
    indexes.append(6 + newVertexIndexOffset);
    indexes.append(7 + newVertexIndexOffset);
    indexes.append(5 + newVertexIndexOffset);
    indexes.append(4 + newVertexIndexOffset);
    indexes.append(7 + newVertexIndexOffset);

    // // Back triangles
    indexes.append(5 + newVertexIndexOffset);
    indexes.append(0 + newVertexIndexOffset);
    indexes.append(4 + newVertexIndexOffset);
    indexes.append(1 + newVertexIndexOffset);
    indexes.append(0 + newVertexIndexOffset);
    indexes.append(5 + newVertexIndexOffset);

    // Front Triangles
    indexes.append(6 + newVertexIndexOffset);
    indexes.append(2 + newVertexIndexOffset);
    indexes.append(3 + newVertexIndexOffset);
    indexes.append(6 + newVertexIndexOffset);
    indexes.append(3 + newVertexIndexOffset);
    indexes.append(7 + newVertexIndexOffset);

    // Left Triangles
    indexes.append(6 + newVertexIndexOffset);
    indexes.append(4 + newVertexIndexOffset);
    indexes.append(2 + newVertexIndexOffset);
    indexes.append(0 + newVertexIndexOffset);
    indexes.append(2 + newVertexIndexOffset);
    indexes.append(4 + newVertexIndexOffset);

    // Right Triangles
    indexes.append(7 + newVertexIndexOffset);
    indexes.append(5 + newVertexIndexOffset);
    indexes.append(1 + newVertexIndexOffset);
    indexes.append(1 + newVertexIndexOffset);
    indexes.append(3 + newVertexIndexOffset);
    indexes.append(7 + newVertexIndexOffset);

    if (joints.empty() && weights.empty()) {
        qDebug() << "No joints or weights given. Set default.";
        QList<float> temp_weights;
        QList<qint32> temp_joints;
        for (int i = 0; i < verts.count(); i++) {
            temp_weights.append(1);
            temp_weights.append(0);
            temp_weights.append(0);
            temp_weights.append(0);
            temp_joints.append(0);
            temp_joints.append(0);
            temp_joints.append(0);
            temp_joints.append(0);
        }
        m_joints.append(temp_joints);
        m_weights.append(temp_weights);
    } else {
        if (joints.size() == weights.size() && joints.size() == verts.size() * 4) {
            m_joints.append(joints);
            m_weights.append(weights);
        } else {
            qDebug() << "Num of Verts" << verts.count();
            qDebug() << "Num of Index" << indexes.count();
            qDebug() << "Num of weights" << m_weights.count();
            qDebug() << "Num of joints" << m_joints.count();
            qFatal() << "Number of weights or joints does not match number of vertices!";
        }
    }
    m_vertexPositions.append(verts);

    m_indexes.append(indexes);

    m_indexDirty = true;
    m_vertexDirty = true;
}
