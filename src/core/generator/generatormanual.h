#ifndef GENERATORMANUAL_H
#define GENERATORMANUAL_H

#include <QObject>
#include "generatorbase.h"

class Agent;

/**
 * @class GeneratorManual
 * @brief A class that represents a manual generator.
 *
 * The GeneratorManual class inherits from the GeneratorBase class and provides
 * the functionality to manually generate location data of an agent instance.
 * It allows adding locators and applying the generated data.
 */
class GeneratorManual : public GeneratorBase
{
    Q_OBJECT
public:
    explicit GeneratorManual(Scene *parent);

    Locator *addLocator(Agent *agent);
    virtual void instanciateAgentInstances();

    void updateLocators() override;

    virtual ~GeneratorManual();

    [[nodiscard]] static bool compare(GeneratorManual *generator1, GeneratorManual *generator2,
                                      QStringList &differences);
};
#endif // GENERATORMANUAL_H
