#ifndef BONE_H
#define BONE_H

#include <QObject>
#include <QVector3D>

#include "src/core/brainiacglobals.h"

class Body;

class Bone : public QObject
{
    Q_OBJECT
public:
    Bone(QObject *parent, BrainiacGlobals::ItemType type, BrainiacGlobals::BrainiacId id,
         BrainiacGlobals::BrainiacId parentId, const QString &name);

    [[nodiscard]] BrainiacGlobals::ItemType type() const;
    [[nodiscard]] Body * body() const;

    [[nodiscard]] BrainiacGlobals::BrainiacId id() const;

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

public:


protected:

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
     * Note: The parent is not to be confused with the parent of the QObject! The parent bobe does not take ownership
     *       of the bone! The parent QObject is the @see Bone
     * Note: The parent id is initialized to 0 in the Bone constructor.
     *
     * @see Bone, BrainiacGlobals
     */
    BrainiacGlobals::BrainiacId m_parent;

    QVector3D m_translation;


private:
    QVector3D m_rotation;


signals:
};

#endif // BONE_H
