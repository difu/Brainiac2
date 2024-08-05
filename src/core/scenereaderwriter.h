#ifndef SCENEREADERWRITER_H
#define SCENEREADERWRITER_H

#include <QObject>

#include "basereaderwriter.h"
#include "generator/generatorbase.h"
class Agent;
class Scene;

/**
 * @class SceneReaderWriter
 *
 * @brief The SceneReaderWriter class is responsible for reading and writing Scene objects from and to da file.
 *        It inherits from the BaseReaderWriter class.
 *
 * The class provides methods to save a Scene as BSF (Brainiac Scene Format) and load a Scene from BSF.
 *
 * @since 1.0
 */
class SceneReaderWriter : public BaseReaderWriter {
    Q_OBJECT

public:
    explicit SceneReaderWriter(Scene *parent);

    [[nodiscard]] bool saveAsBSF() const;

    [[nodiscard]] bool loadFromBSF();

    void parseFields(const QStringList &fields, ConfigBlock &confBlock);

private:
    void writeAgent(const Agent *agent, QTextStream &stream) const;

    static void writeGenerator(GeneratorBase *generator, QTextStream &stream);

    void processAgent(ConfigBlock &confBlock);

    void processGenerator(ConfigBlock &confBlock);

    static void clearConfigBlock(ConfigBlock &confBlock);

    void addAgent(const ConfigBlock &confBlock);

    void addGenerator(const ConfigBlock &confBlock);

    static void addGeneratorHandle2Fields(GeneratorBase *generator,
                                          const QString &field1,
                                          const QString &field2);

    void addGeneratorHandle3Fields(GeneratorBase *generator,
                                   const QString &field1,
                                   const QString &field2,
                                   const QString &field3) const;

    static void addGeneratorHandle4Fields(GeneratorBase *generator,
                                          const QString &field1,
                                          const QString &field2,
                                          const QString &field3,
                                          const QString &field4);

    Scene *m_scene;
signals:
};

#endif // SCENEREADERWRITER_H
