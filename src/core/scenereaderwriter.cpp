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
            parseFields(fields, confBlock);
            confBlock.lines.append(line);
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
        stream << _indent << "agent " << agent->name() << " " << generator->agentRatios().value(agent) << Qt::endl;
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
}

void SceneReaderWriter::processAgent(ConfigBlock &confBlock) {
    if (confBlock.lines.count() > 0) {
        addAgent(confBlock);
        clearConfigBlock(confBlock);
    } else {
        qDebug() << "End of segment reached but empty?";
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
            if ((words.at(0) == "agent")) {
                agentName = words.at(1);
            }
        }
    }
}

