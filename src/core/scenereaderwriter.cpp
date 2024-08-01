#include "scenereaderwriter.h"
#include "scene.h"

SceneReaderWriter::SceneReaderWriter(Scene *parent)
    : BaseReaderWriter(static_cast<QObject *>(parent)), m_scene(parent) {
}
