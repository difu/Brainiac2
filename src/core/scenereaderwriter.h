#ifndef SCENEREADERWRITER_H
#define SCENEREADERWRITER_H

#include <QObject>

#include "basereaderwriter.h"
#include "generator/generatorbase.h"
class Agent;
class Scene;

class SceneReaderWriter : public BaseReaderWriter {
    Q_OBJECT

public:
    explicit SceneReaderWriter(Scene *parent);

    [[nosdiscard]] bool saveAsBSF();

    [[nodiscard]] bool loadFromBSF();

    void parseFields(const QStringList &fields, ConfigBlock &confBlock);

private:
    void checkUnknown(const ConfigBlock &confBlock);

    void writeAgent(Agent *agent, QTextStream &stream) const;

    void writeGenerator(GeneratorBase *generator, QTextStream &stream) const;

    void processAgent(ConfigBlock &confBlock);

    void processGenerator(ConfigBlock &confBlock);

    void clearConfigBlock(ConfigBlock &confBlock);

    void addAgent(ConfigBlock &confBlock);

    void addGenerator(ConfigBlock &confBlock);

    void addGeneratorHandle2fields(GeneratorBase *generator,
                                   const QString &field1,
                                   const QString &field2);

    Scene *m_scene;
signals:
};

#endif // SCENEREADERWRITER_H
