#ifndef AGENTINSTANCEBONEBOX_H
#define AGENTINSTANCEBONEBOX_H

#include <QObject>
#include "agentinstancebone.h"

class BoneBox;

class AgentInstanceBoneBox : public AgentInstanceBone
{
public:
    explicit AgentInstanceBoneBox(QObject *parent, BoneBox *boneBox);
};

#endif // AGENTINSTANCEBONEBOX_H
