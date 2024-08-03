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

void SceneReaderWriter::writeAgent(Agent *agent, QTextStream &stream) const {
    QDir scenePath = m_scene->fileName();
    scenePath.setPath(scenePath.absolutePath());
    scenePath.cdUp();
    QString agentRelativeFileName = scenePath.relativeFilePath(agent->fileName());
    stream << "agent " << agent->name() << Qt::endl;
    stream << _indent << "fileName " << agentRelativeFileName << Qt::endl;
    stream << "endAgent" << Qt::endl;
}

