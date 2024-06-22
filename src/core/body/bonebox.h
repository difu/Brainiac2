#ifndef BONEBOX_H
#define BONEBOX_H

#include <QObject>
#include "bone.h"

class BoneBox : public Bone
{
    Q_OBJECT
public:
    BoneBox(QObject *parent, BrainiacGlobals::BrainiacId id, BrainiacGlobals::BrainiacId parentId=0, const QString& name="");
};

#endif // BONEBOX_H
