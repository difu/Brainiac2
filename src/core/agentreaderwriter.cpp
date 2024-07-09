#include "agentreaderwriter.h"
#include "body/body.h"
#include "body/bonebox.h"

AgentReaderWriter::AgentReaderWriter(Agent *parent)
    : QObject{parent}
    , m_agent(parent)
{}

bool AgentReaderWriter::save()
{
    return false;
}

bool AgentReaderWriter::saveAsBAF()
{
    QFile file(m_agent->fileName());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);

        foreach (auto boneId, m_agent->body()->boneOrder()) {
            auto *bone = m_agent->body()->bones().value(boneId);
            stream << "segment " << bone->objectName() << Qt::endl;
            BrainiacGlobals::BrainiacId parentId = bone->parentBoneId();
            QString parentName;
            if (parentId == 0) {
                parentName = bone->objectName();
            } else {
                parentName = m_agent->body()->bones().value(parentId)->objectName();
            }

            stream << _indent << "parent " << parentName << Qt::endl;
            stream << _indent << "translation " << bone->translation().x() << " "
                   << bone->translation().y() << " " << bone->translation().z() << Qt::endl;
            stream << _indent << "rotation " << bone->rotation().x() << " " << bone->rotation().y()
                   << " " << bone->rotation().z() << Qt::endl;

            QMetaEnum qEnum(BrainiacGlobals::ItemType);
            switch (bone->type()) {
            case BrainiacGlobals::BOX: {
                auto *box = dynamic_cast<BoneBox *>(bone);
                stream << _indent << "primitive "
                       << QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                              BrainiacGlobals::BOX)
                       << Qt::endl;
                stream << _indent << "size " << box->size().x() << " " << box->size().y() << " "
                       << box->size().z() << Qt::endl;
                break;
            }
            default:
                break;
            }
        }
        stream << "End" << Qt::endl;
    }
    return false;
}
