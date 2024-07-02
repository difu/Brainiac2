#ifndef AGENTINSTANCEBODY_H
#define AGENTINSTANCEBODY_H

#include <QObject>
#include "src/core/body/agentinstancebone.h"
#include "src/core/body/bonebox.h"

class AgentInstance;
class Body;

class AgentInstanceBody : public QObject
{
    Q_OBJECT
public:
    explicit AgentInstanceBody(QObject *parent);
    void addBoneBox(const BoneBox *boneBox);

private:
    AgentInstance *m_agentInstance;
    Body *m_body;
    QHash<Bone *, AgentInstanceBone *> m_bones;

signals:
};

#endif // AGENTINSTANCEBODY_H
