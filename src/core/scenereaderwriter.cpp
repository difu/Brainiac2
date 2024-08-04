#include "scenereaderwriter.h"

#include "agent.h"
#include "scene.h"

#include <src/core/generator/generatorpoint.h>

SceneReaderWriter::SceneReaderWriter(Scene *parent)
    : BaseReaderWriter(static_cast<QObject *>(parent)), m_scene(parent) {
}

bool SceneReaderWriter::saveAsBSF() {
    QFile file(m_scene->fileName());
    bool success = false;
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        foreach(auto *agent, m_scene->agents()) {
            writeAgent(agent, stream);
        }
        foreach(auto *generator, m_scene->generators()) {
            writeGenerator(generator, stream);
        }
        stream << "End" << Qt::endl;
        success = true;
    }
    file.close();
    return success;
}

bool SceneReaderWriter::loadFromBSF() {
    bool success = false;
    QFile file(m_scene->fileName());
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        ConfigBlock confBlock;
        while (!stream.atEnd()) {
            QString line = stream.readLine().simplified();
            QStringList fields = line.split(" ");
            confBlock.lines.append(line);
            parseFields(fields, confBlock);
        }
    }
    file.close();
    return success;
}

void SceneReaderWriter::parseFields(const QStringList &fields, ConfigBlock &confBlock) {
    using Action = std::function<void(void)>;
    const std::map<QString, Action> actionMap = {
        {
            "agent",
            [&confBlock, this] {
                checkUnknown(confBlock);
                confBlock.type = BaseReaderWriter::AGENT;
            }
        },
        {"endAgent", [&confBlock, this] { processAgent(confBlock); }},
        {
            "generator",
            [&confBlock, this] {
                checkUnknown(confBlock);
                confBlock.type = BaseReaderWriter::GENERATOR;
            }
        },
        {"endGenerator", [&confBlock, this] { processGenerator(confBlock); }},
    };

    foreach(auto field, fields) {
        if (actionMap.find(field) != actionMap.end()) {
            actionMap.at(field)();
        }
    }
}

void SceneReaderWriter::checkUnknown(const ConfigBlock &confBlock) {
    if (confBlock.type != BaseReaderWriter::UNKNOWN) {
        qCritical() << "Parsing error!";
    }
}

void SceneReaderWriter::writeAgent(Agent *agent, QTextStream &stream) const {
    QDir scenePath = m_scene->fileName();
    scenePath.setPath(scenePath.absolutePath());
    scenePath.cdUp();
    QString agentRelativeFileName = scenePath.relativeFilePath(agent->fileName());
    stream << "agent " << agent->name() << Qt::endl;
    stream << _indent << "fileName " << agentRelativeFileName << Qt::endl;
    stream << "endAgent" << Qt::endl;
}

void SceneReaderWriter::writeGenerator(GeneratorBase *generator, QTextStream &stream) const {
    stream << "generator " << Qt::endl;
    stream << _indent << "type " << QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
        generator->type()) << Qt::endl;
    foreach(auto *agent, generator->agents()) {
        stream << _indent << "agentName " << agent->name() << " "
                << generator->agentRatios().value(agent) << Qt::endl;
    }
    if (generator->type() == BrainiacGlobals::GENERATORPOINT) {
        auto genPoint = dynamic_cast<GeneratorPoint *>(generator);
        stream << _indent << "centerPoint " << genPoint->centerPoint().x() << " "
                << genPoint->centerPoint().y() << " " << genPoint->centerPoint().z() << Qt::endl;
    }
    stream << _indent << "gap " << generator->gap() << Qt::endl;
    stream << _indent << "numAgents " << generator->numTotalAgents() << Qt::endl;

    stream << _indent << "distance " << generator->distance() << Qt::endl;

    stream << _indent << "rows " << generator->rows() << Qt::endl;
    stream << _indent << "columns " << generator->columns() << Qt::endl;

    stream << _indent << "angle " << generator->angle() << Qt::endl;
    stream << _indent << "angleVariation " << generator->angleVariation() << Qt::endl;

    stream << _indent << "height " << generator->height() << Qt::endl;
    stream << _indent << "heightVariation " << generator->heightVariation() << Qt::endl;
    stream << "endGenerator" << Qt::endl;
}

void SceneReaderWriter::processAgent(ConfigBlock &confBlock) {
    if (confBlock.lines.count() > 0) {
        addAgent(confBlock);
        clearConfigBlock(confBlock);
    } else {
        qDebug() << "End of agent block reached but empty?";
    }
}

void SceneReaderWriter::processGenerator(ConfigBlock &confBlock) {
    if (confBlock.lines.count() > 0) {
        addGenerator(confBlock);
        clearConfigBlock(confBlock);
    } else {
        qDebug() << "End of generator block reached but empty?";
    }
}

void SceneReaderWriter::clearConfigBlock(ConfigBlock &confBlock) {
    confBlock.lines.clear();
    confBlock.type = BaseReaderWriter::UNKNOWN;
}

void SceneReaderWriter::addAgent(ConfigBlock &confBlock) {
    Agent *newAgent = nullptr;
    QString agentName;
    foreach(auto line, confBlock.lines) {
        auto words = line.split(" ");
        const auto numOfWords = words.count();
        if (numOfWords == 2) {
            if ((words.at(0) == "fileName")) {
                QDir scenePath = m_scene->fileName();
                scenePath.setPath(scenePath.absolutePath());
                scenePath.cdUp();
                newAgent = new Agent(m_scene);
                auto fileName = scenePath.path().append("/").append(words.at(1));
                newAgent->setFileName(fileName);
                bool successName = newAgent->setName(agentName);
                if (!successName) {
                    qWarning() << "Could not set name of agent";
                }
                newAgent->load();
            }
            if ((words.at(0)
                 == "agent")) {
                // The name of the agent is directly behind the configBlock opening tag
                agentName = words.at(1);
            }
        }
    }
}

void SceneReaderWriter::addGenerator(ConfigBlock &confBlock) {
    GeneratorBase *generator = nullptr;
    foreach(auto line, confBlock.lines) {
        auto words = line.split(" ");
        const auto numOfWords = words.count();
        if (numOfWords == 2) {
            if (words.at(0) == "type"
                && words.at(1)
                == QMetaEnum::fromType<BrainiacGlobals::ItemType>().valueToKey(
                    BrainiacGlobals::GENERATORPOINT)) {
                if (!generator) {
                    generator = new GeneratorPoint(m_scene);
                } else {
                    qCritical() << "generator has already been set!";
                }
                continue;
            } else {
                qCritical() << "Parsing error, unknown Generator Type!";
            }
            addGeneratorHandle2fields(generator, words.at(0), words.at(1));
        }
    }
}

void SceneReaderWriter::addGeneratorHandle2fields(GeneratorBase *generator,
                                                  const QString &field1,
                                                  const QString &field2) {
    if (field1 == "type") {
        qCritical() << "Parsing error, TYPE must not be handled here!";
    }
    if (field1 == "gap") {
        if (generator) {
            generator->setGap(field2.toDouble());
        }
        return;
    }
    if (field1 == "numAgents") {
        if (generator) {
            generator->setNumTotalAgents(field2.toInt());
        }
        return;
    }
    if (field1 == "rows") {
        if (generator) {
            generator->setRows(field2.toInt());
        }
        return;
    }
    if (field1 == "columns") {
        if (generator) {
            generator->setColumns(field2.toInt());
        }
        return;
    }
    if (field1 == "distance") {
        if (generator) {
            generator->setDistance(field2.toDouble());
        }
        return;
    }
    if (field1 == "angle") {
        if (generator) {
            generator->setAngle(field2.toDouble());
        }
        return;
    }
    if (field1 == "angleVariation") {
        if (generator) {
            generator->setAngleVariation(field2.toDouble());
        }
        return;
    }
    if (field1 == "height") {
        if (generator) {
            generator->setHeight(field2.toDouble());
        }
        return;
    }
    if (field1 == "heightVariation") {
        if (generator) {
            generator->setHeightVariation(field2.toDouble());
        }
        return;
    }
}
