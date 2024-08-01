#ifndef SCENEREADERWRITER_H
#define SCENEREADERWRITER_H

#include <QObject>

#include "basereaderwriter.h"
class Scene;

class SceneReaderWriter : public BaseReaderWriter {
    Q_OBJECT

public:
    explicit SceneReaderWriter(Scene *parent);

private:
    Scene *m_scene;
signals:
};

#endif // SCENEREADERWRITER_H
