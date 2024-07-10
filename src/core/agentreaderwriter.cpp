#include "agentreaderwriter.h"
#include "body/body.h"
#include "body/bonebox.h"
#include "brain/brain.h"

AgentReaderWriter::AgentReaderWriter(Agent *parent)
    : QObject{parent}
    , m_agent(parent)
{}

bool AgentReaderWriter::loadFromBAF() {
    if (!m_agent->body()->bones().isEmpty() || !m_agent->brain()->fuzzies().isEmpty()) {
        qCritical() << "Agent has bones or fuzzies. A new agent can currently only be loaded if it "
                "is empty!";
    }
    bool success = false;
    QFile file(m_agent->fileName());
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        ConfigBlock confBlock;
        ConfigBlockType confType = AgentReaderWriter::UNKNOWN;
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList fields = line.split(" ");
            QString cleanLine;
            foreach(auto field, fields) {
                if (field.isEmpty()) {
                    // skip blanks
                    continue;
                }
                //qDebug() << "Field:" << field;
                cleanLine.append(field).append(" ");
                if (field == "segment") {
                    if (confBlock.type != AgentReaderWriter::UNKNOWN) {
                        qCritical() << "Parsing error!";
                    }
                    confBlock.type = AgentReaderWriter::SEGMENT;
                    //qDebug() << "Found segment!";
                }
                if (field == "endSegment") {
                    if (confBlock.lines.count() > 0) {
                        qDebug() << "new segment, dispatching current.";
                        addSegment(confBlock);
                        qDebug() << "Dispatched. Emptying.";
                        confBlock.lines.clear();
                        confBlock.type = AgentReaderWriter::UNKNOWN;
                    } else {
                        qDebug() << "End of segment reached but empty?";
                    }
                }
            }
            confBlock.lines.append(cleanLine);
            //qDebug() << "next line";
        }
    }
    file.close();
    return success;
}

bool AgentReaderWriter::save()
{
    return false;
}

bool AgentReaderWriter::saveAsBAF() const
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
            stream << "endSegment" << Qt::endl;
        }
        stream << "End" << Qt::endl;
    }
    return false;
}

void AgentReaderWriter::addSegment(ConfigBlock &confBlock) {
    bool primitiveKeyFound = false;
    foreach(auto line, confBlock.lines) {
        // QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(BrainiacGlobals::BOX)
        qDebug() << line;
        QStringList fields = line.split(" ");
        foreach(auto field, fields) {
            if (field == "primitive") {
                primitiveKeyFound = true;
                continue;
            }
            if (primitiveKeyFound
                && field
                == QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                    BrainiacGlobals::BOX)) {
                confBlock.itemType = BrainiacGlobals::BOX;
                primitiveKeyFound = false;
                qDebug() << "Adding Box!";
                continue;
            }
        }
    }
    Bone *newBone = nullptr;
    switch (confBlock.itemType) {
        case BrainiacGlobals::BOX: {
            BrainiacGlobals::BrainiacId newId = m_agent->body()->newId();
            newBone = new BoneBox(m_agent->body(), newId);
            qDebug() << "Created BoneBox!";
            break;
        }
        default:
            break;
    }
    QString segmentName;
    foreach(auto line, confBlock.lines) {
        qDebug() << "Again: " << line;
        QStringList fields = line.split(" ");
        if (fields.count()
            == 3) {
            // Actually, there are 2 elements, last one is empty (because of blank at the end?
            if (fields.at(0) == "segment") {
                QString segmentName = fields.at(1);
                newBone->setBoneName(segmentName);
                continue;
            }
        }
        if (fields.count()
            == 3) {
            // Actually, there are 2 elements, last one is empty (because of blank at the end?
            if (fields.at(0) == "parent") {
                QString parentName = fields.at(1);
                if (segmentName == parentName) {
                    // is this a root bone?
                    newBone->setParentId(0);
                } else {
                    BrainiacGlobals::BrainiacId parentId = m_agent->body()->boneIdbyName(parentName);
                    newBone->setParentId(parentId);
                }
                continue;
            }
        }
        if (fields.count() == 5) {
            // Actually 4 elements... see above.
            if (fields.at(0) == "translation") {
                float x = fields.at(1).toFloat();
                float y = fields.at(2).toFloat();
                float z = fields.at(3).toFloat();
                QVector3D trans(x, y, z);
                newBone->setTranslation(trans);
                continue;
            }
            if (fields.at(0) == "rotation") {
                float x = fields.at(1).toFloat();
                float y = fields.at(2).toFloat();
                float z = fields.at(3).toFloat();
                QVector3D rot(x, y, z);
                newBone->setRotation(rot);
                continue;
            }
            if (fields.at(0) == "size") {
                if (BoneBox *box = qobject_cast<BoneBox *>(newBone)) {
                    float x = fields.at(1).toFloat();
                    float y = fields.at(2).toFloat();
                    float z = fields.at(3).toFloat();
                    QVector3D size(x, y, z);
                    box->setSize(size);
                } else {
                    qFatal() << "Unexpected key SIZE for this segment!";
                }
                continue;
            }
        }
    }
}
