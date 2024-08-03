#ifndef SCENEREADERWRITER_H
#define SCENEREADERWRITER_H

#include <QObject>

#include "basereaderwriter.h"
class Agent;
class Scene;

class SceneReaderWriter : public BaseReaderWriter {
    Q_OBJECT

public:
    explicit SceneReaderWriter(Scene *parent);

    bool saveAsBSF();

private:
    void writeAgent(Agent *agent, QTextStream &stream) const;

    Scene *m_scene;
signals:
};

#endif // SCENEREADERWRITER_H
