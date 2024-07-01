#ifndef AGENTINSTANCEBODY_H
#define AGENTINSTANCEBODY_H

#include <QObject>

class AgentInstance;
class Body;

class AgentInstanceBody : public QObject
{
    Q_OBJECT
public:
    explicit AgentInstanceBody(QObject *parent);

private:
    AgentInstance *m_agentInstance;
    Body *m_body;

signals:
};

#endif // AGENTINSTANCEBODY_H
