#include "agentreaderwriter.h"
#include "body/body.h"
#include "body/bonebox.h"
#include "brain/brain.h"
#include "brain/fuzzyand.h"
#include "brain/fuzzyor.h"

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
        while (!stream.atEnd()) {
            QString line = stream.readLine().simplified();
            QStringList fields = line.split(" ");
            foreach(auto field, fields) {
                if (field == "segment") {
                    if (confBlock.type != AgentReaderWriter::UNKNOWN) {
                        qCritical() << "Parsing error!";
                    }
                    confBlock.type = AgentReaderWriter::SEGMENT;
                    //qDebug() << "Found segment!";
                }
                if (field == "endSegment") {
                    if (confBlock.lines.count() > 0) {
                        addSegment(confBlock);
                        confBlock.lines.clear();
                        // cleanLine.clear();
                        confBlock.type = AgentReaderWriter::UNKNOWN;
                    } else {
                        qDebug() << "End of segment reached but empty?";
                    }
                }

                if (field == "fuzz") {
                    if (confBlock.type != AgentReaderWriter::UNKNOWN) {
                        qCritical() << "Parsing error!";
                    }
                    confBlock.type = AgentReaderWriter::FUZZY;
                    //qDebug() << "Found segment!";
                }
                if (field == "endFuzz") {
                    if (confBlock.lines.count() > 0) {
                        addFuzz(confBlock);
                        confBlock.lines.clear();
                        // cleanLine.clear();
                        confBlock.type = AgentReaderWriter::UNKNOWN;
                    } else {
                        qDebug() << "End of segment reached but empty?";
                    }
                }
            }
            confBlock.lines.append(line);
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

bool AgentReaderWriter::saveAsBAF() const {
    QFile file(m_agent->fileName());
    bool success = false;
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

            //QMetaEnum qEnum(BrainiacGlobals::ItemType);
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
            stream << _indent << "editorpos " << bone->editorPos().x() << " " << bone->editorPos().y() << Qt::endl;
            stream << "endSegment" << Qt::endl;
        }

        foreach(auto fuzz, m_agent->brain()->fuzzies()) {
            stream << "fuzz " << fuzz->name() << Qt::endl;
            switch (fuzz->type()) {
            case FuzzyBase::AND: {
                auto *fuzzAnd = dynamic_cast<FuzzyAnd *>(fuzz);
                stream << _indent << "type "
                       << QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                              BrainiacGlobals::AND)
                       << Qt::endl;
                stream << _indent << "mode "
                       << QMetaEnum::fromType<FuzzyAnd::Mode>().valueToKey(fuzzAnd->mode())
                       << Qt::endl;
                // stream << _indent << "size " << fuzzAnd-> << " " << box->size().y() << " "
                //         << box->size().z() << Qt::endl;
                break;
            }
            case FuzzyBase::OR: {
                auto *fuzzyOr = dynamic_cast<FuzzyOr *>(fuzz);
                stream << _indent << "type "
                       << QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                              BrainiacGlobals::OR)
                       << Qt::endl;
                stream << _indent << "mode "
                       << QMetaEnum::fromType<FuzzyOr::Mode>().valueToKey(fuzzyOr->mode())
                       << Qt::endl;
                // stream << _indent << "size " << fuzzAnd-> << " " << box->size().y() << " "
                //         << box->size().z() << Qt::endl;
                break;
            }
            case FuzzyBase::NOISE: {
                auto *noise = dynamic_cast<Noise *>(fuzz);
                stream << _indent << "type "
                       << QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                              BrainiacGlobals::NOISE)
                       << Qt::endl;
                stream << _indent << "rate " << noise->rate() << Qt::endl;
                // stream << _indent << "size " << fuzzAnd-> << " " << box->size().y() << " "
                //         << box->size().z() << Qt::endl;
                break;
            }
            case FuzzyBase::OUTPUT: {
                auto *output = dynamic_cast<FuzzyOutput *>(fuzz);
                stream << _indent << "type "
                       << QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                              BrainiacGlobals::OUTPUT)
                       << Qt::endl;
                stream << _indent << "channel " << m_agent->outputChannelName(output->channelId())
                       << Qt::endl;
                ;
                //         << QMetaEnum::fromType<FuzzyOr::Mode>().valueToKey(
                //             fuzzyOr->mode())
                //         << Qt::endl;
                // stream << _indent << "size " << fuzzAnd-> << " " << box->size().y() << " "
                //         << box->size().z() << Qt::endl;
                break;
            }

                default:
                    break;
            }
            //QMetaEnum qEnum(BrainiacGlobals::ItemType);
            stream << _indent << "editorpos "
                    << fuzz->editorPos().x() << " " << fuzz->editorPos().y() << Qt::endl;
            stream << "endFuzz" << Qt::endl;
        }

        stream << "End" << Qt::endl;
        success = true;
    }
    file.close();
    return success;
}

void AgentReaderWriter::addSegment(ConfigBlock &confBlock) {
    bool primitiveKeyFound = false;
    foreach(auto line, confBlock.lines) {
        // QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(BrainiacGlobals::BOX)
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
                continue;
            }
        }
    }
    Bone *newBone = nullptr;
    switch (confBlock.itemType) {
        case BrainiacGlobals::BOX: {
            BrainiacGlobals::BrainiacId newId = m_agent->body()->newId();
            newBone = new BoneBox(m_agent->body(), newId);
            break;
        }
        default:
            break;
    }
    QString segmentName;
    foreach (auto line, confBlock.lines) {
        QStringList fields = line.split(" ");
        if (fields.count() == 2) {
            if (fields.at(0) == "segment") {
                const QString &segmentName = fields.at(1);
                newBone->setBoneName(segmentName);
                continue;
            }
        }
        if (fields.count() == 2) {
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
        if (fields.count() == 3) {
            if (fields.at(0) == "editorpos") {
                float x = fields.at(1).toFloat();
                float y = fields.at(2).toFloat();
                newBone->setEditorPos(x, y);
            }
        }
        if (fields.count() == 4) {
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

void AgentReaderWriter::addFuzz(ConfigBlock &confBlock)
{
    bool fuzzTypeFound = false;
    foreach (auto line, confBlock.lines) {
        // QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(BrainiacGlobals::BOX)
        QStringList fields = line.split(" ");
        foreach (auto field, fields) {
            if (field == "type") {
                fuzzTypeFound = true;
                continue;
            }
            if (fuzzTypeFound
                && field
                       == QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                           BrainiacGlobals::AND)) {
                confBlock.itemType = BrainiacGlobals::AND;
                fuzzTypeFound = false;
                continue;
            } else if (fuzzTypeFound
                       && field
                              == QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                                  BrainiacGlobals::OR)) {
                confBlock.itemType = BrainiacGlobals::OR;
                fuzzTypeFound = false;
                continue;
            } else if (fuzzTypeFound
                       && field
                              == QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                                  BrainiacGlobals::INPUT)) {
                confBlock.itemType = BrainiacGlobals::INPUT;
                fuzzTypeFound = false;
                continue;
            } else if (fuzzTypeFound
                       && field
                              == QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                                  BrainiacGlobals::OUTPUT)) {
                confBlock.itemType = BrainiacGlobals::OUTPUT;
                fuzzTypeFound = false;
                continue;
            } else if (fuzzTypeFound
                       && field
                              == QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                                  BrainiacGlobals::NOISE)) {
                confBlock.itemType = BrainiacGlobals::NOISE;
                fuzzTypeFound = false;
                continue;
            }
        }
    }
    FuzzyBase *fuzz = nullptr;
    switch (confBlock.itemType) {
        case BrainiacGlobals::NOISE: {
            BrainiacGlobals::BrainiacId newId = m_agent->brain()->newId();
            fuzz = m_agent->brain()->addNoiseNode(newId);
            break;
        }
        case BrainiacGlobals::OR: {
        BrainiacGlobals::BrainiacId newId = m_agent->brain()->newId();
        fuzz = m_agent->brain()->addOrNode(newId);
        break;
        }
        case BrainiacGlobals::AND: {
        BrainiacGlobals::BrainiacId newId = m_agent->brain()->newId();
        fuzz = m_agent->brain()->addAndNode(newId);
        break;
        }
        case BrainiacGlobals::OUTPUT: {
        BrainiacGlobals::BrainiacId newId = m_agent->brain()->newId();
        fuzz = m_agent->brain()->addOutputNode(newId);
        break;
        }

    default:
        qWarning() << "Not implemented: "
                   << QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                          confBlock.itemType);
        return;
    }

    foreach (auto line, confBlock.lines) {
        QStringList fields = line.split(" ");

        if (fields.count() == 2) {
            if (fields.at(0) == "fuzz") {
                const QString &fuzzName = fields.at(1);
                fuzz->setName(fuzzName);
                continue;
            }
        }

        if (fields.count() == 2) {
            if (fields.at(0) == "rate") {
                if (auto *noise = dynamic_cast<Noise *>(fuzz)) {
                    noise->setRate(fields.at(1).toDouble());
                } else {
                    qFatal() << "Could not cast to Noise!";
                }
            }
            if (fields.at(0) == "channel") {
                if(fuzz->type()==FuzzyBase::OUTPUT) {
                    if (auto *out=dynamic_cast<FuzzyOutput *>(fuzz)) {
                        BrainiacGlobals::BrainiacId channelId=m_agent->outputChannels().value(fields.at(1));
                        out->setChannelId(channelId);
                    }
                }
            }
            if (fields.at(0) == "mode") {
                if(fuzz->type()==FuzzyBase::OR) {
                    if (auto *orFuzz=dynamic_cast<FuzzyOr *>(fuzz)) {
                        if(QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                              FuzzyOr::MAX)) {
                            orFuzz->setMode(FuzzyOr::MAX);
                        } else {
                            orFuzz->setMode(FuzzyOr::SUM);
                        }
                    }
                } else if(fuzz->type()==FuzzyBase::AND) {
                    if (auto *andFuzz=dynamic_cast<FuzzyAnd *>(fuzz)) {
                        if(QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                              FuzzyOr::MAX)) {
                            andFuzz->setMode(FuzzyAnd::MIN);
                              } else {
                                  andFuzz->setMode(FuzzyAnd::PRODUCT);
                              }
                    }
                }
            }
        }
        if (fields.count() == 3) {
            if (fields.at(0) == "editorpos") {
                float x = fields.at(1).toFloat();
                float y = fields.at(2).toFloat();
                fuzz->setEditorPos(x, y);
            }
        }
    }
}
