#include "scenereaderwriter.h"

#include "agent.h"
#include "scene.h"

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
    foreach(auto line, confBlock.lines) {
        auto words = line.split(" ");
        const auto numOfWords = words.count();
        if (numOfWords == 2) {
            if ((words.at(0) == "fileName")) {
                QDir scenePath = m_scene->fileName();
                scenePath.setPath(scenePath.absolutePath());
                scenePath.cdUp();
                auto newAgent = new Agent(m_scene);
                auto fileName = scenePath.path().append("/").append(words.at(1));
                newAgent->setFileName(fileName);
                newAgent->load();
            }
        }
    }
}

