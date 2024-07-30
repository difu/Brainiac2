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
    explicit GeneratorManual(QObject *parent);
    Locator *addLocator(Agent *agent);
    virtual void apply();

    void updateLocators() override;

    virtual ~GeneratorManual();
};
#endif // GENERATORMANUAL_H
