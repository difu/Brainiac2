#include "bonebox.h"
#include "body/bodyeditoritem.h"

BoneBox::BoneBox(QObject *parent, const BrainiacGlobals::BrainiacId id, const BrainiacGlobals::BrainiacId parentId,
    const QString &name)
    : Bone{parent, BrainiacGlobals::BOX, id, parentId, name} {
    m_editorItem = new BodyEditorItem(this);
}

QVector3D BoneBox::size() const
{
    return m_size;
}

void BoneBox::setSize(const QVector3D &newSize)
{
    m_size = newSize;
}
