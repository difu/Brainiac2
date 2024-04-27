#ifndef GENERATORMANUAL_H
#define GENERATORMANUAL_H

#include <QObject>
#include "generatorbase.h"

class Agent;

class GeneratorManual : public GeneratorBase
{
    Q_OBJECT
public:
    explicit GeneratorManual(QObject *parent);
    Locator *addLocator(Agent *agent);
    virtual void apply();
    virtual ~GeneratorManual();
};
#endif // GENERATORMANUAL_H
