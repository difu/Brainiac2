#include "agentinstancebody.h"

#include "src/core/agent.h"
#include "src/core/agentinstance.h"
#include "src/core/body/agentinstancebonebox.h"
#include "src/core/body/body.h"

AgentInstanceBody::AgentInstanceBody(QObject *parent)
    : QObject{parent}
{
    m_agentInstance = qobject_cast<AgentInstance *>(parent);
    Q_ASSERT(m_agentInstance);
    m_body = m_agentInstance->agent()->body();
    foreach(Bone *bone, m_body->bones()) {
        if (bone->type() == BrainiacGlobals::BOX) {
            auto *box = dynamic_cast<BoneBox *>(bone);
            auto *instanceBoneBox = new AgentInstanceBoneBox(this, box);
            m_bones.insert(bone, instanceBoneBox);
        } else {
            qFatal() << "Unknown BoneType!";
        }
    }
}
