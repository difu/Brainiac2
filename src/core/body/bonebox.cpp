#include "bonebox.h"

BoneBox::BoneBox(QObject *parent, const BrainiacGlobals::BrainiacId id, const BrainiacGlobals::BrainiacId parentId,
    const QString &name)
    : Bone{parent, BrainiacGlobals::BOX, id, parentId, name} {

}

