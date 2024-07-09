#ifndef AGENTREADERWRITER_H
#define AGENTREADERWRITER_H

#include <QObject>
#include "src/core/agent.h"

class AgentReaderWriter : public QObject
{
    Q_OBJECT
public:
    explicit AgentReaderWriter(Agent *parent);
    [[nodiscard]] bool save();
    [[nodiscard]] bool saveAsBAF();

private:
    Agent *m_agent;
    inline static const QString _indent = "    ";

signals:
};

#endif // AGENTREADERWRITER_H
