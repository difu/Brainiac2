#ifndef FUZZYOUTPUT_H
#define FUZZYOUTPUT_H

#include <QObject>
#include "fuzzybase.h"

class FuzzyOutput : public FuzzyBase
{
    Q_OBJECT
public:
    explicit FuzzyOutput(QObject *parent = nullptr);
};

#endif // FUZZYOUTPUT_H
