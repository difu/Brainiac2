#include "brain.h"

#include <QtDebug>
#include "../agent.h"

Brain::Brain(QObject *parent)
    : QObject{parent}
{
    m_agent = qobject_cast<Agent *> (parent);
}
