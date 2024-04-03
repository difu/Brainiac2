#ifndef BRAIN_H
#define BRAIN_H

#include <QHash>
#include <QObject>
#include <QString>

#include "../brainiacglobals.h"

class Agent;

class Brain: public QObject
{
    Q_OBJECT
public:
    Brain(QObject *parent = nullptr);

private:
    Agent *m_agent;
};

#endif // BRAIN_H
