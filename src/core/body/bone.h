#ifndef BONE_H
#define BONE_H

#include <QObject>
#include <QVector3D>

#include "src/core/brainiacglobals.h"

class EditorItem;
class Body;

class Bone : public QObject
{
    Q_OBJECT
    /**
 * @brief Construct a new Bone object with the given parameters.
 *
 * This constructor creates a new Bone object with the provided parent, type, id, parentId, and name.
 * The parent must be of type QObject and represents the parent object of the Bone. The parent object is the Body.
 * The type must be of type BrainiacGlobals::ItemType and represents the type of the Bone.
 * The id must be of type BrainiacGlobals::BrainiacId and represents the unique identifier of the Bone.
 * The parentId must be of type BrainiacGlobals::BrainiacId and represents the parent id of the Bone.
 * The name must be a QString reference and represents the name of the Bone.
 *
 * Upon construction, the Bone object calls the QObject constructor with the given parent.
 * It then initializes the type, id, and parentId member variables with the provided values.
 * It also assigns the parent object to a member variable m_body, which is expected to be of type Body.
 * If the parent is not of type Body, a fatal error is logged.
 * If the id is equal to 0, a fatal error is logged. A parent_id of 0 marks a bone as a root bone.
 * The constructor then calls the setBoneName() method to set the name of the Bone.
 * Finally, it adds the Bone to the parent Body by calling the addBone() method of the Body object.
 *
 * @param parent The parent object of the Bone.
 * @param type The type of the Bone.
 * @param id The unique identifier of the Bone.
 * @param parentId The parent id of the Bone.
 * @param name The name of the Bone.
 */
public:
    Bone(QObject *parent, BrainiacGlobals::ItemType type, BrainiacGlobals::BrainiacId id,
         BrainiacGlobals::BrainiacId parentId, const QString &name);

    [[nodiscard]] BrainiacGlobals::ItemType type() const;
    [[nodiscard]] Body * body() const;

    [[nodiscard]] BrainiacGlobals::BrainiacId id() const;

    /**
     * @brief Get the parent Bone id of the Bone.
     *
     * This method returns the id of the parent Bone.
     *
     * @return The parent id of the parent Bone.
     */
    [[nodiscard]] BrainiacGlobals::BrainiacId parentBoneId() const;

    /**
     * @brief Set the name of the Bone.
     *
     * This method sets the name of the Bone to the given new name. The new name must be provided as a QString reference.
     * If the new name is empty, a default name in the format "newBone.ID" will be set, where ID is the unique identifier
     * assigned to the Bone. If the new name is already in use by another Bone in the same Body, a unique identifier will
     * be appended to the name to make it unique. The new name is set using the setObjectName() function.
     *
     * Note: The parent Bone does not take ownership of the Bone.
     *
     * @param newName The new name for the Bone.
     * @return The final name set for the Bone.
     * @see setObjectName()
     */
    QString setBoneName(const QString& newName);

    [[nodiscard]] QVector3D translation() const;

    void setTranslation(const QVector3D &translation);

    [[nodiscard]] QVector3D rotation() const;

    void setRotation(const QVector3D &rotation);

    /**
     * @brief Calculate the inverse bind matrix of the Bone.
     *
     * This method calculates and returns the inverse bind matrix of the Bone. The inverse bind matrix represents the transformation
     * matrix that is applied to vertices in the local space of the Bone to transform them to the model space of the Bone's parent.
     *
     * The inverse bind matrix is calculated by first creating a local transformation matrix using the Bone's translation.
     * The local transformation matrix is then inverted, and if the inversion is not successful, a fatal error is logged.
     * If the Bone is a root Bone (m_parentId == 0), the inverted local transformation matrix is returned directly.
     * Otherwise, the inverse bind matrix of the Bone's parent is retrieved recursively by calling the inverseBindMatrix()
     * method of the parent Bone, and then multiplied by the inverted local transformation matrix to obtain the final
     * inverse bind matrix of the Bone.
     *
     * @return The inverse bind matrix of the Bone.
     * @see Bone::Bone, Bone::m_parentId, BrainiacGlobals::BrainiacId, QMatrix4x4::translate, QMatrix4x4::inverted, QMatrix4x4::inverted(),
     *      Bone::id, Bone::m_body, Body::bones(), QVector<QMatrix4x4>::value
     */
    [[nodiscard]] QMatrix4x4 inverseBindMatrix() const;

public:
    void setParentId(BrainiacGlobals::BrainiacId newParentId);

    [[nodiscard]] EditorItem *editorItem() const;

    [[nodiscard]] QPointF editorPos() const;


    void setEditorPos(qreal x, qreal y);

    // QTC_TEMP
protected:
    EditorItem *m_editorItem;

private:
    Body *m_body;
    BrainiacGlobals::ItemType m_type;
    BrainiacGlobals::BrainiacId m_id;
    /**
     * @brief The parent id of the Bone.
     *
     * This variable represents the parent id of a Bone, which is a unique identifier assigned to each Bone object.
     * It is of type BrainiacGlobals::BrainiacId, defined in the BrainiacGlobals class in brainiacglobals.h file.
     *
     * The parent id is used to identify the parent Bone of a given Bone object.
     *
     * Note: The parent is not to be confused with the parent of the QObject! The parent bone does not take ownership
     *       of the bone! The parent QObject is the Body!
     *
     * Note: The parent id is initialized to 0 in the Bone constructor.
     *
     * @see Body, Bone, BrainiacGlobals
     */
    BrainiacGlobals::BrainiacId m_parentId;

    QVector3D m_translation;
    QVector3D m_rotation;

    BrainiacGlobals::BrainiacId m_channelID_TX;
    BrainiacGlobals::BrainiacId m_channelID_TY;
    BrainiacGlobals::BrainiacId m_channelID_TZ;

    BrainiacGlobals::BrainiacId m_channelID_RX;
    BrainiacGlobals::BrainiacId m_channelID_RY;
    BrainiacGlobals::BrainiacId m_channelID_RZ;

private:
    void setBoneNamePriv(const QString &newName);

signals:
};

#endif // BONE_H
