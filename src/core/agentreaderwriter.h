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

    /**
     * Saves the agent as a BAF file.
     *
     * A BAF file is the "Brainiac Agent File" format. This format is a plain ASCII based format that is meant to be
     * human readable. It is also easily editable with any editor and also can be created and understood by external
     * tools or scripts.
     * 
     * @return true if the agent was successfully saved, false otherwise.
     */
    [[nodiscard]] bool saveAsBAF();

private:
    Agent *m_agent;
    inline static const QString _indent = "    ";

signals:
};

#endif // AGENTREADERWRITER_H
