#ifndef GENERATORPOINT_H
#define GENERATORPOINT_H

#include <QObject>
#include "generatorbase.h"

class GeneratorPoint : public GeneratorBase
{
    Q_OBJECT
public:
    explicit GeneratorPoint(QObject *parent = nullptr);
};

#endif // GENERATORPOINT_H
