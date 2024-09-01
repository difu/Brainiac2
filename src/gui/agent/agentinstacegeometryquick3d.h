#ifndef AGENTINSTACEGEOMETRYQUICK3D_H
#define AGENTINSTACEGEOMETRYQUICK3D_H

#include <QQuick3DGeometry>
#include <QVector3D>
#include "src/core/body/bone.h"

class AgentInstance;

/**
 * @class AgentInstaceGeometryQuick3D
 * @brief This class represents a quick 3D geometry for an agent instance in a scene.
 *
 * The AgentInstaceGeometryQuick3D class is a QQuick3DGeometry subclass that provides properties and methods
 * for managing the geometry of an agent instance in a quick 3D scene.
 */
class AgentInstaceGeometryQuick3D : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AgentInstaceGeometry)
public:
    AgentInstaceGeometryQuick3D();
    Q_PROPERTY(QVariant agentInstance READ agentInstance WRITE setAgentInstance NOTIFY
                   agentInstanceChanged FINAL)
    Q_PROPERTY(QVector3D agentRotation READ agentRotation NOTIFY agentRotationChanged FINAL)
    QVector3D agentRotation() const;
    Q_PROPERTY(QVector3D agentTranslation READ agentTranslation NOTIFY agentTranslationChanged FINAL)
    QVector3D agentTranslation() const;
    QVariant agentInstance() const;

    Q_INVOKABLE void setAgentInstance(const QVariant &newAgentInstance);

    [[nodiscard]] bool markerGeometryIsDirty() const;

    void setMarkerGeometryIsDirty();

    void emitRotationChanged();
    void emitTranslationChanged();

    [[nodiscard]] QList<QVector3D> vertexPositions() const;

    [[nodiscard]] QList<qint32> joints() const;

    [[nodiscard]] QList<float> weights() const;

    [[nodiscard]] QList<quint32> indexes() const;

    void setVertexPositions(const QList<QVector3D> &vertexPositions);

    void setJoints(const QList<qint32> &joints);

    void setWeights(const QList<float> &weights);

    void setIndexes(const QList<quint32> &indexes);


signals:
    void agentInstanceChanged();
    void agentRotationChanged(QVector3D);
    void agentTranslationChanged(QVector3D);

protected:
    QSSGRenderGraphObject *updateSpatialNode(QSSGRenderGraphObject *node) override;

private:
    void setAgentInstance(AgentInstance *newAgentInstance);

    AgentInstance *m_agentInstance;

    // This section is for skinned geometry only
    /**
     * @brief Adds a cube to the skin geometry.
     *
     * This method adds a cube to the skin geometry with the specified position and dimensions.
     * The cube is associated with the specified joints and weights.
     *
     * @param position The position of the cube.
     * @param dimensions The dimensions of the cube.
     * @param joints The list of joints associated with the cube.
     * @param weights The list of weights associated with each joint.
     */
    void addCube(QVector3D position,
                 QVector3D dimensions,
                 const QList<qint32> &joints,
                 const QList<float> &weights);

    /**
     * @brief Add a cube to the skin geometry.
     *
     * This method adds a cube to the skin geometry with the specified position and dimensions.
     * The cube is associated with the specified joint.
     *
     * @note the position is the absolute position in the agent space.
     *
     * @param position The position of the cube.
     * @param dimensions The dimensions of the cube.
     * @param joint The joint associated with the cube.
     */
    void addCube(QVector3D position, QVector3D dimensions, quint32 joint);

    QList<QVector3D> m_vertexPositions;
    QList<qint32> m_joints;
    QList<float> m_weights;
    QList<quint32> m_indexes;

    bool m_vertexDirty = false;
    bool m_indexDirty = false;

    QByteArray m_vertexBuffer;
    QByteArray m_indexBuffer;

    /**
     * @brief Lookup table to map Bone objects to QObject objects.
     *
     * The m_boneAgentInstanceBoneLookup variable is a QHash that represents a lookup table.
     * It is used to map Bone objects to QObject objects in the AgentInstaceGeometryQuick3D class.
     * The purpose of this lookup table is to associate each Bone object with its corresponding QObject object,
     * allowing for easy and efficient retrieval of joints associated with the bone.
     */
    QHash<Bone *, QObject *> m_boneAgentInstanceBoneLookup;

    // This section is for the marker geometry only
    void updateData();

    bool m_markerGeometryIsDirty;
};

#endif // AGENTINSTACEGEOMETRYQUICK3D_H
